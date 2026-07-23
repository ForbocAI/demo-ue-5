#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Systems/Bots/SystemsBotsTypes.h"
#include "Features/Systems/Bots/Position/PositionTypes.h"
#include "Features/Systems/Bots/AI/AITypes.h"

class AActor;

namespace ForbocAI {
namespace Game {
namespace Level {

using FBotSettings = ForbocAI::Game::Data::FBotSettings;

struct FBotPositionPayloadSource {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  FBotSettings Settings;
};

struct FBotRegistrationPayloadSource {
  FString Id;
  FString Persona;
  FBotSettings Settings;
};

struct FBotRegistrationDispatchSource {
  FBotRegistrationPayloadSource Registration;
  FBotPositionPayloadSource Position;
};

struct FBotActionDispatchContext {
  FString Id;
  AActor *BotActor;
  FLevelLocalPoint InitialLocalPoint;
  FBotSettings Settings;
};

struct FBotActionDispatchRequest {
  FString ActionType;
  FBotActionDispatchContext Context;
};

struct FBotObservationSource {
  FString BotId;
  FBotSettings Settings;
  func::Maybe<FBotEntity> Bot;
  func::Maybe<FBotPositionComponent> Position;
  func::Maybe<FBotAIComponent> AI;
};

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch runtime actions for tick through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchRuntimeActionsForTick(const FBotPositionPayloadSource &Source) */
void DispatchRuntimeActionsForTick(const FBotPositionPayloadSource &Source);

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch runtime actions for registration through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchRuntimeActionsForRegistration(const FBotRegistrationDispatchSource &Source) */
void DispatchRuntimeActionsForRegistration(const FBotRegistrationDispatchSource &Source);

/** User Story: As a systems bots orchestrator consumer, I need to invoke run bot action dispatch through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn bool RunBotActionDispatch(const FBotActionDispatchRequest &Request) */
bool RunBotActionDispatch(const FBotActionDispatchRequest &Request);

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot state observation through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FString BotStateObservation(const FBotObservationSource &Source) */
FString BotStateObservation(const FBotObservationSource &Source);

} // namespace Level
} // namespace Game
} // namespace ForbocAI
