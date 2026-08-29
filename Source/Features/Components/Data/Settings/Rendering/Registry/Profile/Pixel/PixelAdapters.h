#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Pixel/PixelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FBinding,
                       RuntimePixelMeshSectionIndex,
                       RuntimePixelMaterialIndex);
JSON_SETTINGS_REGISTRY(RenderingProfile::FQuadIndices, PixelQuadIndexA,
                       PixelQuadIndexB, PixelQuadIndexC, PixelQuadIndexD);
JSON_SETTINGS_REGISTRY(RenderingProfile::FQuadBounds,
                       PixelQuadHalfExtentMultiplier, PixelQuadUvMin,
                       PixelQuadUvMax);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FQuad, Binding, Indices,
                               Bounds);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
