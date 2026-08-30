#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FDomainRegistrationSettings, Path, Kind);

template <> struct TJsonSettingsRegistry<FEcsSettings> {
  /** User Story: As a settings registry ecs consumer, I need to invoke fields through a stable signature so the settings registry ecs workflow remains explicit and composable. @fn static const TArray<TField<FEcsSettings>> &Fields() */
  static const TArray<TField<FEcsSettings>> &Fields() {
    static const TArray<TField<FEcsSettings>> RegisteredFields = {
        JSON_OBJECT_ARRAY_SETTING_FIELDS(
            FEcsSettings,
            ReadSettingsWith<FDomainRegistrationSettings>(
                JSON_SETTINGS_ATOMS(Path, Kind)),
            DomainRegistry)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
