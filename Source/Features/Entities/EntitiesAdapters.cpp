#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace EntitiesAdapters {

/** User Story: As an entity subdomain author, I need one prefix-driven key composer so every entity family can declare stable logical identity without duplicating string assembly. @fn ecs::EntityKey PrefixedEntityKey(const FPrefixedEntityKeyPayload &Payload) */
ecs::EntityKey PrefixedEntityKey(const FPrefixedEntityKeyPayload &Payload) {
  return Payload.Prefix + Payload.Id;
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
