#pragma once

#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/StaticMeshComponent.h"
#include "Core/frmt.hpp"
#include "Core/fp.hpp"
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

/** User Story: As a rendering texture palette consumer, I need to invoke find texture rendering declaration index through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn template <typename Declaration> func::Maybe<int32> FindTextureRenderingDeclarationIndex( const FString &Name, const TArray<Declaration> &Declarations) */
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

  /** User Story: As a rendering texture palette consumer, I need to invoke ttexture rendering dispatch declaration through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn TTextureRenderingDispatchDeclaration(const FString &InName, Apply InRun) */
  TTextureRenderingDispatchDeclaration(const FString &InName, Apply InRun)
      : Name(InName), Run(InRun) {}
};

using FPredicateApply = bool (*)(const FRetroPredicateEval &, int32);
using FColorResultApply = FColor (*)(const FRetroResultEval &);
using FPredicateApplyDeclaration =
    TTextureRenderingDispatchDeclaration<FPredicateApply>;
using FColorResultDeclaration =
    TTextureRenderingDispatchDeclaration<FColorResultApply>;

/** User Story: As a rendering texture palette consumer, I need to invoke always predicate through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn bool AlwaysPredicate(const FRetroPredicateEval &Eval, int32 Term) */
bool AlwaysPredicate(const FRetroPredicateEval &Eval, int32 Term);
/** User Story: As a rendering texture palette consumer, I need to invoke mod equals predicate through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn bool ModEqualsPredicate(const FRetroPredicateEval &Eval, int32 Term) */
bool ModEqualsPredicate(const FRetroPredicateEval &Eval, int32 Term);
/** User Story: As a rendering texture palette consumer, I need to invoke mod less than predicate through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn bool ModLessThanPredicate(const FRetroPredicateEval &Eval, int32 Term) */
bool ModLessThanPredicate(const FRetroPredicateEval &Eval, int32 Term);
/** User Story: As a rendering texture palette consumer, I need to invoke solid color through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn FColor SolidColor(const FRetroResultEval &Eval) */
FColor SolidColor(const FRetroResultEval &Eval);
/** User Story: As a rendering texture palette consumer, I need to invoke mix through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn FColor Mix(const FRetroResultEval &Eval) */
FColor Mix(const FRetroResultEval &Eval);

/** User Story: As a rendering texture palette consumer, I need to invoke predicate apply declarations through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn TArray<FPredicateApplyDeclaration> PredicateApplyDeclarations( const ForbocAI::Game::Data::FRenderingSettings &Settings) */
TArray<FPredicateApplyDeclaration> PredicateApplyDeclarations(
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  return {{Settings.RuleNames.PredicateAlways, AlwaysPredicate},
          {Settings.RuleNames.PredicateModEquals, ModEqualsPredicate},
          {Settings.RuleNames.PredicateModLessThan, ModLessThanPredicate}};
}

/** User Story: As a rendering texture palette consumer, I need to invoke color result declarations through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn TArray<FColorResultDeclaration> ColorResultDeclarations( const ForbocAI::Game::Data::FRenderingSettings &Settings) */
TArray<FColorResultDeclaration> ColorResultDeclarations(
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  return {{Settings.RuleNames.ResultSolid, SolidColor},
          {Settings.RuleNames.ResultMix, Mix}};
}

/** User Story: As a rendering texture palette consumer, I need to invoke required predicate declaration through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn FPredicateApplyDeclaration RequiredPredicateDeclaration( const FString &Name, const ForbocAI::Game::Data::FRenderingSettings &Settings) */
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

/** User Story: As a rendering texture palette consumer, I need to invoke required color declaration through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn FColorResultDeclaration RequiredColorDeclaration( const FString &Name, const ForbocAI::Game::Data::FRenderingSettings &Settings) */
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

