#pragma once

#include "Features/Components/Data/RuntimeSettings/BotSettingsTypes.h"
#include "Features/Components/Data/RuntimeSettings/DialogueSettingsTypes.h"
#include "Features/Components/Data/RuntimeSettings/InteractionSettingsTypes.h"
#include "Features/Components/Data/RuntimeSettings/LevelSettingsTypes.h"
#include "Features/Components/Data/RuntimeSettings/PlayerSettingsTypes.h"
#include "Features/Components/Data/RuntimeSettings/RenderingSettingsTypes.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsTypes.h"
#include "Features/Components/Data/RuntimeSettings/SpeechSettingsTypes.h"
#include "Features/Components/Data/RuntimeSettings/UISettingsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {

struct FDemoRuntimeSettings {
  FPlayerPresentationSettings PlayerPresentation;
  FInteractionSettings Interaction;
  FTownspersonDefaultsSettings TownspersonDefaults;
  FLevelTerrainSourceSettings LevelTerrainSources;
  FLevelDataSourceSettings LevelDataSources;
  FRuntimeValidationSettings RuntimeValidation;
  FLevelGeometrySettings LevelGeometry;
  FRenderingAssetPathSettings RenderingAssets;
  FRenderingProfileSettings RenderingProfile;
  TArray<FRenderingTextureSpecSettings> TextureCatalog;
  FRenderingRuntimeSettings RenderingRuntime;
  FDialogueRuntimeSettings DialogueRuntime;
  FBotRuntimeSettings BotRuntime;
  FUIRuntimeSettings UIRuntime;
  FSpeechRuntimeSettings SpeechRuntime;
  FTownspersonPresentationSettings TownspersonPresentation;
  FHorsePresentationSettings HorsePresentation;
  FRuntimeObservationIdSettings RuntimeObservationIds;
  FRuntimeDebugMessageSettings RuntimeDebugMessages;
  FRuntimeViewNameSettings RuntimeViewNames;
  FRuntimeTextSettings RuntimeText;
};

} // namespace Data
} // namespace Demo
} // namespace ForbocAI
