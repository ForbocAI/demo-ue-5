#pragma once
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"


#include "Components/SkinnedMeshComponent.h"
#include "Engine/SkinnedAsset.h"
#include "Rendering/SkeletalMeshRenderData.h"

namespace ForbocAI {
namespace Game {
namespace RenderingDistanceLodSelectors {

/** User Story: As a skeletal LOD consumer, I need the loaded LOD count through a stable signature so clamping remains pure and reusable. @fn inline int32 SelectLodCount(USkinnedMeshComponent *Component) */
inline int32 SelectLodCount(USkinnedMeshComponent *Component) {
  const USkinnedAsset *Asset =
      Component != nullptr ? Component->GetSkinnedAsset() : nullptr;
  const FSkeletalMeshRenderData *RenderData =
      Asset != nullptr ? Asset->GetResourceForRendering() : nullptr;
  return RenderData != nullptr ? RenderData->LODRenderData.Num()
                               : FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA;
}

/** User Story: As a skeletal LOD consumer, I need a valid forced LOD through a stable signature so views never submit an out-of-range model. @fn inline int32 SelectForcedLodModel(USkinnedMeshComponent *Component, int32 Requested) */
inline int32 SelectForcedLodModel(USkinnedMeshComponent *Component,
                                  int32 Requested) {
  const int32 Count = SelectLodCount(Component);
  return Requested <= FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA ||
                 Count <= FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA
             ? FMath::Max(Requested,
                          FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA)
             : FMath::Clamp(Requested,
                            FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4,
                            Count);
}

/** User Story: As a skeletal LOD consumer, I need a valid minimum LOD through a stable signature so views never submit an out-of-range index. @fn inline int32 SelectMinLodModel(USkinnedMeshComponent *Component, int32 Requested) */
inline int32 SelectMinLodModel(USkinnedMeshComponent *Component,
                               int32 Requested) {
  const int32 Count = SelectLodCount(Component);
  return Count <= FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA
             ? FMath::Max(Requested,
                          FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA)
             : FMath::Clamp(
                   Requested, FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA,
                   Count - FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4);
}

} // namespace RenderingDistanceLodSelectors
} // namespace Game
} // namespace ForbocAI
