#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FTownspersonDefaultsSettings {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
};

/** User Story: As a bot defaults consumer, I need to compare values for equality through a stable signature so default townsperson state remains explicit and composable. @fn inline bool operator==(const FTownspersonDefaultsSettings &Left, const FTownspersonDefaultsSettings &Right) */
inline bool operator==(const FTownspersonDefaultsSettings &Left,
                       const FTownspersonDefaultsSettings &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Role == Right.Role && Left.Persona == Right.Persona &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine;
}

/** User Story: As a bot defaults consumer, I need to compare values for inequality through a stable signature so default townsperson state remains explicit and composable. @fn inline bool operator!=(const FTownspersonDefaultsSettings &Left, const FTownspersonDefaultsSettings &Right) */
inline bool operator!=(const FTownspersonDefaultsSettings &Left,
                       const FTownspersonDefaultsSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
