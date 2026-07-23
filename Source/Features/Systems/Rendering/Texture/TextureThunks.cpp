#include "Features/Systems/Rendering/Texture/Palette/Texture/TextureThunks.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
/** User Story: As a systems rendering texture consumer, I need to invoke load blockout material through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn UMaterialInterface *LoadBlockoutMaterial(const FString &Path) */
UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return LoadObject<UMaterialInterface>(nullptr, *Path);
}

/** User Story: As a systems rendering texture consumer, I need to invoke apply texture through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn void ApplyTexture(const FLevelRetroTextureApply &Request) */
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
