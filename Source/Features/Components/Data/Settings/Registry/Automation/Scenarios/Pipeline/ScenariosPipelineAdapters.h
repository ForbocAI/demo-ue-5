#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Pipeline::FTests, IdleTickAdvancesState,
                       HazardCausesDamage, AwarenessTriggersAggro,
                       FleeTransitionOnLowHealth, MultiBotIndependent,
                       DeterministicOrder);

JSON_SETTINGS_REGISTRY(Automation::Pipeline::FTickAssertions,
                       TickCountAdvanced, ActionDispatched,
                       HealthUnchanged);
JSON_SETTINGS_REGISTRY(Automation::Pipeline::FHazardAssertions,
                       HealthReducedByHazard, HealthAfterHazard,
                       HazardBotTookDamage);
JSON_SETTINGS_REGISTRY(Automation::Pipeline::FAggroAssertions, BotHasAggro,
                       PhaseIsCombat, RemembersEnemyPosition,
                       AwareBotHasAggro);
JSON_SETTINGS_REGISTRY(Automation::Pipeline::FFleeAssertions,
                       PhaseTransitionedToFlee);
JSON_SETTINGS_REGISTRY(Automation::Pipeline::FBotAssertions, BotsProcessed,
                       IdleBotFullHealth);
JSON_SETTINGS_REGISTRY(Automation::Pipeline::FDeterminismAssertions,
                       HealthDeterministic, PositionDeterministic,
                       PhaseDeterministic, AggroDeterministic,
                       ActionCountDeterministic);

/** User Story: As a automation scenarios pipeline consumer, I need to invoke read pipeline assertions through a stable signature so the automation scenarios pipeline workflow remains explicit and composable. @fn inline Automation::Pipeline::FAssertions ReadPipelineAssertions(const TSharedPtr<FJsonObject> &Object) */
inline Automation::Pipeline::FAssertions
ReadPipelineAssertions(const TSharedPtr<FJsonObject> &Object) {
  return {
      ReadSettingsFields<Automation::Pipeline::FTickAssertions>(
          Object, JSON_SETTINGS_ATOMS(TickCountAdvanced, ActionDispatched,
                                      HealthUnchanged)),
      ReadSettingsFields<Automation::Pipeline::FHazardAssertions>(
          Object, JSON_SETTINGS_ATOMS(HealthReducedByHazard,
                                      HealthAfterHazard,
                                      HazardBotTookDamage)),
      ReadSettingsFields<Automation::Pipeline::FAggroAssertions>(
          Object, JSON_SETTINGS_ATOMS(BotHasAggro, PhaseIsCombat,
                                      RemembersEnemyPosition,
                                      AwareBotHasAggro)),
      ReadSettingsFields<Automation::Pipeline::FFleeAssertions>(
          Object, JSON_SETTINGS_ATOMS(PhaseTransitionedToFlee)),
      ReadSettingsFields<Automation::Pipeline::FBotAssertions>(
          Object, JSON_SETTINGS_ATOMS(BotsProcessed, IdleBotFullHealth)),
      ReadSettingsFields<Automation::Pipeline::FDeterminismAssertions>(
          Object, JSON_SETTINGS_ATOMS(HealthDeterministic,
                                      PositionDeterministic,
                                      PhaseDeterministic,
                                      AggroDeterministic,
                                      ActionCountDeterministic))};
}

template <> struct TJsonSettingsRegistry<Automation::Pipeline::FSettings> {
  /** User Story: As a pipeline automation settings consumer, I need to access pipeline fields through a stable signature so end-to-end scenarios remain data-driven. @fn static const TArray<TField<Automation::Pipeline::FSettings>> &Fields() */
  static const TArray<TField<Automation::Pipeline::FSettings>> &Fields() {
    static const TArray<TField<Automation::Pipeline::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Pipeline::FSettings, Spec, Groups,
                                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Pipeline::FSettings,
                ReadSettingsWith<Automation::Pipeline::FTests>(
                    JSON_SETTINGS_ATOMS(
                        IdleTickAdvancesState, HazardCausesDamage,
                        AwarenessTriggersAggro, FleeTransitionOnLowHealth,
                        MultiBotIndependent, DeterministicOrder)),
                Tests),
            JSON_OBJECT_SETTING_FIELDS(Automation::Pipeline::FSettings,
                                       ReadPipelineAssertions, Assertions)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
