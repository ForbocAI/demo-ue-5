#include "Features/Systems/Rendering/Texture/Palette/PaletteThunks.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return LoadObject<UMaterialInterface>(nullptr, *Path);
}

void ApplyTexture(const FLevelRetroTextureApply &Request) {
  check(Request.Part);
  check(Request.BaseMaterial);

  const FLevelRetroTextureSpec Spec = RenderingTextureReducers::ReduceTextureSpec(
      {Request.Texture, Request.TextureCatalog});
  UTexture2D *RetroTexture = TextureFor(Spec, Request.Settings);
  check(RetroTexture);

  UMaterialInstanceDynamic *Material =
      UMaterialInstanceDynamic::Create(Request.BaseMaterial, Request.Part);
  check(Material);
  check(Request.Settings.Buffer.MaterialSlotIndex >= int32{});

  Material->SetTextureParameterValue(
      FName(*Request.Settings.Format.MaterialParameter), RetroTexture);
  Request.Part->SetMaterial(Request.Settings.Buffer.MaterialSlotIndex, Material);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
