#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class ETownspersonInteractionIntent : uint8 {
  General,
  Dialogue,
  Memory,
  CombatValidation
};

struct FTownspersonInteraction {
  FString InteractionPrompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
  ETownspersonInteractionIntent InteractionIntent;
};

struct FTownspersonSeed {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FTownspersonInteraction Interaction;
  TArray<FLevelLocalPoint> PatrolRoute;
};

/** User Story: As a townsperson entity consumer, I need value equality so authored interaction records remain deterministic. @fn inline bool operator==(const FTownspersonInteraction &Left, const FTownspersonInteraction &Right) */
inline bool operator==(const FTownspersonInteraction &Left,
                       const FTownspersonInteraction &Right) {
  return Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse &&
         Left.InteractionIntent == Right.InteractionIntent;
}

/** User Story: As a townsperson entity consumer, I need value inequality so authored interaction changes remain explicit. @fn inline bool operator!=(const FTownspersonInteraction &Left, const FTownspersonInteraction &Right) */
inline bool operator!=(const FTownspersonInteraction &Left,
                       const FTownspersonInteraction &Right) {
  return !(Left == Right);
}

/** User Story: As a townsperson entity consumer, I need value equality so authored identities, interaction, and routes remain deterministic. @fn inline bool operator==(const FTownspersonSeed &Left, const FTownspersonSeed &Right) */
inline bool operator==(const FTownspersonSeed &Left,
                       const FTownspersonSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Role == Right.Role && Left.Persona == Right.Persona &&
         Left.Interaction == Right.Interaction &&
         Left.PatrolRoute == Right.PatrolRoute;
}

/** User Story: As a townsperson entity consumer, I need value inequality so authored entity changes remain explicit. @fn inline bool operator!=(const FTownspersonSeed &Left, const FTownspersonSeed &Right) */
inline bool operator!=(const FTownspersonSeed &Left,
                       const FTownspersonSeed &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
