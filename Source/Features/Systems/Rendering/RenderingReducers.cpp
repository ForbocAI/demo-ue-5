#include "Features/Systems/Rendering/RenderingReducers.h"

#include "Core/ue_fp.hpp"
#include "Features/Components/Spatial/SpatialSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingReducers {
namespace {

float WorldFeet(const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry,
                float Feet) {
  return SpatialSelectors::SelectActorWorldUnitsFromFeet({Geometry, Feet});
}

FVector ReduceWorldFeetVector(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry,
    const FVector &Feet) {
  return FVector(WorldFeet(Geometry, Feet.X), WorldFeet(Geometry, Feet.Y),
                 WorldFeet(Geometry, Feet.Z));
}

inline FString RenderingReducerAtom(const char *Atom) {
  return FString(UTF8_TO_TCHAR(Atom));
}

template <typename Output> struct TRenderingTextValueDeclaration {
  FString Text;
  Output Value;

  TRenderingTextValueDeclaration(const char *InText, Output InValue)
      : Text(RenderingReducerAtom(InText)), Value(InValue) {}
};

template <typename Output> struct TRenderingTextValueRegistry;

template <> struct TRenderingTextValueRegistry<ELevelRetroTexture> {
  static const TArray<TRenderingTextValueDeclaration<ELevelRetroTexture>>
      &Values() {
    static const TArray<TRenderingTextValueDeclaration<ELevelRetroTexture>>
        RegisteredValues = {
            {"terrain_ortho", ELevelRetroTexture::TerrainOrtho},
            {"building_timber", ELevelRetroTexture::BuildingTimber},
            {"road_dust", ELevelRetroTexture::RoadDust},
            {"water_creek", ELevelRetroTexture::WaterCreek},
            {"foliage_riparian", ELevelRetroTexture::FoliageRiparian},
            {"rock_granite", ELevelRetroTexture::RockGranite},
            {"mine_timber", ELevelRetroTexture::MineTimber},
            {"marker_paint", ELevelRetroTexture::MarkerPaint},
            {"npc_body", ELevelRetroTexture::NpcBody},
            {"npc_hat", ELevelRetroTexture::NpcHat},
            {"horse_coat", ELevelRetroTexture::HorseCoat},
            {"horse_leg", ELevelRetroTexture::HorseLeg},
            {"horse_tack", ELevelRetroTexture::HorseTack}};
    return RegisteredValues;
  }
};

template <typename Output>
func::Maybe<Output> ReadRenderingTextValue(const FString &Text) {
  const FString LowerText = Text.ToLower();
  const TArray<TRenderingTextValueDeclaration<Output>> &Values =
      TRenderingTextValueRegistry<Output>::Values();
  return func::fold_array<TRenderingTextValueDeclaration<Output>,
                          func::Maybe<Output>>(
      Values, func::nothing<Output>(),
      [LowerText](const func::Maybe<Output> &Current,
                  const TRenderingTextValueDeclaration<Output> &Declaration) {
        return Current.hasValue || Declaration.Text != LowerText
                   ? Current
                   : func::just(Declaration.Value);
      });
}

template <typename Output> Output RequireRenderingTextValue(const FString &Text) {
  const func::Maybe<Output> Resolved = ReadRenderingTextValue<Output>(Text);
  check(Resolved.hasValue);
  return Resolved.value;
}

template <typename Item, typename Predicate>
Item RequireRenderingCatalogItem(const TArray<Item> &Catalog,
                                 Predicate Matches) {
  const func::Maybe<Item> Found = func::find_array<Item>(Catalog, Matches);
  check(Found.hasValue);
  return Found.value;
}

FLevelRetroTextureSpec ReduceTextureCatalogItem(
    const ForbocAI::Game::Data::FRenderingTextureSpecSettings &Settings) {
  return {ReduceTextureKind(Settings.Texture), Settings.Name, Settings.Use,
          FIntPoint(Settings.Size, Settings.Size)};
}

FLevelDistanceLodStage ReduceDistanceLodStageSettings(
    const ForbocAI::Game::Data::FRenderingDistanceLodStageSettings &Settings) {
  return {Settings.Id,
          Settings.MaxDistance,
          Settings.StaticMeshForcedLodModel,
          Settings.SkeletalMeshForcedLodModel,
          Settings.SkeletalMeshMinLodModel,
          Settings.CullDistance,
          Settings.ActorTickIntervalSeconds,
          Settings.bStaticVisible,
          Settings.bDynamicVisible,
          Settings.bLabelsVisible,
          Settings.bAnimated,
          Settings.bUpdateRateOptimizationsEnabled,
          Settings.bPatrolEnabled,
          Settings.bCollisionEnabled,
          Settings.bCastShadow};
}

bool DistanceWithinStage(float Distance,
                         const FLevelDistanceLodStage &Stage) {
  return Distance <= Stage.MaxDistance;
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
  return RequireRenderingTextValue<ELevelRetroTexture>(Texture);
}

FLevelRetroRenderProfile ReduceRuntimeProfile(
    const ForbocAI::Game::Data::FRenderingProfileSettings &Settings) {
  return {Settings.TimeOfDayHour,
          Settings.AntiAliasingMethod,
          Settings.PostProcessAAQuality,
          Settings.ScreenPercentage,
          Settings.MinimumScreenPercentage,
          Settings.InternalRenderWidth,
          Settings.InternalRenderHeight,
          Settings.OutputScaleMultiplier,
          Settings.bFullscreenOutput,
          Settings.ViewDistanceScale,
          Settings.FoliageDensityScale,
          Settings.GrassDensityScale,
          Settings.SunPitchDegrees,
          Settings.SunYawDegrees,
          Settings.SunRollDegrees,
          Settings.DirectionalLightIntensity,
          Settings.DirectionalLightSourceAngle,
          Settings.DirectionalLightColorR,
          Settings.DirectionalLightColorG,
          Settings.DirectionalLightColorB,
          Settings.DirectionalLightColorA,
          Settings.ShadowCascades,
          Settings.ShadowMaxResolution,
          Settings.PostProcessPriority,
          Settings.PostProcessBlendRadius,
          Settings.PostProcessBlendWeight,
          Settings.PostProcessSaturationMultiplier,
          Settings.PostProcessContrastMultiplier,
          Settings.PostProcessGainR,
          Settings.PostProcessGainG,
          Settings.PostProcessGainB,
          Settings.PostProcessGainA,
          Settings.PostProcessSceneTintR,
          Settings.PostProcessSceneTintG,
          Settings.PostProcessSceneTintB,
          Settings.PostProcessSceneTintA,
          Settings.SkyAtmosphereSkyLuminanceR,
          Settings.SkyAtmosphereSkyLuminanceG,
          Settings.SkyAtmosphereSkyLuminanceB,
          Settings.SkyAtmosphereSkyLuminanceA,
          Settings.SkyAtmosphereAerialLuminanceR,
          Settings.SkyAtmosphereAerialLuminanceG,
          Settings.SkyAtmosphereAerialLuminanceB,
          Settings.SkyAtmosphereAerialLuminanceA,
          Settings.SkyAtmosphereHeightFogContribution,
          Settings.SkyLightIntensity,
          Settings.SkyLightColorR,
          Settings.SkyLightColorG,
          Settings.SkyLightColorB,
          Settings.SkyLightColorA,
          Settings.bSkyLightRealTimeCapture,
          Settings.bSkyDomeEnabled,
          Settings.SkyDomeMeshPath,
          Settings.SkyDomeMaterialPath,
          Settings.SkyDomeScale,
          Settings.SkyDomeZOffset,
          Settings.SkyDomeSkyBrightness,
          Settings.SkyDomeCloudBrightness,
          Settings.SkyDomeCloudDarkness,
          Settings.SkyDomeRimBrightness,
          Settings.SkyDomeStarColorR,
          Settings.SkyDomeStarColorG,
          Settings.SkyDomeStarColorB,
          Settings.SkyDomeStarColorA,
          Settings.SkyDomeHorizonColorR,
          Settings.SkyDomeHorizonColorG,
          Settings.SkyDomeHorizonColorB,
          Settings.SkyDomeHorizonColorA,
          Settings.SkyDomeZenithColorR,
          Settings.SkyDomeZenithColorG,
          Settings.SkyDomeZenithColorB,
          Settings.SkyDomeZenithColorA,
          Settings.bMoonDiscEnabled,
          Settings.MoonDiscMeshPath,
          Settings.MoonDiscMaterialPath,
          Settings.MoonDiscDistance,
          Settings.MoonDiscScale,
          Settings.MoonDiscPitchDegrees,
          Settings.MoonDiscYawDegrees,
          Settings.MoonDiscRollDegrees,
          Settings.MoonDiscColorR,
          Settings.MoonDiscColorG,
          Settings.MoonDiscColorB,
          Settings.MoonDiscColorA,
          Settings.MoonPixelGridSize,
          Settings.MoonDiscWorldUnitsPerScale,
          Settings.MoonPixelOverlapMultiplier,
          Settings.PointStarCount,
          Settings.PointStarYawHashSalt,
          Settings.PointStarYawSpanDegrees,
          Settings.PointStarPitchHashSalt,
          Settings.PointStarPitchMinDegrees,
          Settings.PointStarPitchSpanDegrees,
          Settings.PointStarDistanceMultiplier,
          Settings.PointStarSizeHashSalt,
          Settings.PointStarWorldSizeMin,
          Settings.PointStarWorldSizeJitter,
          Settings.bFogEnabled,
          Settings.bVolumetricFogEnabled,
          Settings.FogDensity,
          Settings.FogHeightFalloff,
          Settings.FogStartDistance,
          Settings.FogCutoffDistance,
          Settings.FogMaxOpacity,
          Settings.FogColorR,
          Settings.FogColorG,
          Settings.FogColorB,
          Settings.FogColorA};
}

TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(
    const TArray<ForbocAI::Game::Data::FRenderingTextureSpecSettings>
        &Settings) {
  return func::map_array<
      ForbocAI::Game::Data::FRenderingTextureSpecSettings,
      FLevelRetroTextureSpec>(Settings, ReduceTextureCatalogItem);
}

TArray<FLevelDistanceLodStage> ReduceDistanceLodStages(
    const ForbocAI::Game::Data::FRenderingDistanceLodSettings &Settings) {
  return func::map_array<
      ForbocAI::Game::Data::FRenderingDistanceLodStageSettings,
      FLevelDistanceLodStage>(Settings.Stages, ReduceDistanceLodStageSettings);
}

FLevelDistanceLodStage
ReduceDistanceLodStage(const FLevelDistanceLodStageRequest &Request) {
  check(!Request.Stages.IsEmpty());
  const float Distance = FVector::Dist2D(Request.Origin, Request.Location);
  const func::Maybe<FLevelDistanceLodStage> Stage =
      func::find_array<FLevelDistanceLodStage>(
          Request.Stages, [Distance](const FLevelDistanceLodStage &Candidate) {
            return DistanceWithinStage(Distance, Candidate);
          });
  check(Stage.hasValue);
  return Stage.value;
}

FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings) {
  return {Settings.LevelCubeMeshPath, Settings.BlockoutMaterialPath};
}

FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request) {
  check(!Request.Catalog.IsEmpty());
  return RequireRenderingCatalogItem<FLevelRetroTextureSpec>(
      Request.Catalog, [&Request](const FLevelRetroTextureSpec &Spec) {
        return Spec.Texture == Request.Texture;
      });
}

FRenderingPayload ReduceRenderingPayload(
    const FRenderingPayloadRequest &Request) {
  return {Request.Id, Request.RuntimeProfile, Request.TextureCatalog,
          Request.DistanceLodStages, Request.RuntimeSettings};
}

FRenderingState ReduceRenderingProfileObserved(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.RuntimeProfile = Action.PayloadValue.RuntimeProfile;
            Next.TextureCatalog = Action.PayloadValue.TextureCatalog;
            Next.DistanceLodStages = Action.PayloadValue.DistanceLodStages;
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

} // namespace RenderingReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
