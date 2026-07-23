#pragma once

#include "Core/fp.hpp"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsProjectionBotAdapters {

/** User Story: As a projection bot domains consumer, I need to invoke build bot projection domains through a stable signature so the projection bot domains workflow remains explicit and composable. @fn inline TArray<TArray<FString>> BuildBotProjectionDomains(const FString &BotSystemDomain) */
inline TArray<TArray<FString>>
BuildBotProjectionDomains(const FString &BotSystemDomain) {
  return {ComponentsAdapters::ComponentDomain({"Systems", "Bots"},
                                              BotSystemDomain),
          ComponentsAdapters::ComponentDomain(
              {"Systems", "Projection", "Bots"}),
          ComponentsAdapters::ComponentDomain(
              {"Systems", "Projection", "Bots"}, BotSystemDomain)};
}
template <typename ComponentCatalog>
struct FBotProjectionRequest {
  FString BotSystemDomain;
  ComponentCatalog Components;
};

/** User Story: As a projection bot domains consumer, I need to invoke make bot projection request through a stable signature so the projection bot domains workflow remains explicit and composable. @fn template <typename ComponentCatalog> FBotProjectionRequest<ComponentCatalog> MakeBotProjectionRequest(const FString &BotSystemDomain, const ComponentCatalog &Components) */
template <typename ComponentCatalog>
FBotProjectionRequest<ComponentCatalog>
MakeBotProjectionRequest(const FString &BotSystemDomain,
                         const ComponentCatalog &Components) {
  return {BotSystemDomain, Components};
}

template <typename T> struct TTypeWrapper { using Type = T; };

/** User Story: As a projection bot domains consumer, I need to invoke project bot payload through a stable signature so the projection bot domains workflow remains explicit and composable. @fn template <typename Component, typename Payload, typename ComponentCatalog> ecs::FWorld ProjectBotPayload(const Payload &PayloadValue, const FBotProjectionRequest<ComponentCatalog> &Request, TFunctionRef<FString(const typename TTypeWrapper<Payload>::Type &)> SelectIdValue, TFunctionRef<const Component &(const typename TTypeWrapper<Payload>::Type &)> SelectSourceValue) */
template <typename Component, typename Payload, typename ComponentCatalog>
ecs::FWorld ProjectBotPayload(const Payload &PayloadValue,
                              const FBotProjectionRequest<ComponentCatalog> &Request,
                              TFunctionRef<FString(const typename TTypeWrapper<Payload>::Type &)> SelectIdValue,
                              TFunctionRef<const Component &(const typename TTypeWrapper<Payload>::Type &)> SelectSourceValue) {
  return ComponentsAdapters::ProjectEntityCatalog(
      PayloadValue,
      ComponentsAdapters::TEntityCatalogProjection{
          [SelectIdValue](const Payload &SelectedPayload) {
            return EntitiesAdapters::BotEntityKey(SelectIdValue(SelectedPayload));
          },
          func::constant<TArray<TArray<FString>>>(
              BuildBotProjectionDomains(Request.BotSystemDomain)),
          SelectSourceValue, Request.Components});
}

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
