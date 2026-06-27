#pragma once

#include "Core/rtk.hpp"

#include "CoreMinimal.h"
#include "Features/Components/Rendering/RenderingTypes.h"

class UMaterialInterface;
class UStaticMeshComponent;

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FLevelRetroTextureApply {
  UStaticMeshComponent *Part;
  UMaterialInterface *BaseMaterial;
  ELevelRetroTexture Texture;
};

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
