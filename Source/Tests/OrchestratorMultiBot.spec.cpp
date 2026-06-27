/**
 * Orchestrator Multi-Bot Spec
 *
 * User Story: As a game developer managing multiple AI NPCs, I need bot
 * registration and movement to flow through one RTK runtime store so selectors
 * read the same state that reducers update.
 */

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/BotActions.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Runtime/RuntimeStore.h"
#include "Misc/AutomationTest.h"

using namespace ForbocAI::Demo::Level;

DEFINE_SPEC(FOrchestratorMultiBotSpec, "ForbocAI.Bot.Orchestrator.MultiBot",
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags_ApplicationContextMask)

void FOrchestratorMultiBotSpec::Define() {
  Describe("Single RTK Runtime Store", [this]() {
    It("Should register bots into one root store", [this]() {
      rtk::EnhancedStore<FRuntimeState> Store =
          RuntimeStore::ConfigureStore();

      Store.dispatch(BotActions::BotUpserted()(
          FBotEntity{TEXT("bot-alpha"), TEXT("Bot Alpha"),
                     EBotEntityKind::Townsperson, EBotAlignment::Friendly,
                     true}));
      Store.dispatch(BotActions::BotUpserted()(
          FBotEntity{TEXT("bot-beta"), TEXT("Bot Beta"),
                     EBotEntityKind::Townsperson, EBotAlignment::Friendly,
                     true}));
      Store.dispatch(BotActions::BotUpserted()(
          FBotEntity{TEXT("horse-one"), TEXT("Horse One"),
                     EBotEntityKind::Horse, EBotAlignment::Neutral, true}));

      const TArray<FBotEntity> Bots =
          RuntimeSelectors::SelectBots(Store.getState());

      TestEqual(TEXT("Three bots in root state"), Bots.Num(), 3);
      TestTrue(TEXT("Bot alpha selectable"),
               RuntimeSelectors::SelectBotById(Store.getState(),
                                               TEXT("bot-alpha"))
                   .hasValue);
      TestTrue(TEXT("Horse selectable"),
               RuntimeSelectors::SelectBotById(Store.getState(),
                                               TEXT("horse-one"))
                   .hasValue);
    });

    It("Should dispatch movement through the position slice", [this]() {
      rtk::EnhancedStore<FRuntimeState> Store =
          RuntimeStore::ConfigureStore();

      Store.dispatch(BotActions::BotUpserted()(
          FBotEntity{TEXT("moving-bot"), TEXT("Moving Bot"),
                     EBotEntityKind::Townsperson, EBotAlignment::Friendly,
                     true}));
      Store.dispatch(BotPositionActions::BotPositionUpserted()(
          FBotPositionComponent{
              TEXT("moving-bot"), FLevelLocalPoint{0.0f, 0.0f, 0.0f},
              FVector::ZeroVector, true, true}));
      Store.dispatch(BotPositionActions::BotPositionMoved()(
          FBotPositionMoved{
              TEXT("moving-bot"), FLevelLocalPoint{1.0f, 2.0f, 0.0f},
              FVector(100.0f, 200.0f, 0.0f), true, true}));

      const func::Maybe<FBotPositionComponent> Position =
          RuntimeSelectors::SelectBotPositionById(Store.getState(),
                                                  TEXT("moving-bot"));

      TestTrue(TEXT("Position selectable"), Position.hasValue);
      TestEqual(TEXT("World position updated"),
                Position.hasValue ? Position.value.WorldLocation
                                  : FVector::ZeroVector,
                FVector(100.0f, 200.0f, 0.0f));
    });
  });
}
