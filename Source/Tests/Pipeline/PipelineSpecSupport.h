#pragma once

#include "CoreMinimal.h"
#include "Core/ue_fp.hpp"
#include "Misc/AutomationTest.h"
#include "Features/Systems/Bots/Behavior/BehaviorActions.h"
#include "Features/Systems/Bots/Behavior/BehaviorTypes.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/Bots/Pipeline/PipelineSlice.h"

using namespace ForbocAI::Game::Level;
using namespace ForbocAI::Game::Level::BotPipelineReducers;

namespace {

const ForbocAI::Game::Data::FBotSettings &
PipelineBotSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Bot;
}

const ForbocAI::Game::Data::Automation::Pipeline::FSettings &
PipelineSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.Pipeline;
}

const ForbocAI::Game::Data::Automation::Pipeline::FTests &PipelineTests() {
  return PipelineSettings().Tests;
}

const ForbocAI::Game::Data::Automation::Pipeline::FAssertions &
PipelineAssertions() {
  return PipelineSettings().Assertions;
}

FString PipelineInitialBotName() {
  return PipelineBotSettings().InitialName;
}

FString PipelineMoveBotName() {
  return PipelineBotSettings().MoveActionType;
}

FString PipelineAttackBotName() {
  return PipelineBotSettings().AttackActionType;
}

float PipelineTickDelta() {
  return PipelineBotSettings().PatrolTickIntervalSeconds;
}

float PipelineHazardDamage() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return Settings.InitialHealth * Settings.PhaseFleeHealthRatio;
}

float PipelineDamagePerSecond(const float DeltaTime) {
  return PipelineHazardDamage() / DeltaTime;
}

float PipelineExpectedHazardHealth() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return FMath::Max(Settings.MinimumHealth,
                    Settings.InitialHealth - PipelineHazardDamage());
}

float PipelineFleeHealth() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return Settings.InitialMaxHealth * Settings.PhaseFleeHealthRatio -
         Settings.MovementArrivalDistanceSquared;
}

FVector PipelineEnemyPosition() {
  return PipelineBotSettings().MoveActionOffset;
}

FBotCoreRuntimeState CreateTestBotState(const FString &Name) {
  return CreateBotCoreRuntimeInitialState({Name, PipelineBotSettings()});
}

FBotPipelineWorldSnapshot DefaultWorld(const FBotCoreRuntimeState &State,
                                       float DeltaTime) {
  return ReduceDefaultInputSnapshot({State, DeltaTime}).WorldSnapshot;
}

} // namespace
