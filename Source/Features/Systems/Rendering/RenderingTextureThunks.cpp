#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/StaticMeshComponent.h"
#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Engine/Texture2D.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
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

inline FString RenderingTextureDispatchAtom(const char *Atom) {
  return FString(UTF8_TO_TCHAR(Atom));
}

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

  TTextureRenderingDispatchDeclaration(const char *InName, Apply InRun)
      : Name(RenderingTextureDispatchAtom(InName)), Run(InRun) {}
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

const TArray<FPredicateApplyDeclaration> &PredicateApplyDeclarations() {
  static const TArray<FPredicateApplyDeclaration> RegisteredDeclarations = {
      {"always", AlwaysPredicate},
      {"mod_equals", ModEqualsPredicate},
      {"mod_less_than", ModLessThanPredicate}};
  return RegisteredDeclarations;
}

const TArray<FColorResultDeclaration> &ColorResultDeclarations() {
  static const TArray<FColorResultDeclaration> RegisteredDeclarations = {
      {"solid", SolidColor}, {"mix", Mix}};
  return RegisteredDeclarations;
}

FPredicateApplyDeclaration RequiredPredicateDeclaration(const FString &Name) {
  const TArray<FPredicateApplyDeclaration> &Declarations =
      PredicateApplyDeclarations();
  const func::Maybe<int32> Found =
      FindTextureRenderingDeclarationIndex(Name, Declarations);
  check(Found.hasValue);
  return Declarations[Found.value];
}

FColorResultDeclaration RequiredColorDeclaration(const FString &Name) {
  const TArray<FColorResultDeclaration> &Declarations =
      ColorResultDeclarations();
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
  return RequiredPredicateDeclaration(Eval.Predicate.Kind).Run(Eval, Term);
}

FColor ResolveColor(const FRetroResultEval &Eval) {
  return RequiredColorDeclaration(Eval.Result.Kind).Run(Eval);
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
            return RenderingTextureReducers::ReduceTextureKind(Candidate.Texture) ==
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
      frmt::Args({frmt::Arg(static_cast<int32>(Spec.Texture)),
                  frmt::Arg(Spec.Size.X), frmt::Arg(Spec.Size.Y)}));
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

UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return LoadObject<UMaterialInterface>(nullptr, *Path);
}

void ApplyTexture(const FLevelRetroTextureApply &Request) {
  check(Request.Part);
  check(Request.BaseMaterial);

  const FLevelRetroTextureSpec Spec = RenderingTextureReducers::ReduceTextureSpec(
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
