#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FDomainRegistrationSettings {
  FString Path;
  int32 Kind;
};

struct FEcsSettings {
  TArray<FDomainRegistrationSettings> DomainRegistry;
};

/** User Story: As an ECS settings consumer, I need to compare domain registrations through a stable signature so authored registry state remains deterministic. @fn inline bool operator==(const FDomainRegistrationSettings &Left, const FDomainRegistrationSettings &Right) */
inline bool operator==(const FDomainRegistrationSettings &Left,
                       const FDomainRegistrationSettings &Right) {
  return Left.Path == Right.Path && Left.Kind == Right.Kind;
}

/** User Story: As an ECS settings consumer, I need to compare domain registrations for inequality through a stable signature so authored registry changes remain observable. @fn inline bool operator!=(const FDomainRegistrationSettings &Left, const FDomainRegistrationSettings &Right) */
inline bool operator!=(const FDomainRegistrationSettings &Left,
                       const FDomainRegistrationSettings &Right) {
  return !(Left == Right);
}

/** User Story: As an ECS settings consumer, I need to compare ECS settings through a stable signature so authored registry state remains deterministic. @fn inline bool operator==(const FEcsSettings &Left, const FEcsSettings &Right) */
inline bool operator==(const FEcsSettings &Left,
                       const FEcsSettings &Right) {
  return Left.DomainRegistry == Right.DomainRegistry;
}

/** User Story: As an ECS settings consumer, I need to compare ECS settings for inequality through a stable signature so authored registry changes remain observable. @fn inline bool operator!=(const FEcsSettings &Left, const FEcsSettings &Right) */
inline bool operator!=(const FEcsSettings &Left,
                       const FEcsSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
