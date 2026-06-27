#pragma once

#include "CoreMinimal.h"

namespace ForbocAI::Demo::Level {

inline FString BuildLocalReply(const FString &Name, const FString &Role,
                               const FString &Persona,
                               const FString &PlayerLine) {
  const FString Topic =
      PlayerLine.IsEmpty() ? TEXT("the gulch") : PlayerLine.Left(64);
  return FString::Printf(
      TEXT("%s (%s): %s. Around here in 1899, every claim, road, and rumor ")
      TEXT("runs through French Gulch. %s"),
      *Name, *Role, *Topic, *Persona);
}

} // namespace ForbocAI::Demo::Level
