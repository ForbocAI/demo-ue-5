#pragma once

#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace RenderingSettingsAdapters {

/**
 * @brief Reads rendering asset path settings from one JSON object.
 * @signature FRenderingAssetPathSettings ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a rendering reducer author, authored asset paths become typed
 * state before views load display assets.
 */
FRenderingAssetPathSettings
ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Reads runtime rendering profile settings from one JSON object.
 * @signature FRenderingProfileSettings ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a rendering slice author, profile fields reduce into one
 * store-owned rendering state rather than view-owned constants.
 */
FRenderingProfileSettings
ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Reads one texture catalog entry from a JSON object.
 * @signature FRenderingTextureSpecSettings ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a rendering adapter author, repeated texture specs map through
 * one typed payload factory before reducers own catalog state.
 */
FRenderingTextureSpecSettings
ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Reads the texture catalog settings from the root runtime JSON object.
 * @signature TArray<FRenderingTextureSpecSettings> ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a rendering selector author, texture catalog state should be
 * derived from one authored array and reduced into the single store.
 */
TArray<FRenderingTextureSpecSettings>
ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object);

struct FRenderingSettingsRequest {
  const TSharedPtr<FJsonObject> TextureSettings;
  const TSharedPtr<FJsonObject> ConsoleVariables;
  const TSharedPtr<FJsonObject> TexturePalettes;
};

/**
 * @brief Reads rendering runtime style settings from nested runtime JSON objects.
 * @signature FRenderingSettings ReadRenderingSettings(const FRenderingSettingsRequest &Request)
 *
 * User story: As a rendering maintainer, authored CVar mappings, palette
 * rules, hash parameters, and material parameter names live in JSON while
 * rendering code only interprets typed data.
 */
FRenderingSettings
ReadRenderingSettings(const FRenderingSettingsRequest &Request);

/**
 * @brief Reads authored distance-based runtime LOD stages.
 * @signature FRenderingDistanceLodSettings ReadRenderingDistanceLodSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a performance maintainer, distance LOD policy should be
 * authored JSON that reduces into feature state before views render actors.
 */
FRenderingDistanceLodSettings
ReadRenderingDistanceLodSettings(const TSharedPtr<FJsonObject> &Object);

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
