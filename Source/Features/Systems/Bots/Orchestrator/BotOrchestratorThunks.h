#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Bots/AI/BotAITypes.h"

class AActor;

namespace ForbocAI {
namespace Game {
namespace Level {

using FBotRuntimeSettings = ForbocAI::Game::Data::FBotRuntimeSettings;

struct FBotPositionPayloadSource {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  FBotRuntimeSettings Settings;
};

struct FBotRegistrationPayloadSource {
  FString Id;
  FString Persona;
  FBotRuntimeSettings Settings;
};

struct FBotRegistrationDispatchSource {
  FBotRegistrationPayloadSource Registration;
  FBotPositionPayloadSource Position;
};

struct FBotActionDispatchContext {
  FString Id;
  AActor *BotActor;
  FLevelLocalPoint InitialLocalPoint;
  FBotRuntimeSettings Settings;
};

struct FBotActionDispatchRequest {
  FString ActionType;
  FBotActionDispatchContext Context;
};

struct FBotObservationSource {
  FString BotId;
  FBotRuntimeSettings Settings;
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
