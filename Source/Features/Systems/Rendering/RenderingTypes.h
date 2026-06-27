#pragma once

#include "Core/rtk.hpp"

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

struct FRenderingPayload {
  FString Id;
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
};

struct FRenderingState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  bool bReady = false;
};

inline bool operator==(const FRenderingPayload &Left,
                       const FRenderingPayload &Right) {
  return Left.Id == Right.Id && Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog;
}

inline bool operator!=(const FRenderingPayload &Left,
                       const FRenderingPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value) &&
         Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.bReady == Right.bReady;
}

inline bool operator!=(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
