// View boundary helper: clamp Unreal skeletal LOD requests to the mesh that is
// actually loaded before handing values to the component API.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkinnedMeshComponent.h"
#include "Engine/SkinnedAsset.h"
#include "Rendering/SkeletalMeshRenderData.h"

namespace ForbocAI {
namespace Game {
namespace Views {
namespace SkeletalLodClamp {

/** User Story: As a skeletal lod clamp consumer, I need to invoke lod count through a stable signature so the skeletal lod clamp workflow remains explicit and composable. @fn inline int32 LodCount(USkinnedMeshComponent *Component) */
inline int32 LodCount(USkinnedMeshComponent *Component) {
  const USkinnedAsset *Asset =
      Component != nullptr ? Component->GetSkinnedAsset() : nullptr;
  const FSkeletalMeshRenderData *RenderData =
      Asset != nullptr ? Asset->GetResourceForRendering() : nullptr;
  return RenderData != nullptr ? RenderData->LODRenderData.Num() : 0;
}

/** User Story: As a skeletal lod clamp consumer, I need to invoke forced lod model through a stable signature so the skeletal lod clamp workflow remains explicit and composable. @fn inline int32 ForcedLodModel(USkinnedMeshComponent *Component, int32 Requested) */
inline int32 ForcedLodModel(USkinnedMeshComponent *Component,
                            int32 Requested) {
  const int32 Count = LodCount(Component);
  return Requested <= 0 || Count <= 0 ? FMath::Max(Requested, 0)
                                      : FMath::Clamp(Requested, 1, Count);
}

/** User Story: As a skeletal lod clamp consumer, I need to invoke min lod model through a stable signature so the skeletal lod clamp workflow remains explicit and composable. @fn inline int32 MinLodModel(USkinnedMeshComponent *Component, int32 Requested) */
inline int32 MinLodModel(USkinnedMeshComponent *Component,
                         int32 Requested) {
  const int32 Count = LodCount(Component);
  return Count <= 0 ? FMath::Max(Requested, 0)
                    : FMath::Clamp(Requested, 0, Count - 1);
}

/** User Story: As a skeletal lod clamp consumer, I need to invoke apply through a stable signature so the skeletal lod clamp workflow remains explicit and composable. @fn inline void Apply(USkinnedMeshComponent *Component, int32 ForcedModel, int32 MinModel) */
inline void Apply(USkinnedMeshComponent *Component, int32 ForcedModel,
                  int32 MinModel) {
  check(Component);
  Component->SetForcedLOD(ForcedLodModel(Component, ForcedModel));
  Component->OverrideMinLOD(MinLodModel(Component, MinModel));
}

} // namespace SkeletalLodClamp
} // namespace Views
} // namespace Game
} // namespace ForbocAI
