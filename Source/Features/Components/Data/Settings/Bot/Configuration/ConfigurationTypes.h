#pragma once

#include "Features/Components/Data/Settings/Bot/Configuration/Actions/BotActionTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Behavior/BotBehaviorTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Diagnostics/BotDiagnosticTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Goals/BotGoalTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Lifecycle/BotLifecycleTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Movement/BotMovementTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Awareness/BotAwarenessTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Health/BotHealthTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Projection/BotProjectionTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Schedule/BotScheduleTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Spawn/BotSpawnTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Resources/BotResourceTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/WorldSense/BotWorldSenseTypes.h"
#include "Features/Components/Data/Settings/Bot/Stats/BotStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotSettings {
  FBotHealthSettings Health;
  FBotResourceSettings Resources;
  FBotSpawnSettings Spawn;
  FBotAwarenessSettings Awareness;
  FBotWorldSenseSettings WorldSense;
  FBotBehaviorSettings Behavior;
  FBotMovementSettings Movement;
  FBotGoalSettings Goals;
  FBotScheduleSettings Schedule;
  FBotLifecycleSettings Lifecycle;
  FBotProjectionSettings Projection;
  FBotDiagnosticSettings Diagnostics;
  FBotActionSettings Actions;
  FStatPresetSettings TownspersonStats;
  FStatPresetSettings HorseStats;
};

/** User Story: As a bot runtime consumer, I need to compare values for equality through a stable signature so runtime settings remain explicit and composable. @fn inline bool operator==(const FBotSettings &Left, const FBotSettings &Right) */
inline bool operator==(const FBotSettings &Left,
                       const FBotSettings &Right) {
  return Left.Health == Right.Health && Left.Resources == Right.Resources &&
         Left.Spawn == Right.Spawn && Left.Awareness == Right.Awareness &&
         Left.WorldSense == Right.WorldSense &&
         Left.Behavior == Right.Behavior &&
         Left.Movement == Right.Movement && Left.Goals == Right.Goals &&
         Left.Schedule == Right.Schedule &&
         Left.Lifecycle == Right.Lifecycle &&
         Left.Projection == Right.Projection &&
         Left.Diagnostics == Right.Diagnostics &&
         Left.Actions == Right.Actions &&
         Left.TownspersonStats == Right.TownspersonStats &&
         Left.HorseStats == Right.HorseStats;
}

/** User Story: As a bot runtime consumer, I need to compare values for inequality through a stable signature so runtime settings remain explicit and composable. @fn inline bool operator!=(const FBotSettings &Left, const FBotSettings &Right) */
inline bool operator!=(const FBotSettings &Left,
                       const FBotSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
