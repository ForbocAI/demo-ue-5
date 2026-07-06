#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/ExponentialHeightFogComponent.h"
#include "Core/ue_fp.hpp"
#include "Engine/Engine.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/GameViewportClient.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/GameUserSettings.h"
#include "HAL/IConsoleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocRenderingProfile, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

struct FRetroCVarEval {
  ForbocAI::Game::Data::FRenderingConsoleVariableSettings Setting;
  FLevelRetroRenderProfile Profile;
};

struct FRetroRuntimeFogEval {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
};

struct FRetroRuntimeOutputEval {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
};

inline FString RenderingProfileDispatchAtom(const char *Atom) {
  return FString(UTF8_TO_TCHAR(Atom));
}

template <typename Declaration>
func::Maybe<int32> FindRenderingDeclarationIndex(
    const FString &Name, const TArray<Declaration> &Declarations) {
  const TArray<int32> Indices = func::index_range(Declarations.Num());
  return func::find_indexed(
      Indices, static_cast<size_t>(Indices.Num()),
      [&Name, &Declarations](int32 Index) {
        return Declarations[Index].Name == Name;
      });
}

template <typename Value> struct TProfileFieldDeclaration {
  FString Name;
  Value FLevelRetroRenderProfile::*Member;

  TProfileFieldDeclaration(const char *InName,
                           Value FLevelRetroRenderProfile::*InMember)
      : Name(RenderingProfileDispatchAtom(InName)), Member(InMember) {}
};

template <typename Apply> struct TRenderingDispatchDeclaration {
  FString Name;
  Apply Run;

  TRenderingDispatchDeclaration(const char *InName, Apply InRun)
      : Name(RenderingProfileDispatchAtom(InName)), Run(InRun) {}
};

using FCVarApply = void (*)(const FRetroCVarEval &);
using FCVarApplyDeclaration = TRenderingDispatchDeclaration<FCVarApply>;

void ApplyFixedIntCVar(const FRetroCVarEval &Eval);
void ApplyProfileIntCVar(const FRetroCVarEval &Eval);
void ApplyFixedFloatCVar(const FRetroCVarEval &Eval);
void ApplyProfileFloatCVar(const FRetroCVarEval &Eval);

template <typename Value> struct TProfileFieldRegistry;

template <> struct TProfileFieldRegistry<int32> {
  static const TArray<TProfileFieldDeclaration<int32>> &Fields() {
    static const TArray<TProfileFieldDeclaration<int32>> RegisteredFields = {
        {"anti_aliasing_method",
         &FLevelRetroRenderProfile::AntiAliasingMethod},
        {"post_process_aa_quality",
         &FLevelRetroRenderProfile::PostProcessAAQuality},
        {"shadow_cascades", &FLevelRetroRenderProfile::ShadowCascades},
        {"shadow_max_resolution",
         &FLevelRetroRenderProfile::ShadowMaxResolution}};
    return RegisteredFields;
  }
};

template <> struct TProfileFieldRegistry<float> {
  static const TArray<TProfileFieldDeclaration<float>> &Fields() {
    static const TArray<TProfileFieldDeclaration<float>> RegisteredFields = {
        {"screen_percentage", &FLevelRetroRenderProfile::ScreenPercentage},
        {"view_distance_scale",
         &FLevelRetroRenderProfile::ViewDistanceScale},
        {"foliage_density_scale",
         &FLevelRetroRenderProfile::FoliageDensityScale},
        {"grass_density_scale",
         &FLevelRetroRenderProfile::GrassDensityScale}};
    return RegisteredFields;
  }
};

const TArray<FCVarApplyDeclaration> &CVarApplyDeclarations() {
  static const TArray<FCVarApplyDeclaration> RegisteredDeclarations = {
      {"fixed_int", ApplyFixedIntCVar},
      {"profile_int", ApplyProfileIntCVar},
      {"fixed_float", ApplyFixedFloatCVar},
      {"profile_float", ApplyProfileFloatCVar}};
  return RegisteredDeclarations;
}

FCVarApplyDeclaration RequiredCVarDeclaration(const FString &Name) {
  const TArray<FCVarApplyDeclaration> &Declarations = CVarApplyDeclarations();
  const func::Maybe<int32> Found =
      FindRenderingDeclarationIndex(Name, Declarations);
  check(Found.hasValue);
  return Declarations[Found.value];
}

func::Maybe<IConsoleVariable *>
FindRenderingProfileConsoleVariable(const FString &Name) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(*Name);
  return func::from_nullable_value<IConsoleVariable *>(Found, Found != nullptr);
}

template <typename Value>
bool ApplyRenderingConsoleVariableValue(const FString &Name, Value NewValue) {
  return func::match(
      FindRenderingProfileConsoleVariable(Name),
      [NewValue](IConsoleVariable *Found) {
        Found->Set(NewValue, ECVF_SetByGameSetting);
        return true;
      },
      [&Name]() {
        UE_LOG(LogForbocRenderingProfile, Warning,
               TEXT("Rendering console variable '%s' is unavailable; "
                    "runtime profile value skipped."),
               *Name);
        return false;
      });
}

