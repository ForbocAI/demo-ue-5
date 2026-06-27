#include "Features/Systems/Level/LevelRuntimeAdapters.h"

#include "Dom/JsonObject.h"
#include "Features/Components/Data/DataAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelSystemAdapters {

namespace DataAdapters = ForbocAI::Demo::Data::DataAdapters;

namespace {

constexpr const TCHAR *RuntimeLevelDataPath =
    TEXT("Data/french_gulch_runtime_level.json");

ELevelRuntimeScaleMode ScaleModeFromJson(const FString &Mode) {
  return func::or_else(
      func::multi_match<FString, ELevelRuntimeScaleMode>(
          Mode,
          {
              func::when<FString, ELevelRuntimeScaleMode>(
                  func::equals<FString>(TEXT("building")),
                  [](const FString &) {
                    return ELevelRuntimeScaleMode::Building;
                  }),
              func::when<FString, ELevelRuntimeScaleMode>(
                  func::equals<FString>(TEXT("long_feature")),
                  [](const FString &) {
                    return ELevelRuntimeScaleMode::LongFeature;
                  }),
              func::when<FString, ELevelRuntimeScaleMode>(
                  func::equals<FString>(TEXT("fallback_terrain")),
                  [](const FString &) {
                    return ELevelRuntimeScaleMode::FallbackTerrain;
                  }),
          }),
      ELevelRuntimeScaleMode::Pad);
}

ELevelRuntimeAnchorMode AnchorModeFromJson(const FString &Anchor) {
  return func::or_else(
      func::multi_match<FString, ELevelRuntimeAnchorMode>(
          Anchor,
          {
              func::when<FString, ELevelRuntimeAnchorMode>(
                  func::equals<FString>(TEXT("building_lots")),
                  [](const FString &) {
                    return ELevelRuntimeAnchorMode::BuildingLots;
                  }),
              func::when<FString, ELevelRuntimeAnchorMode>(
                  func::equals<FString>(TEXT("post_office_lots")),
                  [](const FString &) {
                    return ELevelRuntimeAnchorMode::PostOfficeLots;
                  }),
              func::when<FString, ELevelRuntimeAnchorMode>(
                  func::equals<FString>(TEXT("world")),
                  [](const FString &) {
                    return ELevelRuntimeAnchorMode::World;
                  }),
          }),
      ELevelRuntimeAnchorMode::FeatureLots);
}

ELevelRuntimeLabelHeightMode LabelHeightModeFromJson(
    const FString &HeightMode) {
  return func::or_else(
      func::multi_match<FString, ELevelRuntimeLabelHeightMode>(
          HeightMode,
          {
              func::when<FString, ELevelRuntimeLabelHeightMode>(
                  func::equals<FString>(TEXT("label_for_scale")),
                  [](const FString &) {
                    return ELevelRuntimeLabelHeightMode::LabelForScale;
                  }),
              func::when<FString, ELevelRuntimeLabelHeightMode>(
                  func::equals<FString>(TEXT("above_block")),
                  [](const FString &) {
                    return ELevelRuntimeLabelHeightMode::AboveBlock;
                  }),
          }),
      ELevelRuntimeLabelHeightMode::Explicit);
}

ELevelRetroTexture TextureFromJson(const FString &Texture) {
  return func::or_else(
      func::multi_match<FString, ELevelRetroTexture>(
          Texture,
          {
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("terrain_ortho")),
                  [](const FString &) {
                    return ELevelRetroTexture::TerrainOrtho;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("building_timber")),
                  [](const FString &) {
                    return ELevelRetroTexture::BuildingTimber;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("road_dust")),
                  [](const FString &) { return ELevelRetroTexture::RoadDust; }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("water_creek")),
                  [](const FString &) {
                    return ELevelRetroTexture::WaterCreek;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("foliage_riparian")),
                  [](const FString &) {
                    return ELevelRetroTexture::FoliageRiparian;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("rock_granite")),
                  [](const FString &) {
                    return ELevelRetroTexture::RockGranite;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("mine_timber")),
                  [](const FString &) {
                    return ELevelRetroTexture::MineTimber;
                  }),
          }),
      ELevelRetroTexture::MarkerPaint);
}

FVector WorldLocationFromJson(const TSharedPtr<FJsonObject> &World) {
  return FVector(DataAdapters::ReadFloat({World, TEXT("x")}),
                 DataAdapters::ReadFloat({World, TEXT("y")}),
                 DataAdapters::ReadFloat({World, TEXT("z")}));
}

FLevelRuntimeScaleSeed
ScaleFromJson(const TSharedPtr<FJsonObject> &ScaleObject) {
  FLevelRuntimeScaleSeed Seed;
  Seed.Mode = ScaleModeFromJson(
      DataAdapters::ReadString({ScaleObject, TEXT("mode")}));
  Seed.WidthFeet = DataAdapters::ReadFloat({ScaleObject, TEXT("width_feet")});
  Seed.DepthFeet = DataAdapters::ReadFloat({ScaleObject, TEXT("depth_feet")});
  Seed.HeightFeet = DataAdapters::ReadFloat({ScaleObject, TEXT("height_feet")});
  Seed.FrontageFeet =
      DataAdapters::ReadFloat({ScaleObject, TEXT("frontage_feet")});
  Seed.Stories = DataAdapters::ReadFloat({ScaleObject, TEXT("stories")});
  Seed.LengthLots =
      DataAdapters::ReadFloat({ScaleObject, TEXT("length_lots")});
  return Seed;
}

