#include "Features/Systems/Rendering/Presentation/PresentationSlice.h"

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Core/ue_fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingPresentationReducers {
namespace {

float WorldFeet(const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry,
                float Feet) {
  return LevelLayoutAdapters::ActorWorldUnitsFromFeet({Geometry, Feet});
}

FVector ReduceWorldFeetVector(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry,
    const FVector &Feet) {
  return FVector(WorldFeet(Geometry, Feet.X), WorldFeet(Geometry, Feet.Y),
                 WorldFeet(Geometry, Feet.Z));
}

} // namespace

FTownspersonPresentationViewModel ReduceTownspersonPresentation(
    const FTownspersonPresentationReduceRequest &Request) {
  const ForbocAI::Game::Data::FTownspersonPresentationSettings &Settings =
      Request.Settings;
  const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry =
      Request.Geometry;
  const float CharacterHeight = WorldFeet(Geometry, Settings.CharacterHeightFeet);
  const float PromptTextZ =
      CharacterHeight + WorldFeet(Geometry, Settings.PromptAboveHeadFeet);
  const float NameTextZ =
      PromptTextZ + WorldFeet(Geometry, Settings.NameAbovePromptFeet);

  FTownspersonPresentationViewModel Model;
  Model.WalkSpeed = CharacterHeight * Settings.WalkSpeedHeightRatio;
  Model.PauseDuration = Settings.PatrolPauseSeconds;
  Model.PatrolArrivalDistance =
      WorldFeet(Geometry, Settings.CharacterShoulderWidthFeet);
  Model.MannequinOffset =
      ReduceWorldFeetVector(Geometry, Settings.MannequinOffsetFeet);
  Model.MannequinRotation = Settings.MannequinRotation;
  Model.MannequinScale = FVector(Settings.MannequinScale,
                                 Settings.MannequinScale,
                                 Settings.MannequinScale);
  Model.InteractionRadius =
      LevelLayoutAdapters::TownLotWorldUnits(Geometry) *
      Settings.InteractionRadiusLots;
  Model.NameTextLocation = FVector(0.0f, 0.0f, NameTextZ);
  Model.NameTextWorldSize =
      WorldFeet(Geometry, Settings.CharacterShoulderWidthFeet);
  Model.PromptTextLocation = FVector(0.0f, 0.0f, PromptTextZ);
  Model.PromptTextWorldSize =
      WorldFeet(Geometry, Settings.CharacterShoulderWidthFeet) *
      Settings.PromptTextScale;
  Model.DialogueTextLocation =
      FVector(0.0f, 0.0f,
              NameTextZ +
                  CharacterHeight * Settings.DialogueAboveNameHeightRatio);
  Model.DialogueTextWorldSize =
      WorldFeet(Geometry, Settings.CharacterShoulderWidthFeet) *
      Settings.DialogueTextScale;
  Model.MeshPath = Settings.MeshPath;
  Model.AnimationBlueprintClassPath = Settings.AnimationBlueprintClassPath;
  return Model;
}

FHorsePresentationViewModel ReduceHorsePresentation(
    const FHorsePresentationReduceRequest &Request) {
  const ForbocAI::Game::Data::FHorsePresentationSettings &Settings =
      Request.Settings;
  const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry =
      Request.Geometry;
  FHorsePresentationViewModel Model;
  Model.DefaultName = Settings.DefaultName;
  Model.WalkSpeed =
      WorldFeet(Geometry, Settings.HorseLengthFeet) *
      Settings.WalkSpeedHorseLengthRatio;
  Model.PauseDuration = Settings.PatrolPauseSeconds;
  Model.PatrolArrivalDistance =
      WorldFeet(Geometry, Settings.LegHeightFeet) *
      Settings.RouteArrivalLegRatio;
  Model.ImportedHorseScale =
      FVector(Settings.ImportedHorseScale, Settings.ImportedHorseScale,
              Settings.ImportedHorseScale);
  Model.MountedRiderLocation =
      ReduceWorldFeetVector(Geometry, Settings.MountedRiderOffsetFeet);
  Model.MountedRiderScale =
      FVector(Settings.MountedRiderScale, Settings.MountedRiderScale,
              Settings.MountedRiderScale);
  Model.NameTextLocation =
      FVector(0.0f, 0.0f,
              WorldFeet(Geometry,
                        Settings.LegHeightFeet + Settings.BodyHeightFeet +
                            Settings.NeckHeightFeet +
                            Settings.HeadHeightFeet));
  Model.NameTextWorldSize =
      WorldFeet(Geometry, Settings.NameTextWorldSizeFeet);
  Model.HorseMeshPath = Settings.HorseMeshPath;
  Model.HorseWalkAnimationPath = Settings.HorseWalkAnimationPath;
  Model.RiderMeshPath = Settings.RiderMeshPath;
  Model.RiderWalkAnimationPath = Settings.RiderWalkAnimationPath;
  return Model;
}

} // namespace RenderingPresentationReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
