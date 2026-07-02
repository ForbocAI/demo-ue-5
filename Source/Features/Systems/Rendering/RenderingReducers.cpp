#include "Features/Systems/Rendering/RenderingReducers.h"

#include "Core/ue_fp.hpp"
#include "Features/Components/Spatial/SpatialSelectors.h"

#include <initializer_list>

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

inline FString RenderingReducerAtom(const char *Atom) {
  return FString(UTF8_TO_TCHAR(Atom));
}

struct FTextureKindDeclaration {
  FString Name;
  ELevelRetroTexture Texture;

  FTextureKindDeclaration(const char *InName, ELevelRetroTexture InTexture)
      : Name(RenderingReducerAtom(InName)), Texture(InTexture) {}
};

func::Maybe<ELevelRetroTexture> TextureKindFromCatalog(
    const FString &Texture,
    std::initializer_list<FTextureKindDeclaration> Declarations) {
  const TArray<FTextureKindDeclaration> DeclarationList(Declarations);
  return func::fold_array<FTextureKindDeclaration,
                          func::Maybe<ELevelRetroTexture>>(
      DeclarationList, func::nothing<ELevelRetroTexture>(),
      [&Texture](const func::Maybe<ELevelRetroTexture> &Acc,
                 const FTextureKindDeclaration &Declaration) {
        return Acc.hasValue || Declaration.Name != Texture
                   ? Acc
                   : func::just(Declaration.Texture);
      });
}

func::Maybe<ELevelRetroTexture> ResolveTextureKind(const FString &Texture) {
  return TextureKindFromCatalog(
      Texture, {{"terrain_ortho", ELevelRetroTexture::TerrainOrtho},
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
                {"horse_tack", ELevelRetroTexture::HorseTack}});
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
  return func::map_array<
      ForbocAI::Demo::Data::FRenderingTextureSpecSettings,
      FLevelRetroTextureSpec>(Settings, ReduceTextureCatalogItem);
}

FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Demo::Data::FRenderingAssetPathSettings &Settings) {
  return {Settings.LevelCubeMeshPath, Settings.BlockoutMaterialPath};
}

FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request) {
  check(!Request.Catalog.IsEmpty());
  const func::Maybe<FLevelRetroTextureSpec> Found =
      func::find_array<FLevelRetroTextureSpec>(
          Request.Catalog, [&Request](const FLevelRetroTextureSpec &Spec) {
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
