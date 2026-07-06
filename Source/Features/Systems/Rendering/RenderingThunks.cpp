#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/ExponentialHeightFogComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Engine/Engine.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Texture2D.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/GameUserSettings.h"
#include "HAL/FileManager.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformTime.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "UnrealClient.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocRendering, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

struct FRetroTextureCell {
  ELevelRetroTexture Texture;
  int32 X;
  int32 Y;
  const ForbocAI::Game::Data::FRenderingRuntimeSettings *RuntimeSettings;
};

struct FRetroHashCell {
  int32 X;
  int32 Y;
  int32 Salt;
  const ForbocAI::Game::Data::FRenderingTextureHashSettings *Hash;
};

struct FRetroPredicateEval {
  ForbocAI::Game::Data::FRenderingTexturePredicateSettings Predicate;
  int32 X;
  int32 Y;
  int32 Noise;
};

struct FRetroResultEval {
  ForbocAI::Game::Data::FRenderingTextureColorResultSettings Result;
  int32 Noise;
  int32 Alpha;
};

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

inline FString RenderingDispatchAtom(const char *Atom) {
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
      : Name(RenderingDispatchAtom(InName)), Member(InMember) {}
};

template <typename Apply> struct TRenderingDispatchDeclaration {
  FString Name;
  Apply Run;

  TRenderingDispatchDeclaration(const char *InName, Apply InRun)
      : Name(RenderingDispatchAtom(InName)), Run(InRun) {}
};

using FCVarApply = void (*)(const FRetroCVarEval &);
using FPredicateApply = bool (*)(const FRetroPredicateEval &, int32);
using FColorResultApply = FColor (*)(const FRetroResultEval &);
using FCVarApplyDeclaration = TRenderingDispatchDeclaration<FCVarApply>;
using FPredicateApplyDeclaration =
    TRenderingDispatchDeclaration<FPredicateApply>;
using FColorResultDeclaration =
    TRenderingDispatchDeclaration<FColorResultApply>;

void ApplyFixedIntCVar(const FRetroCVarEval &Eval);
void ApplyProfileIntCVar(const FRetroCVarEval &Eval);
void ApplyFixedFloatCVar(const FRetroCVarEval &Eval);
void ApplyProfileFloatCVar(const FRetroCVarEval &Eval);
void ApplyRuntimeFog(const FRetroRuntimeFogEval &Eval);
void ApplyRuntimeOutput(const FRetroRuntimeOutputEval &Eval);
bool AlwaysPredicate(const FRetroPredicateEval &Eval, int32 Term);
bool ModEqualsPredicate(const FRetroPredicateEval &Eval, int32 Term);
bool ModLessThanPredicate(const FRetroPredicateEval &Eval, int32 Term);
FColor SolidColor(const FRetroResultEval &Eval);
FColor Mix(const FRetroResultEval &Eval);

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

template <typename Declaration> struct TRenderingDispatchRegistry;

template <> struct TRenderingDispatchRegistry<FCVarApplyDeclaration> {
  static const TArray<FCVarApplyDeclaration> &Declarations() {
    static const TArray<FCVarApplyDeclaration> RegisteredDeclarations = {
        {"fixed_int", ApplyFixedIntCVar},
        {"profile_int", ApplyProfileIntCVar},
        {"fixed_float", ApplyFixedFloatCVar},
        {"profile_float", ApplyProfileFloatCVar}};
    return RegisteredDeclarations;
  }
};

template <> struct TRenderingDispatchRegistry<FPredicateApplyDeclaration> {
  static const TArray<FPredicateApplyDeclaration> &Declarations() {
    static const TArray<FPredicateApplyDeclaration> RegisteredDeclarations = {
        {"always", AlwaysPredicate},
        {"mod_equals", ModEqualsPredicate},
        {"mod_less_than", ModLessThanPredicate}};
    return RegisteredDeclarations;
  }
};

template <> struct TRenderingDispatchRegistry<FColorResultDeclaration> {
  static const TArray<FColorResultDeclaration> &Declarations() {
    static const TArray<FColorResultDeclaration> RegisteredDeclarations = {
        {"solid", SolidColor}, {"mix", Mix}};
    return RegisteredDeclarations;
  }
};

template <typename Declaration>
Declaration RequiredRenderingDeclaration(const FString &Name) {
  const TArray<Declaration> &Declarations =
      TRenderingDispatchRegistry<Declaration>::Declarations();
  const func::Maybe<int32> Found =
      FindRenderingDeclarationIndex(Name, Declarations);
  check(Found.hasValue);
  return Declarations[Found.value];
}

