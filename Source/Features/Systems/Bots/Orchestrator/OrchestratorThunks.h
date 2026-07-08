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

void DispatchRuntimeActionsForTick(const FBotPositionPayloadSource &Source);

void DispatchRuntimeActionsForRegistration(const FBotRegistrationDispatchSource &Source);

bool RunBotActionDispatch(const FBotActionDispatchRequest &Request);

FString BotStateObservation(const FBotObservationSource &Source);

} // namespace Level
} // namespace Game
} // namespace ForbocAI
