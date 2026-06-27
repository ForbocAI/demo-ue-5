#pragma once

#include "CoreMinimal.h"
#include "Features/Components/RetroStyle.h"

class UMaterialInterface;
class UStaticMeshComponent;

namespace ForbocAI {
namespace Demo {
namespace Map {

struct FMapRetroTextureApply {
  UStaticMeshComponent *Part;
  UMaterialInterface *BaseMaterial;
  EMapRetroTexture Texture;
};

namespace RetroRendering {

void ApplyRuntimeProfile();
UMaterialInterface *LoadBlockoutMaterial();
void ApplyTexture(const FMapRetroTextureApply &Request);

} // namespace RetroRendering
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
