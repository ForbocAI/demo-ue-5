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

func::Maybe<int32> SelectProfileInt(const FLevelRetroRenderProfile &Profile,
                                    const FString &Field) {
  return func::multi_match<FString, int32>(
      Field,
      {
          func::when<FString, int32>(
              func::equals<FString>(TEXT("anti_aliasing_method")),
              [&Profile](const FString &) {
                return Profile.AntiAliasingMethod;
              }),
          func::when<FString, int32>(
              func::equals<FString>(TEXT("post_process_aa_quality")),
              [&Profile](const FString &) {
                return Profile.PostProcessAAQuality;
              }),
          func::when<FString, int32>(
              func::equals<FString>(TEXT("shadow_cascades")),
              [&Profile](const FString &) { return Profile.ShadowCascades; }),
          func::when<FString, int32>(
              func::equals<FString>(TEXT("shadow_max_resolution")),
              [&Profile](const FString &) {
                return Profile.ShadowMaxResolution;
              }),
      });
}

func::Maybe<float> SelectProfileFloat(const FLevelRetroRenderProfile &Profile,
                                      const FString &Field) {
  return func::multi_match<FString, float>(
      Field,
      {
          func::when<FString, float>(
              func::equals<FString>(TEXT("screen_percentage")),
              [&Profile](const FString &) { return Profile.ScreenPercentage; }),
          func::when<FString, float>(
              func::equals<FString>(TEXT("view_distance_scale")),
              [&Profile](const FString &) {
                return Profile.ViewDistanceScale;
              }),
          func::when<FString, float>(
              func::equals<FString>(TEXT("foliage_density_scale")),
              [&Profile](const FString &) {
                return Profile.FoliageDensityScale;
              }),
          func::when<FString, float>(
              func::equals<FString>(TEXT("grass_density_scale")),
              [&Profile](const FString &) {
                return Profile.GrassDensityScale;
              }),
      });
}

int32 RequiredProfileInt(const FRetroCVarEval &Eval) {
  const func::Maybe<int32> Value =
      SelectProfileInt(Eval.Profile, Eval.Setting.ProfileField);
  check(Value.hasValue);
  return Value.value;
}

float RequiredProfileFloat(const FRetroCVarEval &Eval) {
  const func::Maybe<float> Value =
      SelectProfileFloat(Eval.Profile, Eval.Setting.ProfileField);
  check(Value.hasValue);
  return Value.value;
}

void ApplyConsoleVariable(const FRetroCVarEval &Eval) {
  const func::Maybe<bool> Applied = func::multi_match<FString, bool>(
      Eval.Setting.ValueKind,
      {
          func::when<FString, bool>(
              func::equals<FString>(TEXT("fixed_int")),
              [&Eval](const FString &) {
                SetCVarInt(Eval.Setting.Name, Eval.Setting.IntValue);
                return true;
              }),
          func::when<FString, bool>(
              func::equals<FString>(TEXT("profile_int")),
              [&Eval](const FString &) {
                SetCVarInt(Eval.Setting.Name, RequiredProfileInt(Eval));
                return true;
              }),
          func::when<FString, bool>(
              func::equals<FString>(TEXT("fixed_float")),
              [&Eval](const FString &) {
                SetCVarFloat(Eval.Setting.Name, Eval.Setting.FloatValue);
                return true;
              }),
          func::when<FString, bool>(
              func::equals<FString>(TEXT("profile_float")),
              [&Eval](const FString &) {
                SetCVarFloat(Eval.Setting.Name, RequiredProfileFloat(Eval));
                return true;
              }),
      });
  check(Applied.hasValue);
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

bool PredicateMatches(const FRetroPredicateEval &Eval) {
  const int32 Term = PredicateTerm(Eval);
  const func::Maybe<bool> Matched = func::multi_match<FString, bool>(
      Eval.Predicate.Kind,
      {
          func::when<FString, bool>(
              func::equals<FString>(TEXT("always")),
              [](const FString &) { return true; }),
          func::when<FString, bool>(
              func::equals<FString>(TEXT("mod_equals")),
              [&Eval, Term](const FString &) {
                check(Eval.Predicate.Modulus > 0);
                return Term % Eval.Predicate.Modulus ==
                       Eval.Predicate.Equals;
              }),
          func::when<FString, bool>(
              func::equals<FString>(TEXT("mod_less_than")),
              [&Eval, Term](const FString &) {
                check(Eval.Predicate.Modulus > 0);
                return Term % Eval.Predicate.Modulus <
                       Eval.Predicate.LessThan;
              }),
      });
  check(Matched.hasValue);
  return Matched.value;
}

FColor ResolveColor(const FRetroResultEval &Eval) {
  const func::Maybe<FColor> Resolved = func::multi_match<FString, FColor>(
      Eval.Result.Kind,
      {
          func::when<FString, FColor>(
              func::equals<FString>(TEXT("solid")),
              [&Eval](const FString &) {
                return Color(Eval.Result.Color, Eval.Alpha);
              }),
          func::when<FString, FColor>(
              func::equals<FString>(TEXT("mix")),
              [&Eval](const FString &) { return Mix(Eval); }),
      });
  check(Resolved.hasValue);
  return Resolved.value;
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
