/**
 * Orchestrator Multi-Bot Spec
 *
 * User Story: As a game developer managing multiple AI NPCs, I need bot
 * registration and movement to flow through one RTK runtime store so selectors
 * read the same state that reducers update.
 */

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Adapters.h"
#include "Features/Systems/Bots/Actions.h"
#include "Features/Systems/Bots/Position/Actions.h"
#include "Features/Systems/Selectors.h"
#include "Store.h"
#include "Misc/AutomationTest.h"

using namespace ForbocAI::Game::Level;

namespace {

const ForbocAI::Game::Data::FSettings &
OrchestratorSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings;
}

FLevelLocalPoint LocalPointFromVector(const FVector &Vector) {
  return {static_cast<float>(Vector.X), static_cast<float>(Vector.Y),
          static_cast<float>(Vector.Z)};
}

TArray<FBotEntity> OrchestratorSeedBots() {
  const ForbocAI::Game::Data::FSettings &Settings =
      OrchestratorSettings();
  return {
      FBotEntity{Settings.TownspersonDefaults.Id,
                 Settings.TownspersonDefaults.Name,
                 EBotEntityKind::Townsperson, EBotAlignment::Friendly,
                 Settings.Bot.bRegisteredBotActive},
      FBotEntity{Settings.Bot.InitialName,
                 Settings.Bot.InitialName,
                 EBotEntityKind::Townsperson, EBotAlignment::Friendly,
                 Settings.Bot.bRegisteredBotActive},
      FBotEntity{Settings.HorsePresentation.DefaultName,
                 Settings.HorsePresentation.DefaultName, EBotEntityKind::Horse,
                 EBotAlignment::Neutral,
                 Settings.Bot.bRegisteredBotActive}};
}

FBotEntity OrchestratorMovingBot() {
  const ForbocAI::Game::Data::FSettings &Settings =
      OrchestratorSettings();
  return {Settings.Bot.InitialName, Settings.TownspersonDefaults.Name,
          EBotEntityKind::Townsperson, EBotAlignment::Friendly,
          Settings.Bot.bRegisteredBotActive};
}

} // namespace

DEFINE_SPEC(FOrchestratorMultiBotSpec, "ForbocAI.Bot.Orchestrator.MultiBot",
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags_ApplicationContextMask)

void FOrchestratorMultiBotSpec::Define() {
  Describe("Single RTK Runtime Store", [this]() {
    It("Should register bots into one root store", [this]() {
      rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
          Store::ConfigureStore();

      const TArray<FBotEntity> SeedBots = OrchestratorSeedBots();
      func::for_each_indexed(
          SeedBots, static_cast<size_t>(SeedBots.Num()),
          [&EnhancedStoreValue](const FBotEntity &Bot) {
            EnhancedStoreValue.dispatch(BotActions::BotUpserted()(Bot));
          });

      const TArray<FBotEntity> Bots =
          RuntimeSelectors::SelectBots(EnhancedStoreValue.getState());

      TestEqual(TEXT("Three bots in root state"), Bots.Num(), SeedBots.Num());
      TestTrue(TEXT("Bot alpha selectable"),
               RuntimeSelectors::SelectBotById(EnhancedStoreValue.getState(),
                                               SeedBots[SeedBots.Num() -
                                                        SeedBots.Num()]
                                                   .Id)
                   .hasValue);
      TestTrue(TEXT("Horse selectable"),
               RuntimeSelectors::SelectBotById(EnhancedStoreValue.getState(),
                                               SeedBots.Last().Id)
                   .hasValue);
    });

    It("Should dispatch movement through the position slice", [this]() {
      rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
          Store::ConfigureStore();
      const ForbocAI::Game::Data::FSettings &Settings =
          OrchestratorSettings();
      const FBotEntity MovingBot = OrchestratorMovingBot();
      const FLevelLocalPoint InitialLocalLocation =
          LocalPointFromVector(Settings.Bot.InitialPosition);
      const FLevelLocalPoint TargetLocalLocation =
          LocalPointFromVector(Settings.Bot.MoveActionOffset);

      EnhancedStoreValue.dispatch(BotActions::BotUpserted()(MovingBot));
      EnhancedStoreValue.dispatch(BotPositionActions::BotPositionUpserted()(
          FBotPositionComponent{
              MovingBot.Id, InitialLocalLocation,
              Settings.Bot.InitialPosition,
              Settings.Bot.bPositionPayloadHasWorldLocation,
              Settings.Bot.bPositionPayloadHasLocalLocation}));
      EnhancedStoreValue.dispatch(BotPositionActions::BotPositionMoved()(
          FBotPositionMoved{
              MovingBot.Id, TargetLocalLocation,
              Settings.Bot.MoveActionOffset,
              Settings.Bot.bPositionPayloadHasWorldLocation,
              Settings.Bot.bPositionPayloadHasLocalLocation}));

      const func::Maybe<FBotPositionComponent> Position =
          RuntimeSelectors::SelectBotPositionById(EnhancedStoreValue.getState(),
                                                  MovingBot.Id);

      TestTrue(TEXT("Position selectable"), Position.hasValue);
      TestEqual(TEXT("World position updated"),
                Position.hasValue ? Position.value.WorldLocation
                                  : FVector::ZeroVector,
                Settings.Bot.MoveActionOffset);
    });
  });
}
