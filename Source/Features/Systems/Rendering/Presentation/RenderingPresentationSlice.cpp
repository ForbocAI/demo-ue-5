#include "Features/Systems/Rendering/Presentation/RenderingPresentationSlice.h"

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Core/fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingPresentationReducers {
namespace {

/** User Story: As a systems rendering presentation consumer, I need to invoke world feet through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn float WorldFeet(const ForbocAI::Game::Data::FGeometrySettings &Geometry, float Feet) */
float WorldFeet(const ForbocAI::Game::Data::FGeometrySettings &Geometry,
                float Feet) {
  return LevelLayoutAdapters::ActorWorldUnitsFromFeet({Geometry, Feet});
}

/** User Story: As a systems rendering presentation consumer, I need to invoke reduce world feet vector through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn FVector ReduceWorldFeetVector( const ForbocAI::Game::Data::FGeometrySettings &Geometry, const FVector &Feet) */
FVector ReduceWorldFeetVector(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry,
    const FVector &Feet) {
  return FVector(WorldFeet(Geometry, Feet.X), WorldFeet(Geometry, Feet.Y),
                 WorldFeet(Geometry, Feet.Z));
}

} // namespace

/** User Story: As a systems rendering presentation consumer, I need to invoke reduce townsperson presentation through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn FTownspersonPresentationViewModel ReduceTownspersonPresentation( const FTownspersonPresentationReduceRequest &Request) */
FTownspersonPresentationViewModel ReduceTownspersonPresentation(
    const FTownspersonPresentationReduceRequest &Request) {
  const ForbocAI::Game::Data::FTownspersonPresentationSettings &Settings =
      Request.Settings;
  const ForbocAI::Game::Data::FGeometrySettings &Geometry =
      Request.Geometry;
  const float CharacterHeight =
      WorldFeet(Geometry, Settings.Geometry.CharacterHeightFeet);
  const float PromptTextZ =
      CharacterHeight +
      WorldFeet(Geometry, Settings.Text.PromptAboveHeadFeet);
  const float NameTextZ =
      PromptTextZ + WorldFeet(Geometry, Settings.Text.NameAbovePromptFeet);

  FTownspersonPresentationViewModel Model;
  Model.Movement.WalkSpeed =
      CharacterHeight * Settings.Motion.WalkSpeedHeightRatio;
  Model.Movement.PauseDuration = Settings.Motion.PatrolPauseSeconds;
  Model.Movement.PatrolArrivalDistance =
      WorldFeet(Geometry, Settings.Geometry.CharacterShoulderWidthFeet);
  Model.Character.MannequinOffset =
      ReduceWorldFeetVector(Geometry,
                            Settings.Mannequin.MannequinOffsetFeet);
  Model.Character.MannequinRotation =
      Settings.Mannequin.MannequinRotation;
  Model.Character.MannequinScale =
      FVector(Settings.Mannequin.MannequinScale,
              Settings.Mannequin.MannequinScale,
              Settings.Mannequin.MannequinScale);
  Model.Interaction.Radius =
      LevelLayoutAdapters::TownLotWorldUnits(Geometry) *
      Settings.Motion.InteractionRadiusLots;
  Model.NameText.Location = FVector(0.0f, 0.0f, NameTextZ);
  Model.NameText.WorldSize =
      WorldFeet(Geometry, Settings.Geometry.CharacterShoulderWidthFeet);
  Model.PromptText.Location = FVector(0.0f, 0.0f, PromptTextZ);
  Model.PromptText.WorldSize =
      WorldFeet(Geometry, Settings.Geometry.CharacterShoulderWidthFeet) *
      Settings.Text.PromptTextScale;
  Model.DialogueText.Location =
      FVector(0.0f, 0.0f,
              NameTextZ +
                  CharacterHeight *
                      Settings.Text.DialogueAboveNameHeightRatio);
  Model.DialogueText.WorldSize =
      WorldFeet(Geometry, Settings.Geometry.CharacterShoulderWidthFeet) *
      Settings.Text.DialogueTextScale;
  Model.Character.MeshPath = Settings.Mannequin.MeshPath;
  Model.Character.AnimationBlueprintClassPath =
      Settings.Mannequin.AnimationBlueprintClassPath;
  return Model;
}

/** User Story: As a systems rendering presentation consumer, I need to invoke reduce horse presentation through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn FHorsePresentationViewModel ReduceHorsePresentation( const FHorsePresentationReduceRequest &Request) */
FHorsePresentationViewModel ReduceHorsePresentation(
    const FHorsePresentationReduceRequest &Request) {
  const ForbocAI::Game::Data::FHorsePresentationSettings &Settings =
      Request.Settings;
  const ForbocAI::Game::Data::FGeometrySettings &Geometry =
      Request.Geometry;
  FHorsePresentationViewModel Model;
  Model.DefaultName = Settings.Name.DefaultName;
  Model.Movement.WalkSpeed =
      WorldFeet(Geometry, Settings.Body.HorseLengthFeet) *
      Settings.Motion.WalkSpeedHorseLengthRatio;
  Model.Movement.PauseDuration = Settings.Motion.PatrolPauseSeconds;
  Model.Movement.PatrolArrivalDistance =
      WorldFeet(Geometry, Settings.Body.LegHeightFeet) *
      Settings.Motion.RouteArrivalLegRatio;
  Model.Scale.ImportedHorseScale =
      FVector(Settings.Rider.ImportedHorseScale,
              Settings.Rider.ImportedHorseScale,
              Settings.Rider.ImportedHorseScale);
  Model.Scale.MountedRiderLocation =
      ReduceWorldFeetVector(Geometry,
                            Settings.Rider.MountedRiderOffsetFeet);
  Model.Scale.MountedRiderScale =
      FVector(Settings.Rider.MountedRiderScale,
              Settings.Rider.MountedRiderScale,
              Settings.Rider.MountedRiderScale);
  Model.NameText.Location =
      FVector(0.0f, 0.0f,
              WorldFeet(Geometry,
                        Settings.Body.LegHeightFeet +
                            Settings.Body.BodyHeightFeet +
                            Settings.Body.NeckHeightFeet +
                            Settings.Body.HeadHeightFeet));
  Model.NameText.WorldSize =
      WorldFeet(Geometry, Settings.Name.NameTextWorldSizeFeet);
  Model.Assets.HorseMeshPath = Settings.Assets.HorseMeshPath;
  Model.Assets.HorseWalkAnimationPath =
      Settings.Assets.HorseWalkAnimationPath;
  Model.Assets.RiderMeshPath = Settings.Assets.RiderMeshPath;
  Model.Assets.RiderWalkAnimationPath =
      Settings.Assets.RiderWalkAnimationPath;
  return Model;
}

} // namespace RenderingPresentationReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
