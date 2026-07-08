#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"

class UMaterialInterface;
class UStaticMeshComponent;

namespace ForbocAI {
namespace Game {
namespace Level {

struct FLevelRetroTextureApply {
  UStaticMeshComponent *Part;
  UMaterialInterface *BaseMaterial;
  ELevelRetroTexture Texture;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  ForbocAI::Game::Data::FRenderingSettings Settings;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
