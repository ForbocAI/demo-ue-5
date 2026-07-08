#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FEcsDomainRegistrationSettings, Path, Kind);

template <> struct TJsonSettingsRegistry<FEcsSettings> {
  static const TArray<TField<FEcsSettings>> &Fields() {
    static const TArray<TField<FEcsSettings>> RegisteredFields = {
        JSON_OBJECT_ARRAY_SETTING_FIELDS(
            FEcsSettings,
            ReadSettingsWith<FEcsDomainRegistrationSettings>(
                JSON_SETTINGS_ATOMS(Path, Kind)),
            DomainRegistry)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
