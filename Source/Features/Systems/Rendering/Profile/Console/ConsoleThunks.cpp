#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Core/fp.hpp"
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

/** User Story: As a rendering profile console consumer, I need to invoke find rendering declaration index through a stable signature so the rendering profile console workflow remains explicit and composable. @fn template <typename Declaration> func::Maybe<int32> FindRenderingDeclarationIndex( const FString &Name, const TArray<Declaration> &Declarations) */
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

  /** User Story: As a rendering profile console consumer, I need to invoke tprofile field declaration through a stable signature so the rendering profile console workflow remains explicit and composable. @fn TProfileFieldDeclaration(const FString &InName, Value FLevelRetroRenderProfile::*InMember) */
  TProfileFieldDeclaration(const FString &InName,
                           Value FLevelRetroRenderProfile::*InMember)
      : Name(InName), Member(InMember) {}
};

template <typename Apply> struct TRenderingDispatchDeclaration {
  FString Name;
  Apply Run;

  /** User Story: As a rendering profile console consumer, I need to invoke trendering dispatch declaration through a stable signature so the rendering profile console workflow remains explicit and composable. @fn TRenderingDispatchDeclaration(const FString &InName, Apply InRun) */
  TRenderingDispatchDeclaration(const FString &InName, Apply InRun)
      : Name(InName), Run(InRun) {}
};

using FCVarApply = void (*)(const FRetroCVarEval &);
using FCVarApplyDeclaration = TRenderingDispatchDeclaration<FCVarApply>;

/** User Story: As a rendering profile console consumer, I need to invoke apply fixed int cvar through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyFixedIntCVar(const FRetroCVarEval &Eval) */
void ApplyFixedIntCVar(const FRetroCVarEval &Eval);
/** User Story: As a rendering profile console consumer, I need to invoke apply profile int cvar through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyProfileIntCVar(const FRetroCVarEval &Eval) */
void ApplyProfileIntCVar(const FRetroCVarEval &Eval);
/** User Story: As a rendering profile console consumer, I need to invoke apply fixed float cvar through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyFixedFloatCVar(const FRetroCVarEval &Eval) */
void ApplyFixedFloatCVar(const FRetroCVarEval &Eval);
/** User Story: As a rendering profile console consumer, I need to invoke apply profile float cvar through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyProfileFloatCVar(const FRetroCVarEval &Eval) */
void ApplyProfileFloatCVar(const FRetroCVarEval &Eval);

template <typename Value> struct TProfileFieldRegistry;

template <> struct TProfileFieldRegistry<int32> {
  /** User Story: As a rendering profile console consumer, I need to invoke fields through a stable signature so the rendering profile console workflow remains explicit and composable. @fn static TArray<TProfileFieldDeclaration<int32>> Fields( const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) */
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
  /** User Story: As a rendering profile console consumer, I need to invoke fields through a stable signature so the rendering profile console workflow remains explicit and composable. @fn static TArray<TProfileFieldDeclaration<float>> Fields( const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) */
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

/** User Story: As a rendering profile console consumer, I need to invoke cvar apply declarations through a stable signature so the rendering profile console workflow remains explicit and composable. @fn TArray<FCVarApplyDeclaration> CVarApplyDeclarations( const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) */
TArray<FCVarApplyDeclaration> CVarApplyDeclarations(
    const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) {
  return {{Settings.Value.FixedInt, ApplyFixedIntCVar},
          {Settings.Value.ProfileInt, ApplyProfileIntCVar},
          {Settings.Value.FixedFloat, ApplyFixedFloatCVar},
          {Settings.Value.ProfileFloat, ApplyProfileFloatCVar}};
}

/** User Story: As a rendering profile console consumer, I need to invoke required cvar declaration through a stable signature so the rendering profile console workflow remains explicit and composable. @fn FCVarApplyDeclaration RequiredCVarDeclaration( const FString &Name, const ForbocAI::Game::Data::FRenderingConsoleSettings &Settings) */
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

/** User Story: As a rendering profile console consumer, I need to invoke select profile value through a stable signature so the rendering profile console workflow remains explicit and composable. @fn template <typename Value> func::Maybe<Value> SelectProfileValue(const TProfileValueSelectRequest<Value> &Request) */
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

/** User Story: As a rendering profile console consumer, I need to invoke required profile value through a stable signature so the rendering profile console workflow remains explicit and composable. @fn template <typename Value> Value RequiredProfileValue(const FRetroCVarEval &Eval) */
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

/** User Story: As a rendering profile console consumer, I need to invoke required profile int through a stable signature so the rendering profile console workflow remains explicit and composable. @fn int32 RequiredProfileInt(const FRetroCVarEval &Eval) */
int32 RequiredProfileInt(const FRetroCVarEval &Eval) {
  return RequiredProfileValue<int32>(Eval);
}

/** User Story: As a rendering profile console consumer, I need to invoke required profile float through a stable signature so the rendering profile console workflow remains explicit and composable. @fn float RequiredProfileFloat(const FRetroCVarEval &Eval) */
float RequiredProfileFloat(const FRetroCVarEval &Eval) {
  return RequiredProfileValue<float>(Eval);
}

/** User Story: As a rendering profile console consumer, I need to invoke apply fixed int cvar through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyFixedIntCVar(const FRetroCVarEval &Eval) */
void ApplyFixedIntCVar(const FRetroCVarEval &Eval) {
  SetRuntimeCVarInt(Eval.Setting.Name, Eval.Setting.IntValue);
}

/** User Story: As a rendering profile console consumer, I need to invoke apply profile int cvar through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyProfileIntCVar(const FRetroCVarEval &Eval) */
void ApplyProfileIntCVar(const FRetroCVarEval &Eval) {
  SetRuntimeCVarInt(Eval.Setting.Name, RequiredProfileInt(Eval));
}

/** User Story: As a rendering profile console consumer, I need to invoke apply fixed float cvar through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyFixedFloatCVar(const FRetroCVarEval &Eval) */
void ApplyFixedFloatCVar(const FRetroCVarEval &Eval) {
  SetRuntimeCVarFloat(Eval.Setting.Name, Eval.Setting.FloatValue);
}

/** User Story: As a rendering profile console consumer, I need to invoke apply profile float cvar through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyProfileFloatCVar(const FRetroCVarEval &Eval) */
void ApplyProfileFloatCVar(const FRetroCVarEval &Eval) {
  SetRuntimeCVarFloat(Eval.Setting.Name, RequiredProfileFloat(Eval));
}

/** User Story: As a rendering profile console consumer, I need to invoke apply console variable through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyConsoleVariable(const FRetroCVarEval &Eval) */
void ApplyConsoleVariable(const FRetroCVarEval &Eval) {
  check(Eval.Settings);
  RequiredCVarDeclaration(Eval.Setting.ValueKind, Eval.Settings->Console)
      .Run(Eval);
}

} // namespace

/** User Story: As a rendering profile console consumer, I need to invoke apply runtime console variables through a stable signature so the rendering profile console workflow remains explicit and composable. @fn void ApplyRuntimeConsoleVariables( const FLevelRetroRenderProfile &Profile, const ForbocAI::Game::Data::FRenderingSettings &Settings) */
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
