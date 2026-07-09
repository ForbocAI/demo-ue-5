#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Core/ue_fp.hpp"
#include "Features/Systems/Rendering/Profile/ProfileThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

// Runtime console variable subdomain: routes each authored console-variable
// setting to a value source (fixed literal or profile field) and writes it
// through the shared runtime console-variable primitive.

struct FRetroCVarEval {
  ForbocAI::Game::Data::FRenderingConsoleVariableSettings Setting;
  FLevelRetroRenderProfile Profile;
  const ForbocAI::Game::Data::FRenderingSettings *Settings;
};

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

  TProfileFieldDeclaration(const FString &InName,
                           Value FLevelRetroRenderProfile::*InMember)
      : Name(InName), Member(InMember) {}
};

template <typename Apply> struct TRenderingDispatchDeclaration {
  FString Name;
  Apply Run;

  TRenderingDispatchDeclaration(const FString &InName, Apply InRun)
      : Name(InName), Run(InRun) {}
};

using FCVarApply = void (*)(const FRetroCVarEval &);
using FCVarApplyDeclaration = TRenderingDispatchDeclaration<FCVarApply>;

void ApplyFixedIntCVar(const FRetroCVarEval &Eval);
void ApplyProfileIntCVar(const FRetroCVarEval &Eval);
void ApplyFixedFloatCVar(const FRetroCVarEval &Eval);
void ApplyProfileFloatCVar(const FRetroCVarEval &Eval);

template <typename Value> struct TProfileFieldRegistry;

template <> struct TProfileFieldRegistry<int32> {
  static TArray<TProfileFieldDeclaration<int32>> Fields(
      const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) {
    return {{Settings.IntegerProfile.AntiAliasingMethod,
             &FLevelRetroRenderProfile::AntiAliasingMethod},
            {Settings.IntegerProfile.PostProcessAaQuality,
             &FLevelRetroRenderProfile::PostProcessAAQuality},
            {Settings.IntegerProfile.ShadowCascades,
             &FLevelRetroRenderProfile::ShadowCascades},
            {Settings.IntegerProfile.ShadowMaxResolution,
             &FLevelRetroRenderProfile::ShadowMaxResolution}};
  }
};

template <> struct TProfileFieldRegistry<float> {
  static TArray<TProfileFieldDeclaration<float>> Fields(
      const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) {
    return {{Settings.FloatProfile.ScreenPercentage,
             &FLevelRetroRenderProfile::ScreenPercentage},
            {Settings.FloatProfile.ViewDistanceScale,
             &FLevelRetroRenderProfile::ViewDistanceScale},
            {Settings.FloatProfile.FoliageDensityScale,
             &FLevelRetroRenderProfile::FoliageDensityScale},
            {Settings.FloatProfile.GrassDensityScale,
             &FLevelRetroRenderProfile::GrassDensityScale}};
  }
};

TArray<FCVarApplyDeclaration> CVarApplyDeclarations(
    const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) {
  return {{Settings.Value.FixedInt, ApplyFixedIntCVar},
          {Settings.Value.ProfileInt, ApplyProfileIntCVar},
          {Settings.Value.FixedFloat, ApplyFixedFloatCVar},
          {Settings.Value.ProfileFloat, ApplyProfileFloatCVar}};
}

FCVarApplyDeclaration RequiredCVarDeclaration(
    const FString &Name,
    const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) {
  const TArray<FCVarApplyDeclaration> Declarations =
      CVarApplyDeclarations(Settings);
  const func::Maybe<int32> Found =
      FindRenderingDeclarationIndex(Name, Declarations);
  check(Found.hasValue);
  return Declarations[Found.value];
}

template <typename Value>
struct TProfileValueSelectRequest {
  const FLevelRetroRenderProfile *Profile;
  FString Field;
  const ForbocAI::Game::Data::FRenderingConsoleSettings *Settings;
};

template <typename Value>
func::Maybe<Value>
SelectProfileValue(const TProfileValueSelectRequest<Value> &Request) {
  const TArray<TProfileFieldDeclaration<Value>> FieldList =
      TProfileFieldRegistry<Value>::Fields(*Request.Settings);
  return func::match(
      FindRenderingDeclarationIndex(Request.Field, FieldList),
      [&Request, &FieldList](int32 Index) {
        return func::just<Value>(Request.Profile->*FieldList[Index].Member);
      },
      []() { return func::nothing<Value>(); });
}

template <typename Value>
Value RequiredProfileValue(const FRetroCVarEval &Eval) {
  check(Eval.Settings);
  const func::Maybe<Value> ValueResult =
      SelectProfileValue<Value>(
          {&Eval.Profile, Eval.Setting.ProfileField,
           &Eval.Settings->Console});
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
  SetRuntimeCVarInt(Eval.Setting.Name, Eval.Setting.IntValue);
}

void ApplyProfileIntCVar(const FRetroCVarEval &Eval) {
  SetRuntimeCVarInt(Eval.Setting.Name, RequiredProfileInt(Eval));
}

void ApplyFixedFloatCVar(const FRetroCVarEval &Eval) {
  SetRuntimeCVarFloat(Eval.Setting.Name, Eval.Setting.FloatValue);
}

void ApplyProfileFloatCVar(const FRetroCVarEval &Eval) {
  SetRuntimeCVarFloat(Eval.Setting.Name, RequiredProfileFloat(Eval));
}

void ApplyConsoleVariable(const FRetroCVarEval &Eval) {
  check(Eval.Settings);
  RequiredCVarDeclaration(Eval.Setting.ValueKind, Eval.Settings->Console)
      .Run(Eval);
}

} // namespace

void ApplyRuntimeConsoleVariables(
    const FLevelRetroRenderProfile &Profile,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  func::for_each_indexed(
      Settings.ConsoleVariables,
      static_cast<size_t>(Settings.ConsoleVariables.Num()),
      [&Profile, &Settings](
          const ForbocAI::Game::Data::FRenderingConsoleVariableSettings
              &Setting) {
        ApplyConsoleVariable({Setting, Profile, &Settings});
      });
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
