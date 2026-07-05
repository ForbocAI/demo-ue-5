#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/StaticMeshComponent.h"
#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Engine/Texture2D.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "HAL/IConsoleManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingThunks {
namespace {

struct FRetroTextureCell {
  ELevelRetroTexture Texture;
  int32 X;
  int32 Y;
  const ForbocAI::Demo::Data::FRenderingRuntimeSettings *RuntimeSettings;
};

struct FRetroHashCell {
  int32 X;
  int32 Y;
  int32 Salt;
  const ForbocAI::Demo::Data::FRenderingTextureHashSettings *Hash;
};

struct FRetroPredicateEval {
  ForbocAI::Demo::Data::FRenderingTexturePredicateSettings Predicate;
  int32 X;
  int32 Y;
  int32 Noise;
};

struct FRetroResultEval {
  ForbocAI::Demo::Data::FRenderingTextureColorResultSettings Result;
  int32 Noise;
  int32 Alpha;
};

struct FRetroCVarEval {
  ForbocAI::Demo::Data::FRenderingConsoleVariableSettings Setting;
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

void SetCVarFloat(const FString &Name, float Value) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(*Name);
  check(Found);
  Found->Set(Value, ECVF_SetByGameSetting);
}

void SetCVarInt(const FString &Name, int32 Value) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(*Name);
  check(Found);
  Found->Set(Value, ECVF_SetByGameSetting);
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

FColor Color(const ForbocAI::Demo::Data::FRenderingRgbSettings &Rgb,
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

ForbocAI::Demo::Data::FRenderingTexturePaletteSettings
RequiredPalette(const FRetroTextureCell &Cell) {
  check(Cell.RuntimeSettings);
  const func::Maybe<ForbocAI::Demo::Data::FRenderingTexturePaletteSettings>
      Palette = func::find_indexed(
          Cell.RuntimeSettings->TexturePalettes,
          static_cast<size_t>(Cell.RuntimeSettings->TexturePalettes.Num()),
          [&Cell](
              const ForbocAI::Demo::Data::FRenderingTexturePaletteSettings
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
  const ForbocAI::Demo::Data::FRenderingTexturePaletteSettings Palette =
      RequiredPalette(Cell);
  const func::Maybe<ForbocAI::Demo::Data::FRenderingTextureRuleSettings> Rule =
      func::find_indexed(
          Palette.Rules, static_cast<size_t>(Palette.Rules.Num()),
          [&Cell, Noise](
              const ForbocAI::Demo::Data::FRenderingTextureRuleSettings
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
    const ForbocAI::Demo::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  return frmt::RuntimeString(
      RuntimeSettings.TextureCacheKeyFormat,
      frmt::Args(
          {frmt::Arg(static_cast<int32>(Spec.Texture)),
           frmt::Arg(Spec.Size.X),
           frmt::Arg(Spec.Size.Y)}));
}

UTexture2D *CreateRetroTexture(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Demo::Data::FRenderingRuntimeSettings &RuntimeSettings) {
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
    const ForbocAI::Demo::Data::FRenderingRuntimeSettings &RuntimeSettings) {
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
    const FLevelRetroRenderProfile &Profile,
    const ForbocAI::Demo::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  func::for_each_indexed(
      RuntimeSettings.ConsoleVariables,
      static_cast<size_t>(RuntimeSettings.ConsoleVariables.Num()),
      [&Profile](
          const ForbocAI::Demo::Data::FRenderingConsoleVariableSettings
              &Setting) { ApplyConsoleVariable({Setting, Profile}); });
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
} // namespace Demo
} // namespace ForbocAI