void SetCVarFloat(const FString &Name, float Value) {
  ApplyRenderingConsoleVariableValue<float>(Name, Value);
}

void SetCVarInt(const FString &Name, int32 Value) {
  ApplyRenderingConsoleVariableValue<int32>(Name, Value);
}

template <typename Value>
func::Maybe<Value> SelectProfileValue(
    const FLevelRetroRenderProfile &Profile, const FString &Field) {
  const TArray<TProfileFieldDeclaration<Value>> &FieldList =
      TProfileFieldRegistry<Value>::Fields();
  return func::match(
      FindRenderingDeclarationIndex(Field, FieldList),
      [&Profile, &FieldList](int32 Index) {
        return func::just<Value>(Profile.*FieldList[Index].Member);
      },
      []() { return func::nothing<Value>(); });
}

template <typename Value> Value RequiredProfileValue(const FRetroCVarEval &Eval) {
  const func::Maybe<Value> ValueResult =
      SelectProfileValue<Value>(Eval.Profile, Eval.Setting.ProfileField);
  check(ValueResult.hasValue);
  return ValueResult.value;
}

int32 RequiredProfileInt(const FRetroCVarEval &Eval) {
  return RequiredProfileValue<int32>(Eval);
}

float RequiredProfileFloat(const FRetroCVarEval &Eval) {
  return RequiredProfileValue<float>(Eval);
}

void ApplyFixedIntCVar(const FRetroCVarEval &Eval) {
  SetCVarInt(Eval.Setting.Name, Eval.Setting.IntValue);
}

void ApplyProfileIntCVar(const FRetroCVarEval &Eval) {
  SetCVarInt(Eval.Setting.Name, RequiredProfileInt(Eval));
}

void ApplyFixedFloatCVar(const FRetroCVarEval &Eval) {
  SetCVarFloat(Eval.Setting.Name, Eval.Setting.FloatValue);
}

void ApplyProfileFloatCVar(const FRetroCVarEval &Eval) {
  SetCVarFloat(Eval.Setting.Name, RequiredProfileFloat(Eval));
}

void ApplyConsoleVariable(const FRetroCVarEval &Eval) {
  RequiredCVarDeclaration(Eval.Setting.ValueKind).Run(Eval);
}

FLinearColor FogColor(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.FogColorR, Profile.FogColorG, Profile.FogColorB,
                      Profile.FogColorA);
}

void ApplyRuntimeFogComponent(UExponentialHeightFogComponent *Component,
                              const FLevelRetroRenderProfile &Profile) {
  check(Component);
  Component->SetVisibility(Profile.bFogEnabled);
  Component->SetFogDensity(Profile.FogDensity);
  Component->SetFogHeightFalloff(Profile.FogHeightFalloff);
  Component->SetStartDistance(Profile.FogStartDistance);
  Component->SetFogCutoffDistance(Profile.FogCutoffDistance);
  Component->SetFogMaxOpacity(Profile.FogMaxOpacity);
  Component->SetFogInscatteringColor(FogColor(Profile));
  Component->SetVolumetricFog(Profile.bVolumetricFogEnabled);
}

void ApplyRuntimeFog(const FRetroRuntimeFogEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        AExponentialHeightFog *Fog =
            WorldValue->SpawnActor<AExponentialHeightFog>(
                FVector::ZeroVector, FRotator::ZeroRotator);
        Fog ? (ApplyRuntimeFogComponent(Fog->GetComponent(), Eval.Profile),
               true)
            : false;
      },
      []() {});
}

FIntPoint SelectViewportSize(UWorld *World) {
  return func::match(
      func::from_nullable_value(World, World != nullptr),
      [](UWorld *WorldValue) {
        UGameViewportClient *Viewport = WorldValue->GetGameViewport();
        return func::match(
            func::from_nullable_value(Viewport, Viewport != nullptr),
            [](UGameViewportClient *ViewportValue) {
              FVector2D Size = FVector2D::ZeroVector;
              ViewportValue->GetViewportSize(Size);
              return FIntPoint(FMath::RoundToInt(Size.X),
                               FMath::RoundToInt(Size.Y));
            },
            []() { return FIntPoint::ZeroValue; });
      },
      []() { return FIntPoint::ZeroValue; });
}

