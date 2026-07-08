#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/StaticMeshComponent.h"
#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Engine/Texture2D.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

struct FRetroTextureCell {
  ELevelRetroTexture Texture;
  int32 X;
  int32 Y;
  const ForbocAI::Game::Data::FRenderingSettings *Settings;
};

struct FRetroHashCell {
  int32 X;
  int32 Y;
  int32 Salt;
  const ForbocAI::Game::Data::FHashSettings *Hash;
};

struct FRetroPredicateEval {
  ForbocAI::Game::Data::FPredicateSettings Predicate;
  int32 X;
  int32 Y;
  int32 Noise;
  const ForbocAI::Game::Data::FRenderingSettings *Settings;
};

struct FRetroResultEval {
  ForbocAI::Game::Data::FColorResultSettings Result;
  int32 Noise;
  int32 Alpha;
  const ForbocAI::Game::Data::FRenderingSettings *Settings;
};

template <typename Declaration>
func::Maybe<int32> FindTextureRenderingDeclarationIndex(
    const FString &Name, const TArray<Declaration> &Declarations) {
  const TArray<int32> Indices = func::index_range(Declarations.Num());
  return func::find_indexed(
      Indices, static_cast<size_t>(Indices.Num()),
      [&Name, &Declarations](int32 Index) {
        return Declarations[Index].Name == Name;
      });
}

template <typename Apply> struct TTextureRenderingDispatchDeclaration {
  FString Name;
  Apply Run;

  TTextureRenderingDispatchDeclaration(const FString &InName, Apply InRun)
      : Name(InName), Run(InRun) {}
};

using FPredicateApply = bool (*)(const FRetroPredicateEval &, int32);
using FColorResultApply = FColor (*)(const FRetroResultEval &);
using FPredicateApplyDeclaration =
    TTextureRenderingDispatchDeclaration<FPredicateApply>;
using FColorResultDeclaration =
    TTextureRenderingDispatchDeclaration<FColorResultApply>;

bool AlwaysPredicate(const FRetroPredicateEval &Eval, int32 Term);
bool ModEqualsPredicate(const FRetroPredicateEval &Eval, int32 Term);
bool ModLessThanPredicate(const FRetroPredicateEval &Eval, int32 Term);
FColor SolidColor(const FRetroResultEval &Eval);
FColor Mix(const FRetroResultEval &Eval);

TArray<FPredicateApplyDeclaration> PredicateApplyDeclarations(
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  return {{Settings.PredicateAlwaysKind, AlwaysPredicate},
          {Settings.PredicateModEqualsKind, ModEqualsPredicate},
          {Settings.PredicateModLessThanKind, ModLessThanPredicate}};
}

TArray<FColorResultDeclaration> ColorResultDeclarations(
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  return {{Settings.ResultSolidKind, SolidColor},
          {Settings.ResultMixKind, Mix}};
}

FPredicateApplyDeclaration RequiredPredicateDeclaration(
    const FString &Name,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  const TArray<FPredicateApplyDeclaration> Declarations =
      PredicateApplyDeclarations(Settings);
  const func::Maybe<int32> Found =
      FindTextureRenderingDeclarationIndex(Name, Declarations);
  check(Found.hasValue);
  return Declarations[Found.value];
}

FColorResultDeclaration RequiredColorDeclaration(
    const FString &Name,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  const TArray<FColorResultDeclaration> Declarations =
      ColorResultDeclarations(Settings);
  const func::Maybe<int32> Found =
      FindTextureRenderingDeclarationIndex(Name, Declarations);
  check(Found.hasValue);
  return Declarations[Found.value];
}

FColor Color(const ForbocAI::Game::Data::FRenderingRgbSettings &Rgb,
             int32 Alpha) {
  return FColor(static_cast<uint8>(Rgb.R), static_cast<uint8>(Rgb.G),
                static_cast<uint8>(Rgb.B), static_cast<uint8>(Alpha));
}

FColor Mix(const FRetroResultEval &Eval) {
  check(Eval.Result.Denominator > int32{});
  check(Eval.Result.NumeratorNoiseModulus > int32{});
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
  check(Eval.Predicate.XDivisor > int32{});
  check(Eval.Predicate.YDivisor > int32{});
  return Eval.Predicate.XMultiplier * (Eval.X / Eval.Predicate.XDivisor) +
         Eval.Predicate.YMultiplier * (Eval.Y / Eval.Predicate.YDivisor) +
         Eval.Predicate.NoiseMultiplier * Eval.Noise;
}

bool AlwaysPredicate(const FRetroPredicateEval &, int32) { return true; }

bool ModEqualsPredicate(const FRetroPredicateEval &Eval, int32 Term) {
  check(Eval.Predicate.Modulus > int32{});
  return Term % Eval.Predicate.Modulus == Eval.Predicate.Equals;
}

bool ModLessThanPredicate(const FRetroPredicateEval &Eval, int32 Term) {
  check(Eval.Predicate.Modulus > int32{});
  return Term % Eval.Predicate.Modulus < Eval.Predicate.LessThan;
}

