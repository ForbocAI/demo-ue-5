#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Data/RuntimeSettings/BotSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/LevelSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/PlayerSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/RenderingSettingsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace RuntimeSettingsAdapters {
namespace Json = JsonAdapters;

FDemoRuntimeSettings
ReadDemoRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonObjectValueReader ObjectValue = Json::ObjectValueIn(Object);
  FDemoRuntimeSettings Settings;
  Settings.PlayerPresentation =
      PlayerSettingsAdapters::ReadPlayerPresentationSettings(
          ObjectValue(TEXT("player_presentation")));
  Settings.Interaction = PlayerSettingsAdapters::ReadInteractionSettings(
      ObjectValue(TEXT("interaction")));
  Settings.TownspersonDefaults =
      BotSettingsAdapters::ReadTownspersonDefaultsSettings(
          ObjectValue(TEXT("townsperson_defaults")));
  Settings.LevelTerrainSources =
      LevelSettingsAdapters::ReadLevelTerrainSourceSettings(
          ObjectValue(TEXT("level_terrain_sources")));
  Settings.LevelDataSources =
      LevelSettingsAdapters::ReadLevelDataSourceSettings(
          ObjectValue(TEXT("level_data_sources")));
  Settings.RuntimeValidation =
      LevelSettingsAdapters::ReadRuntimeValidationSettings(
          ObjectValue(TEXT("runtime_validation")));
  Settings.LevelGeometry = LevelSettingsAdapters::ReadLevelGeometrySettings(
      ObjectValue(TEXT("level_geometry")));
  Settings.RenderingAssets =
      RenderingSettingsAdapters::ReadRenderingAssetPathSettings(
          ObjectValue(TEXT("rendering_assets")));
  Settings.RenderingProfile =
      RenderingSettingsAdapters::ReadRenderingProfileSettings(
          ObjectValue(TEXT("rendering_profile")));
  Settings.TextureCatalog =
      RenderingSettingsAdapters::ReadTextureCatalogSettings(Object);
  Settings.TownspersonPresentation =
      BotSettingsAdapters::ReadTownspersonPresentationSettings(
          ObjectValue(TEXT("townsperson_presentation")));
  Settings.HorsePresentation =
      BotSettingsAdapters::ReadHorsePresentationSettings(
          ObjectValue(TEXT("horse_presentation")));
  return Settings;
}

FDemoRuntimeSettings LoadDemoRuntimeSettings() {
  return ReadDemoRuntimeSettings(
      Json::LoadRequiredObjectFromContent(
          {TEXT("Data/runtime_demo_settings.json")}));
}

} // namespace RuntimeSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