float SelectInternalRenderScreenPercentage(
    const FLevelRetroRenderProfile &Profile, const FIntPoint &ViewportSize) {
  const bool bCanScale = Profile.InternalRenderWidth > 0 &&
                         Profile.InternalRenderHeight > 0 &&
                         ViewportSize.X > 0 && ViewportSize.Y > 0;
  const float WidthScale =
      bCanScale ? (static_cast<float>(Profile.InternalRenderWidth) /
                   static_cast<float>(ViewportSize.X)) *
                      Profile.ScreenPercentage
                : Profile.ScreenPercentage;
  const float HeightScale =
      bCanScale ? (static_cast<float>(Profile.InternalRenderHeight) /
                   static_cast<float>(ViewportSize.Y)) *
                      Profile.ScreenPercentage
                : Profile.ScreenPercentage;
  return FMath::Clamp(FMath::Min(WidthScale, HeightScale),
                      Profile.MinimumScreenPercentage,
                      Profile.ScreenPercentage);
}

FIntPoint ProfileOutputRenderSize(const FLevelRetroRenderProfile &Profile) {
  check(Profile.OutputScaleMultiplier > 0);
  return FIntPoint(Profile.InternalRenderWidth * Profile.OutputScaleMultiplier,
                   Profile.InternalRenderHeight * Profile.OutputScaleMultiplier);
}

FString RuntimeResolutionCommand(const FLevelRetroRenderProfile &Profile) {
  const FIntPoint OutputSize = ProfileOutputRenderSize(Profile);
  return FString::Printf(TEXT("r.SetRes %dx%d%s"), OutputSize.X, OutputSize.Y,
                         Profile.bFullscreenOutput ? TEXT("f") : TEXT("w"));
}

void ApplyRuntimeResolutionCommand(UWorld *World,
                                   const FLevelRetroRenderProfile &Profile) {
  GEngine ? GEngine->Exec(World, *RuntimeResolutionCommand(Profile)) : false;
}

void ApplyRuntimeVideoSettings(const FLevelRetroRenderProfile &Profile) {
  UGameUserSettings *Settings =
      GEngine != nullptr ? GEngine->GetGameUserSettings() : nullptr;
  Settings
      ? (Settings->SetScreenResolution(ProfileOutputRenderSize(Profile)),
         Settings->SetFullscreenMode(Profile.bFullscreenOutput
                                         ? EWindowMode::Fullscreen
                                         : EWindowMode::Windowed),
         Settings->ApplyResolutionSettings(false),
         Settings->ApplySettings(false), void())
      : void();
}

void ApplyRuntimeOutput(const FRetroRuntimeOutputEval &Eval) {
  ApplyRuntimeVideoSettings(Eval.Profile);
  ApplyRuntimeResolutionCommand(Eval.World, Eval.Profile);
  const FIntPoint OutputSize = ProfileOutputRenderSize(Eval.Profile);
  const FIntPoint EffectiveViewportSize =
      OutputSize == FIntPoint::ZeroValue ? SelectViewportSize(Eval.World)
                                         : OutputSize;
  const float EffectiveScreenPercentage =
      SelectInternalRenderScreenPercentage(Eval.Profile,
                                           EffectiveViewportSize);
  SetCVarFloat(TEXT("r.ScreenPercentage"), EffectiveScreenPercentage);
  UE_LOG(LogForbocRenderingProfile, Display,
         TEXT("runtime render output internal=%dx%d output=%dx%d viewport=%dx%d "
              "screen_percentage=%.2f fullscreen=%d"),
         Eval.Profile.InternalRenderWidth, Eval.Profile.InternalRenderHeight,
         OutputSize.X, OutputSize.Y, EffectiveViewportSize.X,
         EffectiveViewportSize.Y, EffectiveScreenPercentage,
         Eval.Profile.bFullscreenOutput ? 1 : 0);
}

} // namespace

rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id) {
  return [Id](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
              std::function<const FRuntimeState &()> GetState)
             -> func::AsyncResult<FRenderingPayload> {
    const FRuntimeState &State = GetState();
    const FRenderingPayload Payload =
        RenderingReducers::ReduceRenderingPayload(
            {Id, RuntimeSelectors::SelectRuntimeProfile(State),
             RuntimeSelectors::SelectTextureCatalog(State),
             RuntimeSelectors::SelectRenderingRuntimeSettings(State)});
    return func::createAsyncResult<FRenderingPayload>(
        [Dispatch, Payload](std::function<void(FRenderingPayload)> Resolve,
                            std::function<void(std::string)> Reject) {
          Dispatch(RenderingActions::RenderingProfileObserved()(Payload));
          Resolve(Payload);
        });
  };
}

void ApplyRuntimeProfile(
    UWorld *World, const FLevelRetroRenderProfile &Profile,
    const ForbocAI::Game::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  func::for_each_indexed(
      RuntimeSettings.ConsoleVariables,
      static_cast<size_t>(RuntimeSettings.ConsoleVariables.Num()),
      [&Profile](
          const ForbocAI::Game::Data::FRenderingConsoleVariableSettings
              &Setting) { ApplyConsoleVariable({Setting, Profile}); });
  ApplyRuntimeOutput({World, Profile});
  ApplyRuntimeFog({World, Profile});
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