bool PredicateMatches(const FRetroPredicateEval &Eval) {
  check(Eval.Settings);
  const int32 Term = PredicateTerm(Eval);
  return RequiredPredicateDeclaration(Eval.Predicate.Kind, *Eval.Settings)
      .Run(Eval, Term);
}

FColor ResolveColor(const FRetroResultEval &Eval) {
  check(Eval.Settings);
  return RequiredColorDeclaration(Eval.Result.Kind, *Eval.Settings).Run(Eval);
}

ForbocAI::Game::Data::FPaletteSettings
RequiredPalette(const FRetroTextureCell &Cell) {
  check(Cell.Settings);
  const TArray<int32> Indices =
      func::index_range(Cell.Settings->TexturePalettes.Num());
  const func::Maybe<int32> PaletteIndex = func::find_indexed(
      Indices, static_cast<size_t>(Indices.Num()), [&Cell](int32 Index) {
        return static_cast<ELevelRetroTexture>(Index) == Cell.Texture;
      });
  check(PaletteIndex.hasValue);
  return Cell.Settings->TexturePalettes[PaletteIndex.value];
}

FColor PaletteColor(const FRetroTextureCell &Cell) {
  check(Cell.Settings);
  const int32 Noise =
      HashCell({Cell.X, Cell.Y, static_cast<int32>(Cell.Texture),
                &Cell.Settings->TextureHash});
  const ForbocAI::Game::Data::FPaletteSettings Palette =
      RequiredPalette(Cell);
  const func::Maybe<ForbocAI::Game::Data::FRuleSettings> Rule =
      func::find_indexed(
          Palette.Rules, static_cast<size_t>(Palette.Rules.Num()),
          [&Cell, Noise](
              const ForbocAI::Game::Data::FRuleSettings
                  &Candidate) {
            return PredicateMatches(
                {Candidate.Predicate, Cell.X, Cell.Y, Noise, Cell.Settings});
          });
  check(Rule.hasValue);
  return ResolveColor({Rule.value.Result, Noise,
                       Cell.Settings->TextureAlpha, Cell.Settings});
}

FString TextureCacheKey(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  return frmt::RuntimeString(
      Settings.TextureCacheKeyFormat,
      frmt::Args({frmt::Arg(static_cast<int32>(Spec.Texture)),
                  frmt::Arg(Spec.Size.X), frmt::Arg(Spec.Size.Y)}));
}

UTexture2D *CreateRetroTexture(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  check(Spec.Size.X > int32{});
  check(Spec.Size.Y > int32{});

  UTexture2D *Result =
      UTexture2D::CreateTransient(Spec.Size.X, Spec.Size.Y, PF_B8G8R8A8);
  check(Result);
  check(Result->GetPlatformData());
  check(Settings.TextureMipIndex >= int32{});
  check(Result->GetPlatformData()->Mips.Num() > Settings.TextureMipIndex);

  Result->Filter = TF_Nearest;
  Result->AddressX = TA_Wrap;
  Result->AddressY = TA_Wrap;
  Result->MipGenSettings = TMGS_NoMipmaps;
  Result->NeverStream = true;
  Result->SRGB = true;

  FTexture2DMipMap &Mip =
      Result->GetPlatformData()->Mips[Settings.TextureMipIndex];
  void *Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
  check(Data);

  const std::vector<FColor> Pixels = func::map_grid<FColor>(
      static_cast<size_t>(Spec.Size.Y), static_cast<size_t>(Spec.Size.X),
      [&Spec, &Settings](const func::GridIndex &Index) {
        return PaletteColor({Spec.Texture, static_cast<int32>(Index.Column),
                             static_cast<int32>(Index.Row),
                             &Settings});
      });
  FMemory::Memcpy(Data, Pixels.data(),
                  Pixels.size() * Settings.TextureChannels *
                      sizeof(uint8));
  Mip.BulkData.Unlock();

  Result->UpdateResource();
  Result->AddToRoot();
  return Result;
}

UTexture2D *TextureFor(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  static TMap<FString, UTexture2D *> TextureCache;
  const FString Key = TextureCacheKey(Spec, Settings);
  UTexture2D **Cached = TextureCache.Find(Key);
  return Cached ? *Cached
                : [&]() {
                    UTexture2D *Created =
                        CreateRetroTexture(Spec, Settings);
                    TextureCache.Add(Key, Created);
                    return Created;
                  }();
}

} // namespace

UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return LoadObject<UMaterialInterface>(nullptr, *Path);
}

void ApplyTexture(const FLevelRetroTextureApply &Request) {
  check(Request.Part);
  check(Request.BaseMaterial);

  const FLevelRetroTextureSpec Spec = RenderingTextureReducers::ReduceTextureSpec(
      {Request.Texture, Request.TextureCatalog});
  UTexture2D *RetroTexture = TextureFor(Spec, Request.Settings);
  check(RetroTexture);

  UMaterialInstanceDynamic *Material =
      UMaterialInstanceDynamic::Create(Request.BaseMaterial, Request.Part);
  check(Material);
  check(Request.Settings.TextureMaterialSlotIndex >= int32{});

  Material->SetTextureParameterValue(
      FName(*Request.Settings.MaterialTextureParameter), RetroTexture);
  Request.Part->SetMaterial(Request.Settings.TextureMaterialSlotIndex, Material);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
