#pragma once

#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Entities/Environments/Nature/EnvironmentsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

using FProjectNatureFeatureEntityPayload =
    EntitiesAdapters::TProjectEntityPayload<FFeatureSeed>;

/** User Story: As a nature entity consumer, I need a stable logical key so nature systems and views address one ECS entity. @fn ecs::EntityKey NatureEntityKey(const FString &Id) */
ecs::EntityKey NatureEntityKey(const FString &Id);

/** User Story: As a nature entity consumer, I need feature records projected through their domain-owned adapter so the root ECS world remains normalized. @fn ecs::FWorld ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) */
ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload);

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
