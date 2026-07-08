/**
 * Orchestrator Multi-Bot Spec
 *
 * User Story: As a game developer managing multiple AI NPCs, I need bot
 * registration and movement to flow through one RTK runtime store so selectors
 * read the same state that reducers update.
 */

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/Bots/SystemsBotsActions.h"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/SystemsSelectors.h"
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

const ForbocAI::Game::Data::Automation::Bot::FOrchestratorSettings &
OrchestratorAutomationSettings() {
  return OrchestratorSettings().Automation.Bot.Orchestrator;
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

DEFINE_SPEC(FOrchestratorMultiBotSpec,
            OrchestratorAutomationSettings().MultiBotSpec,
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags_ApplicationContextMask)

void FOrchestratorMultiBotSpec::Define() {
  const ForbocAI::Game::Data::Automation::Bot::FOrchestratorSettings
      &Automation = OrchestratorAutomationSettings();

  Describe(Automation.Groups.RuntimeStore, [this, &Automation]() {
    It(Automation.Cases.RegisterBots, [this, &Automation]() {
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

      TestEqual(Automation.Assertions.ThreeBotsInRootState, Bots.Num(),
                SeedBots.Num());
      TestTrue(Automation.Assertions.BotSelectable,
               RuntimeSelectors::SelectBotById(EnhancedStoreValue.getState(),
                                               SeedBots[SeedBots.Num() -
                                                        SeedBots.Num()]
                                                   .Id)
                   .hasValue);
      TestTrue(Automation.Assertions.HorseSelectable,
               RuntimeSelectors::SelectBotById(EnhancedStoreValue.getState(),
                                               SeedBots.Last().Id)
                   .hasValue);
    });

    It(Automation.Cases.DispatchMovement, [this, &Automation]() {
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

      TestTrue(Automation.Assertions.PositionSelectable, Position.hasValue);
      TestEqual(Automation.Assertions.WorldPositionUpdated,
                Position.hasValue ? Position.value.WorldLocation
                                  : FVector::ZeroVector,
                Settings.Bot.MoveActionOffset);
    });
  });
}
