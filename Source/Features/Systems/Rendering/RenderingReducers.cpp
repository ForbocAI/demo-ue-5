#include "Features/Systems/Rendering/RenderingReducers.h"

#include "Features/Components/Data/DataAdapters.h"
#include "Features/Components/Spatial/SpatialSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingReducers {
namespace {

float WorldFeet(const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry,
                float Feet) {
  return SpatialSelectors::SelectActorWorldUnitsFromFeet({Geometry, Feet});
}

FVector ReduceWorldFeetVector(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry,
    const FVector &Feet) {
  return FVector(WorldFeet(Geometry, Feet.X), WorldFeet(Geometry, Feet.Y),
                 WorldFeet(Geometry, Feet.Z));
}

ELevelRetroTexture ReduceTextureKind(const FString &Texture) {
  return Texture == TEXT("terrain_ortho")
             ? ELevelRetroTexture::TerrainOrtho
             : Texture == TEXT("building_timber")
                   ? ELevelRetroTexture::BuildingTimber
                   : Texture == TEXT("road_dust")
                         ? ELevelRetroTexture::RoadDust
                         : Texture == TEXT("water_creek")
                               ? ELevelRetroTexture::WaterCreek
                               : Texture == TEXT("foliage_riparian")
                                     ? ELevelRetroTexture::FoliageRiparian
                                     : Texture == TEXT("rock_granite")
                                           ? ELevelRetroTexture::RockGranite
                                           : Texture == TEXT("mine_timber")
                                                 ? ELevelRetroTexture::MineTimber
                                                 : Texture ==
                                                           TEXT("marker_paint")
                                                       ? ELevelRetroTexture::
                                                             MarkerPaint
                                                       : Texture ==
                                                                 TEXT("npc_body")
                                                             ? ELevelRetroTexture::
                                                                   NpcBody
                                                             : Texture ==
                                                                       TEXT("npc_hat")
                                                                   ? ELevelRetroTexture::
                                                                         NpcHat
                                                                   : Texture ==
                                                                             TEXT("horse_coat")
                                                                         ? ELevelRetroTexture::
                                                                               HorseCoat
                                                                         : Texture ==
                                                                                   TEXT("horse_leg")
                                                                               ? ELevelRetroTexture::
                                                                                     HorseLeg
                                                                               : ELevelRetroTexture::
                                                                                     HorseTack;
}

FLevelRetroTextureSpec ReduceTextureCatalogItem(
    const ForbocAI::Demo::Data::FRenderingTextureSpecSettings &Settings) {
  return {ReduceTextureKind(Settings.Texture), Settings.Name, Settings.Use,
          FIntPoint(Settings.Size, Settings.Size)};
}

FLevelRetroTextureSpec ReduceTextureSpecAtIndex(
    const FRenderingTextureSpecRequest &Request, int32 Index) {
  return Index >= Request.Catalog.Num()
             ? Request.Catalog[0]
             : (Request.Catalog[Index].Texture == Request.Texture
                    ? Request.Catalog[Index]
                    : ReduceTextureSpecAtIndex(Request, Index + 1));
}

} // namespace

FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input) {
  return (func::pipe(Input) |
          [](FLevelRetroTextureApply Value) -> FLevelRetroTextureApply {
    return Value;
  })
      .val;
}

FLevelRetroRenderProfile ReduceRuntimeProfile(
    const ForbocAI::Demo::Data::FRenderingProfileSettings &Settings) {
  return {Settings.TimeOfDayHour,
          Settings.AntiAliasingMethod,
          Settings.PostProcessAAQuality,
          Settings.ScreenPercentage,
          Settings.ViewDistanceScale,
          Settings.FoliageDensityScale,
          Settings.GrassDensityScale,
          Settings.SunPitchDegrees,
          Settings.SunYawDegrees,
          Settings.DirectionalLightIntensity,
          Settings.DirectionalLightSourceAngle,
          Settings.ShadowCascades,
          Settings.ShadowMaxResolution};
}

TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(
    const TArray<ForbocAI::Demo::Data::FRenderingTextureSpecSettings>
        &Settings) {
  return ForbocAI::Demo::Data::DataAdapters::MapArray<
      ForbocAI::Demo::Data::FRenderingTextureSpecSettings,
      FLevelRetroTextureSpec>({Settings, ReduceTextureCatalogItem});
}

FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Demo::Data::FRenderingAssetPathSettings &Settings) {
  return {Settings.LevelCubeMeshPath, Settings.BlockoutMaterialPath};
}

FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request) {
  return Request.Catalog.IsEmpty()
             ? FLevelRetroTextureSpec()
             : ReduceTextureSpecAtIndex(Request, 0);
}

FRenderingState ReduceRenderingProfileObserved(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.RuntimeProfile = Action.PayloadValue.RuntimeProfile;
            Next.TextureCatalog = Action.PayloadValue.TextureCatalog;
            Next.bReady = true;
            return Next;
          })
      .val;
}

FRenderingState ReduceTownspersonPresentationRequested(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPresentationRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            return Next;
          })
      .val;
}

FRenderingState ReduceHorsePresentationRequested(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPresentationRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            return Next;
          })
      .val;
}

FTownspersonPresentationViewModel ReduceTownspersonPresentation(
    const FTownspersonPresentationReduceRequest &Request) {
  const ForbocAI::Demo::Data::FTownspersonPresentationSettings &Settings =
      Request.Settings;
  const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry =
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
      SpatialSelectors::SelectTownLotWorldUnits(Geometry) *
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
  const ForbocAI::Demo::Data::FHorsePresentationSettings &Settings =
      Request.Settings;
  const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry =
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
      FVector(0.0f, 0.0f,
              WorldFeet(Geometry,
                        Settings.LegHeightFeet + Settings.BodyHeightFeet +
                            Settings.SaddleHeightFeet * 0.5f));
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

} // namespace RenderingReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