/** User Story: As a rendering texture palette consumer, I need to invoke color through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn FColor Color(const ForbocAI::Game::Data::FRenderingRgbSettings &Rgb, int32 Alpha) */
FColor Color(const ForbocAI::Game::Data::FRenderingRgbSettings &Rgb,
             int32 Alpha) {
  return FColor(static_cast<uint8>(Rgb.R), static_cast<uint8>(Rgb.G),
                static_cast<uint8>(Rgb.B), static_cast<uint8>(Alpha));
}

/** User Story: As a rendering texture palette consumer, I need to invoke mix through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn FColor Mix(const FRetroResultEval &Eval) */
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

/** User Story: As a rendering texture palette consumer, I need to invoke solid color through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn FColor SolidColor(const FRetroResultEval &Eval) */
FColor SolidColor(const FRetroResultEval &Eval) {
  return Color(Eval.Result.Color, Eval.Alpha);
}

/** User Story: As a rendering texture palette consumer, I need to invoke hash cell through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn int32 HashCell(FRetroHashCell Cell) */
int32 HashCell(FRetroHashCell Cell) {
  check(Cell.Hash);
  int32 Value = Cell.X * Cell.Hash->XMultiplier ^
                Cell.Y * Cell.Hash->YMultiplier ^
                Cell.Salt * Cell.Hash->SaltMultiplier;
  Value ^= Value >> Cell.Hash->XorShift;
  return FMath::Abs(Value);
}

/** User Story: As a rendering texture palette consumer, I need to invoke predicate term through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn int32 PredicateTerm(const FRetroPredicateEval &Eval) */
int32 PredicateTerm(const FRetroPredicateEval &Eval) {
  check(Eval.Predicate.Term.XDivisor > int32{});
  check(Eval.Predicate.Term.YDivisor > int32{});
  return Eval.Predicate.Term.XMultiplier *
             (Eval.X / Eval.Predicate.Term.XDivisor) +
         Eval.Predicate.Term.YMultiplier *
             (Eval.Y / Eval.Predicate.Term.YDivisor) +
         Eval.Predicate.Term.NoiseMultiplier * Eval.Noise;
}

/** User Story: As a rendering texture palette consumer, I need to invoke always predicate through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn bool AlwaysPredicate(const FRetroPredicateEval &, int32) */
bool AlwaysPredicate(const FRetroPredicateEval &, int32) { return true; }

/** User Story: As a rendering texture palette consumer, I need to invoke mod equals predicate through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn bool ModEqualsPredicate(const FRetroPredicateEval &Eval, int32 Term) */
bool ModEqualsPredicate(const FRetroPredicateEval &Eval, int32 Term) {
  check(Eval.Predicate.Comparison.Modulus > int32{});
  return Term % Eval.Predicate.Comparison.Modulus ==
         Eval.Predicate.Comparison.Equals;
}

/** User Story: As a rendering texture palette consumer, I need to invoke mod less than predicate through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn bool ModLessThanPredicate(const FRetroPredicateEval &Eval, int32 Term) */
bool ModLessThanPredicate(const FRetroPredicateEval &Eval, int32 Term) {
  check(Eval.Predicate.Comparison.Modulus > int32{});
  return Term % Eval.Predicate.Comparison.Modulus <
         Eval.Predicate.Comparison.LessThan;
}

/** User Story: As a rendering texture palette consumer, I need to invoke predicate matches through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn bool PredicateMatches(const FRetroPredicateEval &Eval) */
bool PredicateMatches(const FRetroPredicateEval &Eval) {
  check(Eval.Settings);
  const int32 Term = PredicateTerm(Eval);
  return RequiredPredicateDeclaration(Eval.Predicate.Kind, *Eval.Settings)
      .Run(Eval, Term);
}

/** User Story: As a rendering texture palette consumer, I need to invoke resolve color through a stable signature so the rendering texture palette workflow remains explicit and composable. @fn FColor ResolveColor(const FRetroResultEval &Eval) */
FColor ResolveColor(const FRetroResultEval &Eval) {
  check(Eval.Settings);
  return RequiredColorDeclaration(Eval.Result.Kind, *Eval.Settings).Run(Eval);
}

} // namespace
} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
