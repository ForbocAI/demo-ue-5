// View boundary helper: clamp Unreal skeletal LOD requests to the mesh that is
// actually loaded before handing values to the component API.

#pragma once
#include "Features/Systems/Rendering/Distance/Lod/Clamp/ClampSelectors.h"

#include "CoreMinimal.h"
#include "Components/SkinnedMeshComponent.h"

namespace ForbocAI {
namespace Game {
namespace Views {
namespace SkeletalLodClamp {

/** User Story: As a skeletal lod clamp consumer, I need to invoke apply through a stable signature so the skeletal lod clamp workflow remains explicit and composable. @fn inline void Apply(USkinnedMeshComponent *Component, int32 ForcedModel, int32 MinModel) */
inline void Apply(USkinnedMeshComponent *Component, int32 ForcedModel,
                  int32 MinModel) {
  check(Component);
  Component->SetForcedLOD(
      RenderingDistanceLodSelectors::SelectForcedLodModel(Component,
                                                          ForcedModel));
  Component->OverrideMinLOD(
      RenderingDistanceLodSelectors::SelectMinLodModel(Component, MinModel));
}

} // namespace SkeletalLodClamp
} // namespace Views
} // namespace Game
} // namespace ForbocAI
