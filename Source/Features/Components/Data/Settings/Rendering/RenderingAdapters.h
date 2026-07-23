#pragma once

#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace RenderingSettingsAdapters {

/**
 * @fn FRenderingAssetPathSettings ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads rendering asset path settings from one JSON object.
 *
 * User story: As a rendering reducer author, authored asset paths become typed
 * state before views load display assets.
 * User Story: As a data settings rendering consumer, I need to invoke read rendering asset path settings through a stable signature so the data settings rendering workflow remains explicit and composable.
 */
FRenderingAssetPathSettings
ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn FProfileSettings ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads runtime rendering profile settings from one JSON object.
 *
 * User story: As a rendering slice author, profile fields reduce into one
 * store-owned rendering state rather than view-owned constants.
 * User Story: As a data settings rendering consumer, I need to invoke read rendering profile settings through a stable signature so the data settings rendering workflow remains explicit and composable.
 */
FProfileSettings
ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn FSpecSettings ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads one texture catalog entry from a JSON object.
 *
 * User story: As a rendering adapter author, repeated texture specs map through
 * one typed payload factory before reducers own catalog state.
 * User Story: As a data settings rendering consumer, I need to invoke read rendering texture spec settings through a stable signature so the data settings rendering workflow remains explicit and composable.
 */
FSpecSettings
ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn TArray<FSpecSettings> ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads the texture catalog settings from the root runtime JSON object.
 *
 * User story: As a rendering selector author, texture catalog state should be
 * derived from one authored array and reduced into the single store.
 * User Story: As a data settings rendering consumer, I need to invoke read texture catalog settings through a stable signature so the data settings rendering workflow remains explicit and composable.
 */
TArray<FSpecSettings>
ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object);

struct FSettingsRequest {
  const TSharedPtr<FJsonObject> TextureSettings;
  const TSharedPtr<FJsonObject> ConsoleSettings;
  const TSharedPtr<FJsonObject> ConsoleVariables;
  const TSharedPtr<FJsonObject> TexturePalettes;
};

/**
 * @fn FRenderingSettings ReadRenderingSettings(const FSettingsRequest &Request)
 * @brief Reads rendering runtime style settings from nested runtime JSON objects.
 *
 * User story: As a rendering maintainer, authored CVar mappings, palette
 * rules, hash parameters, and material parameter names live in JSON while
 * rendering code only interprets typed data.
 * User Story: As a data settings rendering consumer, I need to invoke read rendering settings through a stable signature so the data settings rendering workflow remains explicit and composable.
 */
FRenderingSettings
ReadRenderingSettings(const FSettingsRequest &Request);

/**
 * @fn FLodSettings ReadRenderingDistanceLodSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads authored distance-based runtime LOD stages.
 *
 * User story: As a performance maintainer, distance LOD policy should be
 * authored JSON that reduces into feature state before views render actors.
 * User Story: As a data settings rendering consumer, I need to invoke read rendering distance lod settings through a stable signature so the data settings rendering workflow remains explicit and composable.
 */
FLodSettings
ReadRenderingDistanceLodSettings(const TSharedPtr<FJsonObject> &Object);

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
