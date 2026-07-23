#pragma once

#include "CoreMinimal.h"
#include "Core/fp.hpp"
#include "Misc/AutomationTest.h"
#include "Features/Systems/Bots/Behavior/BehaviorActions.h"
#include "Features/Systems/Bots/Behavior/BehaviorTypes.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/Bots/Pipeline/PipelineSlice.h"

using namespace ForbocAI::Game::Level;
using namespace ForbocAI::Game::Level::BotPipelineReducers;

namespace {

/** User Story: As a tests pipeline consumer, I need to invoke pipeline bot settings through a stable signature so the tests pipeline workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FBotSettings & PipelineBotSettings() */
const ForbocAI::Game::Data::FBotSettings &
PipelineBotSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Bot;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline settings through a stable signature so the tests pipeline workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Pipeline::FSettings & PipelineSettings() */
const ForbocAI::Game::Data::Automation::Pipeline::FSettings &
PipelineSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.Pipeline;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline tests through a stable signature so the tests pipeline workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Pipeline::FTests &PipelineTests() */
const ForbocAI::Game::Data::Automation::Pipeline::FTests &PipelineTests() {
  return PipelineSettings().Tests;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline assertions through a stable signature so the tests pipeline workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Pipeline::FAssertions & PipelineAssertions() */
const ForbocAI::Game::Data::Automation::Pipeline::FAssertions &
PipelineAssertions() {
  return PipelineSettings().Assertions;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline initial bot name through a stable signature so the tests pipeline workflow remains explicit and composable. @fn FString PipelineInitialBotName() */
FString PipelineInitialBotName() {
  return PipelineBotSettings().InitialName;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline move bot name through a stable signature so the tests pipeline workflow remains explicit and composable. @fn FString PipelineMoveBotName() */
FString PipelineMoveBotName() {
  return PipelineBotSettings().MoveActionType;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline attack bot name through a stable signature so the tests pipeline workflow remains explicit and composable. @fn FString PipelineAttackBotName() */
FString PipelineAttackBotName() {
  return PipelineBotSettings().AttackActionType;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline tick delta through a stable signature so the tests pipeline workflow remains explicit and composable. @fn float PipelineTickDelta() */
float PipelineTickDelta() {
  return PipelineBotSettings().PatrolTickIntervalSeconds;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline hazard damage through a stable signature so the tests pipeline workflow remains explicit and composable. @fn float PipelineHazardDamage() */
float PipelineHazardDamage() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return Settings.InitialHealth * Settings.PhaseFleeHealthRatio;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline damage per second through a stable signature so the tests pipeline workflow remains explicit and composable. @fn float PipelineDamagePerSecond(const float DeltaTime) */
float PipelineDamagePerSecond(const float DeltaTime) {
  return PipelineHazardDamage() / DeltaTime;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline expected hazard health through a stable signature so the tests pipeline workflow remains explicit and composable. @fn float PipelineExpectedHazardHealth() */
float PipelineExpectedHazardHealth() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return FMath::Max(Settings.MinimumHealth,
                    Settings.InitialHealth - PipelineHazardDamage());
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline flee health through a stable signature so the tests pipeline workflow remains explicit and composable. @fn float PipelineFleeHealth() */
float PipelineFleeHealth() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return Settings.InitialMaxHealth * Settings.PhaseFleeHealthRatio -
         Settings.MovementArrivalDistanceSquared;
}

/** User Story: As a tests pipeline consumer, I need to invoke pipeline enemy position through a stable signature so the tests pipeline workflow remains explicit and composable. @fn FVector PipelineEnemyPosition() */
FVector PipelineEnemyPosition() {
  return PipelineBotSettings().MoveActionOffset;
}

/** User Story: As a tests pipeline consumer, I need to invoke create test bot state through a stable signature so the tests pipeline workflow remains explicit and composable. @fn FBotCoreRuntimeState CreateTestBotState(const FString &Name) */
FBotCoreRuntimeState CreateTestBotState(const FString &Name) {
  return CreateBotCoreRuntimeInitialState({Name, PipelineBotSettings()});
}

/** User Story: As a tests pipeline consumer, I need to invoke default world through a stable signature so the tests pipeline workflow remains explicit and composable. @fn FBotPipelineWorldSnapshot DefaultWorld(const FBotCoreRuntimeState &State, float DeltaTime) */
FBotPipelineWorldSnapshot DefaultWorld(const FBotCoreRuntimeState &State,
                                       float DeltaTime) {
  return ReduceDefaultInputSnapshot({State, DeltaTime}).WorldSnapshot;
}

} // namespace
