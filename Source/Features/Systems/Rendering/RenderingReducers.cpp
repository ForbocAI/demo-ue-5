#include "Features/Systems/Rendering/RenderingReducers.h"

#include "Features/Components/Spatial/SpatialSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingReducers {
namespace {

constexpr int32 BlockTextureSize = 16;
constexpr int32 CharacterTextureSize = 16;

struct FRetroTextureSpecSource {
  ELevelRetroTexture Texture;
  const TCHAR *Name;
  const TCHAR *Use;
  int32 Size;
};

float WorldFeet(float Feet) {
  return SpatialSelectors::SelectActorWorldUnitsFromFeet(Feet);
}

FLevelRetroTextureSpec ReduceTextureSpecSource(
    const FRetroTextureSpecSource &Source) {
  return {Source.Texture, FString(Source.Name), FString(Source.Use),
          FIntPoint(Source.Size, Source.Size)};
}

FLevelRetroTextureSpec ReduceTextureSpecAtIndex(
    const FRenderingTextureSpecRequest &Request, int32 Index) {
  return Index >= Request.Catalog.Num()
             ? Request.Catalog[0]
             : (Request.Catalog[Index].Texture == Request.Texture
                    ? Request.Catalog[Index]
                    : ReduceTextureSpecAtIndex(Request, Index + 1));
}

FTownspersonPresentationViewModel ReduceTownspersonPresentationValues() {
  constexpr float CharacterHeightFeet = 5.5f;
  constexpr float CharacterShoulderWidthFeet = 1.75f;
  constexpr float PatrolPauseSeconds = 1.6f;
  constexpr float PromptAboveHeadFeet = 1.15f;
  constexpr float NameAbovePromptFeet = 1.75f;
  constexpr float DialogueAboveNameHeightRatio = 0.41f;
  constexpr float InteractionRadiusLots = 0.9f;
  constexpr float WalkSpeedHeightRatio = 0.86f;
  constexpr float MannequinScale = 0.82f;

  const float CharacterHeight = WorldFeet(CharacterHeightFeet);
  const float PromptTextZ =
      CharacterHeight + WorldFeet(PromptAboveHeadFeet);
  const float NameTextZ = PromptTextZ + WorldFeet(NameAbovePromptFeet);

  FTownspersonPresentationViewModel Model;
  Model.WalkSpeed = CharacterHeight * WalkSpeedHeightRatio;
  Model.PauseDuration = PatrolPauseSeconds;
  Model.PatrolArrivalDistance = WorldFeet(CharacterShoulderWidthFeet);
  Model.MannequinOffset = FVector(0.0f, 0.0f, -WorldFeet(0.18f));
  Model.MannequinRotation = FRotator(0.0f, -90.0f, 0.0f);
  Model.MannequinScale =
      FVector(MannequinScale, MannequinScale, MannequinScale);
  Model.InteractionRadius =
      SpatialSelectors::SelectTownLotWorldUnits() * InteractionRadiusLots;
  Model.NameTextLocation = FVector(0.0f, 0.0f, NameTextZ);
  Model.NameTextWorldSize = WorldFeet(CharacterShoulderWidthFeet);
  Model.PromptTextLocation = FVector(0.0f, 0.0f, PromptTextZ);
  Model.PromptTextWorldSize = WorldFeet(CharacterShoulderWidthFeet) * 0.82f;
  Model.InteractionPrompt = TEXT("Press E to talk");
  Model.DialogueTextLocation =
      FVector(0.0f, 0.0f,
              NameTextZ + CharacterHeight * DialogueAboveNameHeightRatio);
  Model.DialogueTextWorldSize = WorldFeet(CharacterShoulderWidthFeet) * 0.7f;
  return Model;
}

FHorsePresentationViewModel ReduceHorsePresentationValues() {
  constexpr float HorseLengthFeet = 8.0f;
  constexpr float BodyHeightFeet = 2.0f;
  constexpr float LegHeightFeet = 3.25f;
  constexpr float NeckHeightFeet = 1.65f;
  constexpr float HeadHeightFeet = 0.95f;
  constexpr float SaddleHeightFeet = 0.28f;
  constexpr float HorsePatrolPauseSeconds = 1.4f;
  constexpr float WalkSpeedHorseLengthRatio = 0.58f;
  constexpr float RouteArrivalLegRatio = 0.8f;
  constexpr float ImportedHorseScale = 0.62f;
  constexpr float MountedRiderScale = 0.62f;

  FHorsePresentationViewModel Model;
  Model.WalkSpeed = WorldFeet(HorseLengthFeet) * WalkSpeedHorseLengthRatio;
  Model.PauseDuration = HorsePatrolPauseSeconds;
  Model.PatrolArrivalDistance = WorldFeet(LegHeightFeet) * RouteArrivalLegRatio;
  Model.ImportedHorseScale =
      FVector(ImportedHorseScale, ImportedHorseScale, ImportedHorseScale);
  Model.MountedRiderLocation =
      FVector(0.0f, 0.0f,
              WorldFeet(LegHeightFeet + BodyHeightFeet +
                        SaddleHeightFeet * 0.5f));
  Model.MountedRiderScale =
      FVector(MountedRiderScale, MountedRiderScale, MountedRiderScale);
  Model.NameTextLocation =
      FVector(0.0f, 0.0f,
              WorldFeet(LegHeightFeet + BodyHeightFeet + NeckHeightFeet +
                        HeadHeightFeet));
  Model.NameTextWorldSize = WorldFeet(1.35f);
  return Model;
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

const FLevelRetroRenderProfile &ReduceRuntimeProfileDefaults() {
  static const func::Lazy<FLevelRetroRenderProfile> Profile =
      func::lazy([]() -> FLevelRetroRenderProfile {
        return {15.0f, 0,     0,     55.0f, 1.65f, 0.85f, 0.85f,
                -56.0f, -64.0f, 8.5f, 0.08f, 1,     512};
      });
  return func::eval(Profile);
}

TArray<FLevelRetroTextureSpec> ReduceTextureCatalogDefaults() {
  return {
      ReduceTextureSpecSource(
          {ELevelRetroTexture::TerrainOrtho, TEXT("terrain-ortho-dither"),
           TEXT("dithered terrain color from the French Gulch ortho sample"),
           BlockTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::BuildingTimber, TEXT("building-timber"),
           TEXT("false-front buildings, cabins, porch boards, and hotel/store silhouettes"),
           BlockTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::RoadDust, TEXT("road-dust"),
           TEXT("Main Street, French Gulch Road, wagon yard, and mine road"),
           BlockTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::WaterCreek, TEXT("clear-creek-water"),
           TEXT("Clear Creek ribbon, shallow bends, and future Water System guide surfaces"),
           BlockTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::FoliageRiparian, TEXT("riparian-foliage"),
           TEXT("cottonwoods, willow shrubs, chaparral, and PCG foliage markers"),
           BlockTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::RockGranite, TEXT("creek-granite-rock"),
           TEXT("boulders, creek stone shelves, cemetery terrace, and hillside rock scatter"),
           BlockTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::MineTimber, TEXT("mine-dark-timber"),
           TEXT("mine entrance, timber supports, and waste-rock edge pieces"),
           BlockTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::MarkerPaint, TEXT("prototype-marker-paint"),
           TEXT("PCG/Water staging markers and remaining non-final blockout markers"),
           BlockTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::NpcBody, TEXT("npc-work-cloth"),
           TEXT("primitive townspeople clothing"), CharacterTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::NpcHat, TEXT("npc-worn-hat"),
           TEXT("primitive townspeople hats"), CharacterTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::HorseCoat, TEXT("horse-coat"),
           TEXT("walking horse body, neck, and head primitives"),
           CharacterTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::HorseLeg, TEXT("horse-leg-dark"),
           TEXT("walking horse legs and tail primitives"),
           CharacterTextureSize}),
      ReduceTextureSpecSource(
          {ELevelRetroTexture::HorseTack, TEXT("horse-tack-saddle"),
           TEXT("walking horse saddle and tack primitives"),
           CharacterTextureSize}),
  };
}

FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request) {
  return Request.Catalog.IsEmpty()
             ? FLevelRetroTextureSpec()
             : ReduceTextureSpecAtIndex(Request, 0);
}

FLevelRetroTextureSpec
ReduceDefaultTextureSpec(const FRenderingDefaultTextureSpecRequest &Request) {
  return ReduceTextureSpec({Request.Texture, ReduceTextureCatalogDefaults()});
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
            Next.TownspersonPresentation =
                ReduceTownspersonPresentation(Action.PayloadValue);
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
            Next.HorsePresentation =
                ReduceHorsePresentation(Action.PayloadValue);
            return Next;
          })
      .val;
}

FTownspersonPresentationViewModel ReduceTownspersonPresentation(
    const FRenderingPresentationRequest &Request) {
  (void)Request;
  return ReduceTownspersonPresentationValues();
}

FHorsePresentationViewModel ReduceHorsePresentation(
    const FRenderingPresentationRequest &Request) {
  (void)Request;
  return ReduceHorsePresentationValues();
}

} // namespace RenderingReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