/**
 * @brief Finds a runtime console variable without asserting on platform gaps.
 * @signature func::Maybe<IConsoleVariable *> FindRenderingConsoleVariable(const FString &Name)
 *
 * User Story: As first-run runtime rendering, I need optional engine CVars to skip
 * cleanly when a target runtime does not register them.
 */
func::Maybe<IConsoleVariable *>
FindRenderingConsoleVariable(const FString &Name) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(*Name);
  return func::from_nullable_value<IConsoleVariable *>(Found, Found != nullptr);
}

/**
 * @brief Applies a runtime console variable value when the CVar exists.
 * @signature template <typename Value> bool ApplyRenderingConsoleVariableValue(const FString &Name, Value NewValue)
 *
 * User Story: As the runtime profile thunk, I need profile application to be
 * resilient to engine CVar availability while still reporting skipped settings.
 */
template <typename Value>
bool ApplyRenderingConsoleVariableValue(const FString &Name, Value NewValue) {
  return func::match(
      FindRenderingConsoleVariable(Name),
      [NewValue](IConsoleVariable *Found) {
        Found->Set(NewValue, ECVF_SetByGameSetting);
        return true;
      },
      [&Name]() {
        UE_LOG(LogForbocRendering, Warning,
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

template <typename Value>
Value RequiredProfileValue(const FRetroCVarEval &Eval) {
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
  RequiredRenderingDeclaration<FCVarApplyDeclaration>(Eval.Setting.ValueKind)
      .Run(Eval);
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

bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) {
  return ElapsedSeconds >= IntervalSeconds;
}

float SelectCommandLineFloat(const FString &Key, float DefaultValue) {
  float Value = DefaultValue;
  const bool bHasValue = FParse::Value(FCommandLine::Get(), *Key, Value);
  return bHasValue ? Value : DefaultValue;
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

FIntPoint ProfileInternalRenderSize(const FLevelRetroRenderProfile &Profile) {
  return FIntPoint(Profile.InternalRenderWidth, Profile.InternalRenderHeight);
}

FIntPoint ProfileOutputRenderSize(const FLevelRetroRenderProfile &Profile) {
  check(Profile.OutputScaleMultiplier > 0);
  return FIntPoint(Profile.InternalRenderWidth * Profile.OutputScaleMultiplier,
                   Profile.InternalRenderHeight * Profile.OutputScaleMultiplier);
}

FString RuntimeResolutionCommand(const FLevelRetroRenderProfile &Profile) {
  const FIntPoint OutputSize = ProfileOutputRenderSize(Profile);
  return FString::Printf(TEXT("r.SetRes %dx%d%s"),
                         OutputSize.X,
                         OutputSize.Y,
                         Profile.bFullscreenOutput ? TEXT("f") : TEXT("w"));
}

void ApplyRuntimeResolutionCommand(
    UWorld *World, const FLevelRetroRenderProfile &Profile) {
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
      OutputSize == FIntPoint::ZeroValue
          ? SelectViewportSize(Eval.World)
          : OutputSize;
  const float EffectiveScreenPercentage =
      SelectInternalRenderScreenPercentage(Eval.Profile,
                                           EffectiveViewportSize);
  SetCVarFloat(TEXT("r.ScreenPercentage"), EffectiveScreenPercentage);
  UE_LOG(LogForbocRendering, Display,
         TEXT("runtime render output internal=%dx%d output=%dx%d viewport=%dx%d "
              "screen_percentage=%.2f fullscreen=%d"),
         Eval.Profile.InternalRenderWidth, Eval.Profile.InternalRenderHeight,
         OutputSize.X,
         OutputSize.Y,
         EffectiveViewportSize.X, EffectiveViewportSize.Y,
         EffectiveScreenPercentage,
         Eval.Profile.bFullscreenOutput ? 1 : 0);
}

FString RuntimeBudgetScreenshotDirectory(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return FPaths::Combine(FPaths::ProjectDir(),
                         Settings.BudgetScreenshotDirectory);
}

FString RuntimeBudgetScreenshotPath(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index) {
  return FPaths::Combine(
      RuntimeBudgetScreenshotDirectory(Settings),
      frmt::RuntimeString(Settings.BudgetScreenshotFileNameFormat,
                          frmt::Args({frmt::Arg(Index)})));
}

FColor Color(const ForbocAI::Game::Data::FRenderingRgbSettings &Rgb,
             int32 Alpha) {
  return FColor(static_cast<uint8>(Rgb.R), static_cast<uint8>(Rgb.G),
                static_cast<uint8>(Rgb.B), static_cast<uint8>(Alpha));
}

FColor Mix(const FRetroResultEval &Eval) {
  check(Eval.Result.Denominator > 0);
  check(Eval.Result.NumeratorNoiseModulus > 0);
  const int32 Numerator =
      Eval.Result.NumeratorBase + Eval.Noise % Eval.Result.NumeratorNoiseModulus;
  const int32 Inverse = Eval.Result.Denominator - Numerator;
  return FColor(
      static_cast<uint8>((Eval.Result.ColorA.R * Inverse +
                          Eval.Result.ColorB.R * Numerator) /
                         Eval.Result.Denominator),
      static_cast<uint8>((Eval.Result.ColorA.G * Inverse +
                          Eval.Result.ColorB.G * Numerator) /
                         Eval.Result.Denominator),
      static_cast<uint8>((Eval.Result.ColorA.B * Inverse +
                          Eval.Result.ColorB.B * Numerator) /
                         Eval.Result.Denominator),
      static_cast<uint8>(Eval.Alpha));
}

FColor SolidColor(const FRetroResultEval &Eval) {
  return Color(Eval.Result.Color, Eval.Alpha);
}

int32 HashCell(FRetroHashCell Cell) {
  check(Cell.Hash);
  int32 Value = Cell.X * Cell.Hash->XMultiplier ^
                Cell.Y * Cell.Hash->YMultiplier ^
                Cell.Salt * Cell.Hash->SaltMultiplier;
  Value ^= Value >> Cell.Hash->XorShift;
  return FMath::Abs(Value);
}

int32 PredicateTerm(const FRetroPredicateEval &Eval) {
  check(Eval.Predicate.XDivisor > 0);
  check(Eval.Predicate.YDivisor > 0);
  return Eval.Predicate.XMultiplier * (Eval.X / Eval.Predicate.XDivisor) +
         Eval.Predicate.YMultiplier * (Eval.Y / Eval.Predicate.YDivisor) +
         Eval.Predicate.NoiseMultiplier * Eval.Noise;
}

bool AlwaysPredicate(const FRetroPredicateEval &, int32) { return true; }

bool ModEqualsPredicate(const FRetroPredicateEval &Eval, int32 Term) {
  check(Eval.Predicate.Modulus > 0);
  return Term % Eval.Predicate.Modulus == Eval.Predicate.Equals;
}

bool ModLessThanPredicate(const FRetroPredicateEval &Eval, int32 Term) {
  check(Eval.Predicate.Modulus > 0);
  return Term % Eval.Predicate.Modulus < Eval.Predicate.LessThan;
}

bool PredicateMatches(const FRetroPredicateEval &Eval) {
  const int32 Term = PredicateTerm(Eval);
  return RequiredRenderingDeclaration<FPredicateApplyDeclaration>(
             Eval.Predicate.Kind)
      .Run(Eval, Term);
}

FColor ResolveColor(const FRetroResultEval &Eval) {
  return RequiredRenderingDeclaration<FColorResultDeclaration>(Eval.Result.Kind)
      .Run(Eval);
}

ForbocAI::Game::Data::FRenderingTexturePaletteSettings
RequiredPalette(const FRetroTextureCell &Cell) {
  check(Cell.RuntimeSettings);
  const func::Maybe<ForbocAI::Game::Data::FRenderingTexturePaletteSettings>
      Palette = func::find_indexed(
          Cell.RuntimeSettings->TexturePalettes,
          static_cast<size_t>(Cell.RuntimeSettings->TexturePalettes.Num()),
          [&Cell](
              const ForbocAI::Game::Data::FRenderingTexturePaletteSettings
                  &Candidate) {
            return RenderingReducers::ReduceTextureKind(Candidate.Texture) ==
                   Cell.Texture;
          });
  check(Palette.hasValue);
  return Palette.value;
}

FColor PaletteColor(const FRetroTextureCell &Cell) {
  check(Cell.RuntimeSettings);
  const int32 Noise =
      HashCell({Cell.X, Cell.Y, static_cast<int32>(Cell.Texture),
                &Cell.RuntimeSettings->TextureHash});
  const ForbocAI::Game::Data::FRenderingTexturePaletteSettings Palette =
      RequiredPalette(Cell);
  const func::Maybe<ForbocAI::Game::Data::FRenderingTextureRuleSettings> Rule =
      func::find_indexed(
          Palette.Rules, static_cast<size_t>(Palette.Rules.Num()),
          [&Cell, Noise](
              const ForbocAI::Game::Data::FRenderingTextureRuleSettings
                  &Candidate) {
            return PredicateMatches(
                {Candidate.Predicate, Cell.X, Cell.Y, Noise});
          });
  check(Rule.hasValue);
  return ResolveColor({Rule.value.Result, Noise,
                       Cell.RuntimeSettings->TextureAlpha});
}

FString TextureCacheKey(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  return frmt::RuntimeString(
      RuntimeSettings.TextureCacheKeyFormat,
      frmt::Args(
          {frmt::Arg(static_cast<int32>(Spec.Texture)),
           frmt::Arg(Spec.Size.X),
           frmt::Arg(Spec.Size.Y)}));
}

UTexture2D *CreateRetroTexture(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  check(Spec.Size.X > 0);
  check(Spec.Size.Y > 0);

  UTexture2D *Result =
      UTexture2D::CreateTransient(Spec.Size.X, Spec.Size.Y, PF_B8G8R8A8);
  check(Result);
  check(Result->GetPlatformData());
  check(Result->GetPlatformData()->Mips.Num() > 0);

  Result->Filter = TF_Nearest;
  Result->AddressX = TA_Wrap;
  Result->AddressY = TA_Wrap;
  Result->MipGenSettings = TMGS_NoMipmaps;
  Result->NeverStream = true;
  Result->SRGB = true;

  FTexture2DMipMap &Mip = Result->GetPlatformData()->Mips[0];
  void *Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
  check(Data);

  const std::vector<FColor> Pixels = func::map_grid<FColor>(
      static_cast<size_t>(Spec.Size.Y), static_cast<size_t>(Spec.Size.X),
      [&Spec, &RuntimeSettings](const func::GridIndex &Index) {
        return PaletteColor({Spec.Texture, static_cast<int32>(Index.Column),
                             static_cast<int32>(Index.Row),
                             &RuntimeSettings});
      });
  FMemory::Memcpy(Data, Pixels.data(),
                  Pixels.size() * RuntimeSettings.TextureChannels *
                      sizeof(uint8));
  Mip.BulkData.Unlock();

  Result->UpdateResource();
  Result->AddToRoot();
  return Result;
}

UTexture2D *TextureFor(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  static TMap<FString, UTexture2D *> TextureCache;
  const FString Key = TextureCacheKey(Spec, RuntimeSettings);
  UTexture2D **Cached = TextureCache.Find(Key);
  return Cached ? *Cached
                : [&]() {
                    UTexture2D *Created =
                        CreateRetroTexture(Spec, RuntimeSettings);
                    TextureCache.Add(Key, Created);
                    return Created;
                  }();
}
} // namespace

rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id) {
  return [Id](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
              std::function<FRuntimeState()> GetState)
             -> func::AsyncResult<FRenderingPayload> {
    const FRuntimeState State = GetState();
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
    UWorld *World,
    const FLevelRetroRenderProfile &Profile,
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

float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return SelectCommandLineFloat(
      Settings.BudgetScreenshotIntervalCommandLineKey,
      Settings.BudgetScreenshotIntervalSeconds);
}

double SelectRuntimeBudgetClockSeconds() { return FPlatformTime::Seconds(); }

bool ShouldRunRuntimeBudgetWallInterval(double CurrentSeconds,
                                        double LastSeconds,
                                        float IntervalSeconds) {
  return IntervalSeconds > 0.0f &&
         CurrentSeconds - LastSeconds >= IntervalSeconds;
}

bool ShouldRunRuntimeBudgetScreenshot(
    double CurrentSeconds, double LastSeconds, float IntervalSeconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return IntervalSeconds > Settings.BudgetScreenshotDisabledIntervalSeconds &&
         ShouldRunRuntimeBudgetWallInterval(CurrentSeconds, LastSeconds,
                                            IntervalSeconds);
}

void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index) {
  const FString Directory = RuntimeBudgetScreenshotDirectory(Settings);
  IFileManager::Get().MakeDirectory(
      *Directory, Settings.bBudgetScreenshotCreateDirectoryTree);
  const FString Path = RuntimeBudgetScreenshotPath(Settings, Index);
  FScreenshotRequest::RequestScreenshot(
      Path, Settings.bBudgetScreenshotShowUI,
      Settings.bBudgetScreenshotAddFilenameSuffix);
  UE_LOG(LogForbocRendering, Display,
         TEXT("runtime-budget screenshot requested path=%s"), *Path);
}

UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return LoadObject<UMaterialInterface>(nullptr, *Path);
}

void ApplyTexture(const FLevelRetroTextureApply &Request) {
  check(Request.Part);
  check(Request.BaseMaterial);

  const FLevelRetroTextureSpec Spec = RenderingReducers::ReduceTextureSpec(
      {Request.Texture, Request.TextureCatalog});
  UTexture2D *RetroTexture = TextureFor(Spec, Request.RuntimeSettings);
  check(RetroTexture);

  UMaterialInstanceDynamic *Material =
      UMaterialInstanceDynamic::Create(Request.BaseMaterial, Request.Part);
  check(Material);

  Material->SetTextureParameterValue(
      FName(*Request.RuntimeSettings.MaterialTextureParameter), RetroTexture);
  Request.Part->SetMaterial(0, Material);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
