#include "Features/Systems/Rendering/RenderingReducers.h"

#include "Core/ecs.hpp"
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

func::Maybe<ELevelRetroTexture> ResolveTextureKind(const FString &Texture) {
  return func::multi_match<FString, ELevelRetroTexture>(
      Texture,
      {
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("terrain_ortho")),
              [](const FString &) { return ELevelRetroTexture::TerrainOrtho; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("building_timber")),
              [](const FString &) { return ELevelRetroTexture::BuildingTimber; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("road_dust")),
              [](const FString &) { return ELevelRetroTexture::RoadDust; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("water_creek")),
              [](const FString &) { return ELevelRetroTexture::WaterCreek; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("foliage_riparian")),
              [](const FString &) {
                return ELevelRetroTexture::FoliageRiparian;
              }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("rock_granite")),
              [](const FString &) { return ELevelRetroTexture::RockGranite; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("mine_timber")),
              [](const FString &) { return ELevelRetroTexture::MineTimber; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("marker_paint")),
              [](const FString &) { return ELevelRetroTexture::MarkerPaint; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("npc_body")),
              [](const FString &) { return ELevelRetroTexture::NpcBody; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("npc_hat")),
              [](const FString &) { return ELevelRetroTexture::NpcHat; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("horse_coat")),
              [](const FString &) { return ELevelRetroTexture::HorseCoat; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("horse_leg")),
              [](const FString &) { return ELevelRetroTexture::HorseLeg; }),
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("horse_tack")),
              [](const FString &) { return ELevelRetroTexture::HorseTack; }),
      });
}

FLevelRetroTextureSpec ReduceTextureCatalogItem(
    const ForbocAI::Demo::Data::FRenderingTextureSpecSettings &Settings) {
  return {ReduceTextureKind(Settings.Texture), Settings.Name, Settings.Use,
          FIntPoint(Settings.Size, Settings.Size)};
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

ELevelRetroTexture ReduceTextureKind(const FString &Texture) {
  const func::Maybe<ELevelRetroTexture> Resolved = ResolveTextureKind(Texture);
  check(Resolved.hasValue);
  return Resolved.value;
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
  return func::fold_indexed(
      Settings, static_cast<size_t>(Settings.Num()),
      TArray<FLevelRetroTextureSpec>(),
      [](const TArray<FLevelRetroTextureSpec> &Acc,
         const ForbocAI::Demo::Data::FRenderingTextureSpecSettings &Item) {
        TArray<FLevelRetroTextureSpec> Next = Acc;
        Next.Add(ReduceTextureCatalogItem(Item));
        return Next;
      });
}

FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Demo::Data::FRenderingAssetPathSettings &Settings) {
  return {Settings.LevelCubeMeshPath, Settings.BlockoutMaterialPath};
}

FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request) {
  check(!Request.Catalog.IsEmpty());
  const func::Maybe<FLevelRetroTextureSpec> Found = func::find_indexed(
      Request.Catalog, static_cast<size_t>(Request.Catalog.Num()),
      [&Request](const FLevelRetroTextureSpec &Spec) {
        return Spec.Texture == Request.Texture;
      });
  check(Found.hasValue);
  return Found.value;
}

FRenderingPayload ReduceRenderingPayload(
    const FRenderingPayloadRequest &Request) {
  return {Request.Id, Request.RuntimeProfile, Request.TextureCatalog,
          Request.RuntimeSettings};
}

FRenderingState ReduceRenderingProfileObserved(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.RuntimeProfile = Action.PayloadValue.RuntimeProfile;
            Next.TextureCatalog = Action.PayloadValue.TextureCatalog;
            Next.RuntimeSettings = Action.PayloadValue.RuntimeSettings;
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