FLevelRuntimeScaleSeed
ReadScaleSeed(const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return func::match(
      DataAdapters::ReadObject(Request),
      [](const TSharedPtr<FJsonObject> &ScaleObject) {
        return ScaleFromJson(ScaleObject);
      },
      []() { return FLevelRuntimeScaleSeed{}; });
}

FLevelRuntimeBlockSeed
BlockFromJson(const TSharedPtr<FJsonObject> &BlockObject) {
  FLevelRuntimeBlockSeed Seed;
  Seed.Id = DataAdapters::ReadString({BlockObject, TEXT("id")});
  Seed.Name = DataAdapters::ReadString({BlockObject, TEXT("name")});
  Seed.Anchor = AnchorModeFromJson(
      DataAdapters::ReadString({BlockObject, TEXT("anchor")}));
  Seed.EastLots = DataAdapters::ReadFloat({BlockObject, TEXT("east_lots")});
  Seed.NorthLots = DataAdapters::ReadFloat({BlockObject, TEXT("north_lots")});
  Seed.WorldLocation = func::match(
      DataAdapters::ReadObject({BlockObject, TEXT("world_location")}),
      WorldLocationFromJson, []() { return FVector::ZeroVector; });
  Seed.Scale = ReadScaleSeed({BlockObject, TEXT("scale")});
  Seed.Texture = TextureFromJson(
      DataAdapters::ReadString({BlockObject, TEXT("texture")}));
  return Seed;
}

FLevelRuntimeLabelSeed
LabelFromJson(const TSharedPtr<FJsonObject> &LabelObject) {
  FLevelRuntimeLabelSeed Seed;
  Seed.Id = DataAdapters::ReadString({LabelObject, TEXT("id")});
  Seed.Text = DataAdapters::ReadString({LabelObject, TEXT("text")});
  Seed.Anchor = AnchorModeFromJson(
      DataAdapters::ReadString({LabelObject, TEXT("anchor")}));
  Seed.Height = LabelHeightModeFromJson(
      DataAdapters::ReadString({LabelObject, TEXT("height_mode")}));
  Seed.EastLots = DataAdapters::ReadFloat({LabelObject, TEXT("east_lots")});
  Seed.NorthLots = DataAdapters::ReadFloat({LabelObject, TEXT("north_lots")});
  Seed.HeightOffset =
      DataAdapters::ReadFloat({LabelObject, TEXT("height_offset")});
  Seed.WorldSizeScale =
      DataAdapters::ReadFloat({LabelObject, TEXT("world_size_scale")});
  Seed.ReferenceScale = ReadScaleSeed({LabelObject, TEXT("reference_scale")});
  return Seed;
}

FLevelRuntimeSectionSeed
SectionFromJson(const TSharedPtr<FJsonObject> &SectionObject) {
  return {DataAdapters::MapJsonValues<FLevelRuntimeBlockSeed>(
              {DataAdapters::ReadArray({SectionObject, TEXT("blocks")}),
               BlockFromJson}),
          DataAdapters::MapJsonValues<FLevelRuntimeLabelSeed>(
              {DataAdapters::ReadArray({SectionObject, TEXT("labels")}),
               LabelFromJson})};
}

FLevelRuntimeSectionSeed
ReadSection(const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return func::match(
      DataAdapters::ReadObject(Request),
      SectionFromJson, []() { return FLevelRuntimeSectionSeed{}; });
}

FLevelRuntimeLayoutSeed
LayoutFromJson(const TSharedPtr<FJsonObject> &Root) {
  FLevelRuntimeLayoutSeed Seed;
  Seed.FallbackTerrainBlock =
      func::match(DataAdapters::ReadObject({Root, TEXT("fallback_terrain")}),
                  BlockFromJson, []() { return FLevelRuntimeBlockSeed{}; });
  Seed.Terrain = ReadSection({Root, TEXT("terrain")});
  Seed.Town = ReadSection({Root, TEXT("town")});
  Seed.Mine = ReadSection({Root, TEXT("mine")});
  Seed.OverlayLabels =
      DataAdapters::MapJsonValues<FLevelRuntimeLabelSeed>(
          {DataAdapters::ReadArray({Root, TEXT("overlay_labels")}),
           LabelFromJson});
  return Seed;
}

} // namespace

FLevelRuntimeLayoutSeed BuildFrenchGulchRuntimeLayoutSeed() {
  return func::match(
      DataAdapters::LoadObjectFromContent({RuntimeLevelDataPath}),
      LayoutFromJson, []() { return FLevelRuntimeLayoutSeed{}; });
}

} // namespace LevelSystemAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
