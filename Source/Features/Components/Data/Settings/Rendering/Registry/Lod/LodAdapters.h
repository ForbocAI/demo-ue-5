#pragma once

#include "Features/Components/Data/Settings/Rendering/RenderingAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

template <> struct TJsonSettingsRegistry<FStageSettings> {
  /** User Story: As a rendering registry lod consumer, I need to invoke fields through a stable signature so the rendering registry lod workflow remains explicit and composable. @fn static const TArray<TField<FStageSettings>> &Fields() */
  static const TArray<TField<FStageSettings>> &Fields() {
    static const TArray<TField<FStageSettings>> RegisteredFields = {
        NestedSettingField(JSON_SETTING_ATOM(Id),
                           NestedFieldMembers(&FStageSettings::Selection,
                                              &FStageSelectionSettings::Id)),
        NestedSettingField(JSON_SETTING_ATOM(MaxDistance),
                           NestedFieldMembers(&FStageSettings::Selection,
                                              &FStageSelectionSettings::MaxDistance)),
        NestedSettingField(
            JSON_SETTING_ATOM(StaticMeshForcedLodModel),
            NestedFieldMembers(&FStageSettings::Mesh,
                               &FStageMeshSettings::StaticMeshForcedLodModel)),
        NestedSettingField(
            JSON_SETTING_ATOM(SkeletalMeshForcedLodModel),
            NestedFieldMembers(&FStageSettings::Mesh,
                               &FStageMeshSettings::SkeletalMeshForcedLodModel)),
        NestedSettingField(
            JSON_SETTING_ATOM(SkeletalMeshMinLodModel),
            NestedFieldMembers(&FStageSettings::Mesh,
                               &FStageMeshSettings::SkeletalMeshMinLodModel)),
        NestedSettingField(JSON_SETTING_ATOM(CullDistance),
                           NestedFieldMembers(&FStageSettings::Timing,
                                              &FStageTimingSettings::CullDistance)),
        NestedSettingField(
            JSON_SETTING_ATOM(ActorTickIntervalSeconds),
            NestedFieldMembers(&FStageSettings::Timing,
                               &FStageTimingSettings::ActorTickIntervalSeconds)),
        NestedSettingField(
            JSON_SETTING_ATOM(bStaticVisible),
            NestedFieldMembers(&FStageSettings::Visibility,
                               &FStageVisibilitySettings::bStaticVisible)),
        NestedSettingField(
            JSON_SETTING_ATOM(bDynamicVisible),
            NestedFieldMembers(&FStageSettings::Visibility,
                               &FStageVisibilitySettings::bDynamicVisible)),
        NestedSettingField(
            JSON_SETTING_ATOM(bLabelsVisible),
            NestedFieldMembers(&FStageSettings::Visibility,
                               &FStageVisibilitySettings::bLabelsVisible)),
        NestedSettingField(JSON_SETTING_ATOM(bAnimated),
                           NestedFieldMembers(&FStageSettings::Behavior,
                                              &FStageBehaviorSettings::bAnimated)),
        NestedSettingField(
            JSON_SETTING_ATOM(bUpdateRateOptimizationsEnabled),
            NestedFieldMembers(
                &FStageSettings::Behavior,
                &FStageBehaviorSettings::bUpdateRateOptimizationsEnabled)),
        NestedSettingField(
            JSON_SETTING_ATOM(bPatrolEnabled),
            NestedFieldMembers(&FStageSettings::Behavior,
                               &FStageBehaviorSettings::bPatrolEnabled)),
        NestedSettingField(
            JSON_SETTING_ATOM(bCollisionEnabled),
            NestedFieldMembers(&FStageSettings::Behavior,
                               &FStageBehaviorSettings::bCollisionEnabled)),
        NestedSettingField(JSON_SETTING_ATOM(bCastShadow),
                           NestedFieldMembers(&FStageSettings::Behavior,
                                              &FStageBehaviorSettings::bCastShadow))};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
