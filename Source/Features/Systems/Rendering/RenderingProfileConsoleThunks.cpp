#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Core/ue_fp.hpp"
#include "Features/Systems/Rendering/RenderingProfileRuntime.h"

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

template <typename Value>
func::Maybe<Value> SelectProfileValue(const FLevelRetroRenderProfile &Profile,
                                      const FString &Field) {
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
  RequiredCVarDeclaration(Eval.Setting.ValueKind).Run(Eval);
}

} // namespace

void ApplyRuntimeConsoleVariables(
    const FLevelRetroRenderProfile &Profile,
    const ForbocAI::Game::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  func::for_each_indexed(
      RuntimeSettings.ConsoleVariables,
      static_cast<size_t>(RuntimeSettings.ConsoleVariables.Num()),
      [&Profile](const ForbocAI::Game::Data::FRenderingConsoleVariableSettings
                     &Setting) { ApplyConsoleVariable({Setting, Profile}); });
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
