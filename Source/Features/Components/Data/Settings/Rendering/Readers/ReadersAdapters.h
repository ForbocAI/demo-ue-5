#pragma once

#include "Features/Components/Data/Settings/Rendering/RenderingAdapters.h"
#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace RenderingSettingsAdapters {
namespace Json = JsonAdapters;
namespace {

/** User Story: As a settings rendering readers consumer, I need to invoke read rendering texture palette source through a stable signature so the settings rendering readers workflow remains explicit and composable. @fn FPaletteSettings ReadRenderingTexturePaletteSource(const FString &Source) */
FPaletteSettings
ReadRenderingTexturePaletteSource(const FString &Source) {
  return Json::ReadSettingsWith<FPaletteSettings>(
      JSON_SETTINGS_ATOMS(Texture, Rules))(
      Json::ReadObjectField(Json::LoadRequiredObjectFromContent({Source}),
                            "TexturePalette"));
}

/** User Story: As a settings rendering readers consumer, I need to invoke read rendering texture palette sources through a stable signature so the settings rendering readers workflow remains explicit and composable. @fn TArray<FPaletteSettings> ReadRenderingTexturePaletteSources(const TSharedPtr<FJsonObject> &Object) */
TArray<FPaletteSettings>
ReadRenderingTexturePaletteSources(const TSharedPtr<FJsonObject> &Object) {
  return func::map_array<FString, FPaletteSettings>(
      Json::ReadStringArrayField(Object, "TexturePaletteSources"),
      ReadRenderingTexturePaletteSource);
}

} // namespace

/** User Story: As a settings rendering readers consumer, I need to invoke read rendering asset path settings through a stable signature so the settings rendering readers workflow remains explicit and composable. @fn FRenderingAssetPathSettings ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object) */
FRenderingAssetPathSettings
ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingAssetPathSettings>(
      Object, JSON_SETTINGS_ATOMS(LevelCubeMeshPath, BlockoutMaterialPath,
                                  TerrainVertexColorMaterialPath));
}

/** User Story: As a settings rendering readers consumer, I need to invoke read rendering profile settings through a stable signature so the settings rendering readers workflow remains explicit and composable. @fn FProfileSettings ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object) */
FProfileSettings
ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FProfileSettings>(
      Object, Json::TJsonSettingsRegistry<FProfileSettings>::Fields());
}

/** User Story: As a settings rendering readers consumer, I need to invoke read rendering texture spec settings through a stable signature so the settings rendering readers workflow remains explicit and composable. @fn FSpecSettings ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object) */
FSpecSettings
ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FSpecSettings>(
      Object, JSON_SETTINGS_ATOMS(Texture, Name, Use, Size));
}

/** User Story: As a settings rendering readers consumer, I need to invoke read texture catalog settings through a stable signature so the settings rendering readers workflow remains explicit and composable. @fn TArray<FSpecSettings> ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object) */
TArray<FSpecSettings>
ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadObjectArrayField<FSpecSettings>(
      Object, "TextureCatalog", ReadRenderingTextureSpecSettings);
}

/** User Story: As a settings rendering readers consumer, I need to invoke read rendering settings through a stable signature so the settings rendering readers workflow remains explicit and composable. @fn FRenderingSettings ReadRenderingSettings(const FSettingsRequest &Request) */
FRenderingSettings
ReadRenderingSettings(const FSettingsRequest &Request) {
  FRenderingSettings Settings =
      Json::ReadSettingsFields<FRenderingSettings>(
          Request.TextureSettings,
          JSON_SETTINGS_ATOMS(TextureChannels, TextureAlpha,
                              TextureCacheKeyFormat, MaterialTextureParameter,
                              PredicateAlwaysKind, PredicateModEqualsKind,
                              PredicateModLessThanKind, ResultSolidKind,
                              ResultMixKind, TextureMipIndex,
                              TextureMaterialSlotIndex,
                              TextureHash));
  Settings.Console =
      Json::ReadSettingsWith<FRenderingConsoleSettings>(
          JSON_SETTINGS_ATOMS(
              ProfileFieldAntiAliasingMethod,
              ProfileFieldPostProcessAaQuality, ProfileFieldShadowCascades,
              ProfileFieldShadowMaxResolution, ProfileFieldScreenPercentage,
              ProfileFieldViewDistanceScale,
              ProfileFieldFoliageDensityScale,
              ProfileFieldGrassDensityScale, ValueKindFixedInt,
              ValueKindProfileInt, ValueKindFixedFloat,
              ValueKindProfileFloat))(Request.ConsoleSettings);
  Settings.ConsoleVariables =
      Json::ReadObjectArrayField<FRenderingConsoleVariableSettings>(
          Request.ConsoleVariables, "ConsoleVariables",
          Json::ReadSettingsWith<FRenderingConsoleVariableSettings>(
              JSON_SETTINGS_ATOMS(Name, ValueKind, ProfileField, IntValue,
                                  FloatValue)));
  Settings.TexturePalettes = ReadRenderingTexturePaletteSources(Request.TexturePalettes);
  return Settings;
}

/** User Story: As a settings rendering readers consumer, I need to invoke read rendering distance lod settings through a stable signature so the settings rendering readers workflow remains explicit and composable. @fn FLodSettings ReadRenderingDistanceLodSettings(const TSharedPtr<FJsonObject> &Object) */
FLodSettings
ReadRenderingDistanceLodSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLodSettings>(
      Object, JSON_SETTINGS_ATOMS(Stages));
}

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
