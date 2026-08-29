#pragma once

#include "Features/Components/Data/Settings/Bot/Configuration/Commands/CommandsTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Behavior/ConfigurationBehaviorTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Diagnostics/ConfigurationDiagnosticsTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Goals/ConfigurationGoalsTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Lifecycle/ConfigurationLifecycleTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Movement/MovementTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Awareness/AwarenessTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Health/HealthTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Projection/ConfigurationProjectionTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Schedule/ScheduleTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Spawn/ConfigurationSpawnTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/Resources/ResourcesTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/WorldSense/WorldSenseTypes.h"
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
