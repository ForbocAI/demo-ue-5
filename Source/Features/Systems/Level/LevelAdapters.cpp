#include "Features/Systems/Level/LevelAdapters.h"

#include "Dom/JsonObject.h"
#include "Features/Components/Data/DataAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelAdapters {

namespace DataAdapters = ForbocAI::Demo::Data::DataAdapters;

namespace {

constexpr const TCHAR *RuntimeLevelDataPath =
    TEXT("Data/french_gulch_runtime_level.json");

void LogInvalidField(const FString &FieldName) {
  UE_LOG(LogTemp, Error, TEXT("Runtime level JSON invalid field: %s"),
         *FieldName);
}

func::Maybe<FString> ReadRequiredString(
    const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  FString Value;
  if (Request.Object.IsValid() &&
      Request.Object->TryGetStringField(Request.FieldName, Value)) {
    return func::just(Value);
  }

  LogInvalidField(Request.FieldName);
  return func::nothing<FString>();
}

func::Maybe<float> ReadRequiredFloat(
    const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  double Value = 0.0;
  if (Request.Object.IsValid() &&
      Request.Object->TryGetNumberField(Request.FieldName, Value)) {
    return func::just(static_cast<float>(Value));
  }

  LogInvalidField(Request.FieldName);
  return func::nothing<float>();
}

func::Maybe<TSharedPtr<FJsonObject>> ReadRequiredObject(
    const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Value =
      DataAdapters::ReadObject(Request);
  if (Value.hasValue) {
    return Value;
  }

  LogInvalidField(Request.FieldName);
  return func::nothing<TSharedPtr<FJsonObject>>();
}

func::Maybe<TArray<TSharedPtr<FJsonValue>>> ReadRequiredArray(
    const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  const TArray<TSharedPtr<FJsonValue>> *Values = nullptr;
  if (Request.Object.IsValid() &&
      Request.Object->TryGetArrayField(Request.FieldName, Values) &&
      Values != nullptr) {
    return func::just(*Values);
  }

  LogInvalidField(Request.FieldName);
  return func::nothing<TArray<TSharedPtr<FJsonValue>>>();
}

template <typename Output>
func::Maybe<TArray<Output>> MapRequiredJsonValues(
    const TArray<TSharedPtr<FJsonValue>> &Values,
    TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue) {
  TArray<Output> Acc;
  Acc.Reserve(Values.Num());
  for (const TSharedPtr<FJsonValue> &Value : Values) {
    const TSharedPtr<FJsonObject> Object =
        Value.IsValid() ? Value->AsObject() : TSharedPtr<FJsonObject>();
    if (!Object.IsValid()) {
      UE_LOG(LogTemp, Error, TEXT("Runtime level JSON array item invalid"));
      return func::nothing<TArray<Output>>();
    }

    const func::Maybe<Output> Parsed = MapValue(Object);
    if (!Parsed.hasValue) {
      return func::nothing<TArray<Output>>();
    }

    Acc.Add(Parsed.value);
  }
  return func::just(Acc);
}

func::Maybe<ELevelRuntimeScaleMode> ScaleModeFromJson(const FString &Mode) {
  return func::multi_match<FString, ELevelRuntimeScaleMode>(
      Mode.ToLower(),
      {
          func::when<FString, ELevelRuntimeScaleMode>(
              func::equals<FString>(TEXT("building")),
              [](const FString &) { return ELevelRuntimeScaleMode::Building; }),
          func::when<FString, ELevelRuntimeScaleMode>(
              func::equals<FString>(TEXT("long_feature")),
              [](const FString &) {
                return ELevelRuntimeScaleMode::LongFeature;
              }),
          func::when<FString, ELevelRuntimeScaleMode>(
              func::equals<FString>(TEXT("pad")),
              [](const FString &) { return ELevelRuntimeScaleMode::Pad; }),
          func::when<FString, ELevelRuntimeScaleMode>(
              func::equals<FString>(TEXT("pad_feet")),
              [](const FString &) { return ELevelRuntimeScaleMode::Pad; }),
      });
}

func::Maybe<ELevelRuntimeAnchorMode> AnchorModeFromJson(
    const FString &Anchor) {
  return func::multi_match<FString, ELevelRuntimeAnchorMode>(
      Anchor.ToLower(),
      {
          func::when<FString, ELevelRuntimeAnchorMode>(
              func::equals<FString>(TEXT("building_lots")),
              [](const FString &) {
                return ELevelRuntimeAnchorMode::BuildingLots;
              }),
          func::when<FString, ELevelRuntimeAnchorMode>(
              func::equals<FString>(TEXT("feature_lots")),
              [](const FString &) {
                return ELevelRuntimeAnchorMode::FeatureLots;
              }),
          func::when<FString, ELevelRuntimeAnchorMode>(
              func::equals<FString>(TEXT("post_office_lots")),
              [](const FString &) {
                return ELevelRuntimeAnchorMode::PostOfficeLots;
              }),
          func::when<FString, ELevelRuntimeAnchorMode>(
              func::equals<FString>(TEXT("world")),
              [](const FString &) { return ELevelRuntimeAnchorMode::World; }),
      });
}

func::Maybe<ELevelRuntimeLabelHeightMode> LabelHeightModeFromJson(
    const FString &HeightMode) {
  return func::multi_match<FString, ELevelRuntimeLabelHeightMode>(
      HeightMode.ToLower(),
      {
          func::when<FString, ELevelRuntimeLabelHeightMode>(
              func::equals<FString>(TEXT("explicit")),
              [](const FString &) {
                return ELevelRuntimeLabelHeightMode::Explicit;
              }),
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
      });
}

func::Maybe<ELevelRetroTexture> TextureFromJson(const FString &Texture) {
  return func::multi_match<FString, ELevelRetroTexture>(
      Texture.ToLower(),
      {
          func::when<FString, ELevelRetroTexture>(
              func::equals<FString>(TEXT("terrain_ortho")),
              [](const FString &) { return ELevelRetroTexture::TerrainOrtho; }),
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
      });
}

func::Maybe<FVector> WorldLocationFromJson(
    const TSharedPtr<FJsonObject> &World) {
  const func::Maybe<float> X = ReadRequiredFloat({World, TEXT("x")});
  const func::Maybe<float> Y = ReadRequiredFloat({World, TEXT("y")});
  const func::Maybe<float> Z = ReadRequiredFloat({World, TEXT("z")});
  return X.hasValue && Y.hasValue && Z.hasValue
             ? func::just(FVector(X.value, Y.value, Z.value))
             : func::nothing<FVector>();
}

func::Maybe<FLevelRuntimeScaleSeed>
ScaleFromJson(const TSharedPtr<FJsonObject> &ScaleObject) {
  const func::Maybe<FString> ModeText =
      ReadRequiredString({ScaleObject, TEXT("mode")});
  if (!ModeText.hasValue) {
    return func::nothing<FLevelRuntimeScaleSeed>();
  }

  const func::Maybe<ELevelRuntimeScaleMode> Mode =
      ScaleModeFromJson(ModeText.value);
  if (!Mode.hasValue) {
    LogInvalidField(TEXT("mode"));
    return func::nothing<FLevelRuntimeScaleSeed>();
  }

  FLevelRuntimeScaleSeed Seed;
  Seed.Mode = Mode.value;
  if (Seed.Mode == ELevelRuntimeScaleMode::Building) {
    const func::Maybe<float> FrontageFeet =
        ReadRequiredFloat({ScaleObject, TEXT("frontage_feet")});
    const func::Maybe<float> DepthFeet =
        ReadRequiredFloat({ScaleObject, TEXT("depth_feet")});
    const func::Maybe<float> Stories =
        ReadRequiredFloat({ScaleObject, TEXT("stories")});
    if (!FrontageFeet.hasValue || !DepthFeet.hasValue || !Stories.hasValue) {
      return func::nothing<FLevelRuntimeScaleSeed>();
    }
    Seed.FrontageFeet = FrontageFeet.value;
    Seed.DepthFeet = DepthFeet.value;
    Seed.Stories = Stories.value;
    return func::just(Seed);
  }

  if (Seed.Mode == ELevelRuntimeScaleMode::LongFeature) {
    const func::Maybe<float> WidthFeet =
        ReadRequiredFloat({ScaleObject, TEXT("width_feet")});
    const func::Maybe<float> LengthLots =
        ReadRequiredFloat({ScaleObject, TEXT("length_lots")});
    const func::Maybe<float> HeightFeet =
        ReadRequiredFloat({ScaleObject, TEXT("height_feet")});
    if (!WidthFeet.hasValue || !LengthLots.hasValue || !HeightFeet.hasValue) {
      return func::nothing<FLevelRuntimeScaleSeed>();
    }
    Seed.WidthFeet = WidthFeet.value;
    Seed.LengthLots = LengthLots.value;
    Seed.HeightFeet = HeightFeet.value;
    return func::just(Seed);
  }

  const func::Maybe<float> WidthFeet =
      ReadRequiredFloat({ScaleObject, TEXT("width_feet")});
  const func::Maybe<float> DepthFeet =
      ReadRequiredFloat({ScaleObject, TEXT("depth_feet")});
  const func::Maybe<float> HeightFeet =
      ReadRequiredFloat({ScaleObject, TEXT("height_feet")});
  if (!WidthFeet.hasValue || !DepthFeet.hasValue || !HeightFeet.hasValue) {
    return func::nothing<FLevelRuntimeScaleSeed>();
  }
  Seed.WidthFeet = WidthFeet.value;
  Seed.DepthFeet = DepthFeet.value;
  Seed.HeightFeet = HeightFeet.value;
  return func::just(Seed);
}

func::Maybe<FLevelRuntimeScaleSeed>
ReadScaleSeed(const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return func::match(
      ReadRequiredObject(Request),
      [](const TSharedPtr<FJsonObject> &ScaleObject) {
        return ScaleFromJson(ScaleObject);
      },
      []() { return func::nothing<FLevelRuntimeScaleSeed>(); });
}

func::Maybe<FLevelRuntimeBlockSeed>
BlockFromJson(const TSharedPtr<FJsonObject> &BlockObject) {
  const func::Maybe<FString> Id =
      ReadRequiredString({BlockObject, TEXT("id")});
  const func::Maybe<FString> Name =
      ReadRequiredString({BlockObject, TEXT("name")});
  const func::Maybe<FString> AnchorText =
      ReadRequiredString({BlockObject, TEXT("anchor")});
  const func::Maybe<FLevelRuntimeScaleSeed> Scale =
      ReadScaleSeed({BlockObject, TEXT("scale")});
  const func::Maybe<FString> TextureText =
      ReadRequiredString({BlockObject, TEXT("texture")});
  if (!Id.hasValue || !Name.hasValue || !AnchorText.hasValue ||
      !Scale.hasValue || !TextureText.hasValue) {
    return func::nothing<FLevelRuntimeBlockSeed>();
  }

  const func::Maybe<ELevelRuntimeAnchorMode> Anchor =
      AnchorModeFromJson(AnchorText.value);
  const func::Maybe<ELevelRetroTexture> Texture =
      TextureFromJson(TextureText.value);
  if (!Anchor.hasValue || !Texture.hasValue) {
    LogInvalidField(!Anchor.hasValue ? TEXT("anchor") : TEXT("texture"));
    return func::nothing<FLevelRuntimeBlockSeed>();
  }

  FLevelRuntimeBlockSeed Seed;
  Seed.Id = Id.value;
  Seed.Name = Name.value;
  Seed.Anchor = Anchor.value;
  Seed.Scale = Scale.value;
  Seed.Texture = Texture.value;
  if (Seed.Anchor == ELevelRuntimeAnchorMode::World) {
    const func::Maybe<TSharedPtr<FJsonObject>> WorldObject =
        ReadRequiredObject({BlockObject, TEXT("world_location")});
    if (!WorldObject.hasValue) {
      return func::nothing<FLevelRuntimeBlockSeed>();
    }
    const func::Maybe<FVector> Location =
        WorldLocationFromJson(WorldObject.value);
    if (!Location.hasValue) {
      return func::nothing<FLevelRuntimeBlockSeed>();
    }
    Seed.WorldLocation = Location.value;
    return func::just(Seed);
  }

  const func::Maybe<float> EastLots =
      ReadRequiredFloat({BlockObject, TEXT("east_lots")});
  const func::Maybe<float> NorthLots =
      ReadRequiredFloat({BlockObject, TEXT("north_lots")});
  if (!EastLots.hasValue || !NorthLots.hasValue) {
    return func::nothing<FLevelRuntimeBlockSeed>();
  }
  Seed.EastLots = EastLots.value;
  Seed.NorthLots = NorthLots.value;
  return func::just(Seed);
}

func::Maybe<FLevelRuntimeLabelSeed>
LabelFromJson(const TSharedPtr<FJsonObject> &LabelObject) {
  const func::Maybe<FString> Id =
      ReadRequiredString({LabelObject, TEXT("id")});
  const func::Maybe<FString> Text =
      ReadRequiredString({LabelObject, TEXT("text")});
  const func::Maybe<FString> AnchorText =
      ReadRequiredString({LabelObject, TEXT("anchor")});
  const func::Maybe<FString> HeightText =
      ReadRequiredString({LabelObject, TEXT("height_mode")});
  const func::Maybe<float> EastLots =
      ReadRequiredFloat({LabelObject, TEXT("east_lots")});
  const func::Maybe<float> NorthLots =
      ReadRequiredFloat({LabelObject, TEXT("north_lots")});
  const func::Maybe<float> WorldSizeScale =
      ReadRequiredFloat({LabelObject, TEXT("world_size_scale")});
  if (!Id.hasValue || !Text.hasValue || !AnchorText.hasValue ||
      !HeightText.hasValue || !EastLots.hasValue || !NorthLots.hasValue ||
      !WorldSizeScale.hasValue) {
    return func::nothing<FLevelRuntimeLabelSeed>();
  }

  const func::Maybe<ELevelRuntimeAnchorMode> Anchor =
      AnchorModeFromJson(AnchorText.value);
  const func::Maybe<ELevelRuntimeLabelHeightMode> Height =
      LabelHeightModeFromJson(HeightText.value);
  if (!Anchor.hasValue || !Height.hasValue) {
    LogInvalidField(!Anchor.hasValue ? TEXT("anchor") : TEXT("height_mode"));
    return func::nothing<FLevelRuntimeLabelSeed>();
  }

  FLevelRuntimeLabelSeed Seed;
  Seed.Id = Id.value;
  Seed.Text = Text.value;
  Seed.Anchor = Anchor.value;
  Seed.Height = Height.value;
  Seed.EastLots = EastLots.value;
  Seed.NorthLots = NorthLots.value;
  Seed.WorldSizeScale = WorldSizeScale.value;
  if (Seed.Height == ELevelRuntimeLabelHeightMode::Explicit) {
    const func::Maybe<float> HeightOffset =
        ReadRequiredFloat({LabelObject, TEXT("height_offset")});
    if (!HeightOffset.hasValue) {
      return func::nothing<FLevelRuntimeLabelSeed>();
    }
    Seed.HeightOffset = HeightOffset.value;
    return func::just(Seed);
  }

  const func::Maybe<FLevelRuntimeScaleSeed> ReferenceScale =
      ReadScaleSeed({LabelObject, TEXT("reference_scale")});
  if (!ReferenceScale.hasValue) {
    return func::nothing<FLevelRuntimeLabelSeed>();
  }
  Seed.ReferenceScale = ReferenceScale.value;
  return func::just(Seed);
}

func::Maybe<FLevelRuntimeSectionSeed>
SectionFromJson(const TSharedPtr<FJsonObject> &SectionObject) {
  const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Blocks =
      ReadRequiredArray({SectionObject, TEXT("blocks")});
  const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Labels =
      ReadRequiredArray({SectionObject, TEXT("labels")});
  if (!Blocks.hasValue || !Labels.hasValue) {
    return func::nothing<FLevelRuntimeSectionSeed>();
  }

  const func::Maybe<TArray<FLevelRuntimeBlockSeed>> ParsedBlocks =
      MapRequiredJsonValues<FLevelRuntimeBlockSeed>(Blocks.value,
                                                    BlockFromJson);
  const func::Maybe<TArray<FLevelRuntimeLabelSeed>> ParsedLabels =
      MapRequiredJsonValues<FLevelRuntimeLabelSeed>(Labels.value,
                                                    LabelFromJson);
  if (!ParsedBlocks.hasValue || !ParsedLabels.hasValue) {
    return func::nothing<FLevelRuntimeSectionSeed>();
  }

  return func::just(
      FLevelRuntimeSectionSeed{ParsedBlocks.value, ParsedLabels.value});
}

func::Maybe<FLevelRuntimeSectionSeed>
ReadSection(const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return func::match(
      ReadRequiredObject(Request),
      SectionFromJson,
      []() { return func::nothing<FLevelRuntimeSectionSeed>(); });
}

func::Maybe<FLevelRuntimeLayoutSeed>
LayoutFromJson(const TSharedPtr<FJsonObject> &Root) {
  const func::Maybe<FLevelRuntimeSectionSeed> Terrain =
      ReadSection({Root, TEXT("terrain")});
  const func::Maybe<FLevelRuntimeSectionSeed> Town =
      ReadSection({Root, TEXT("town")});
  const func::Maybe<FLevelRuntimeSectionSeed> Mine =
      ReadSection({Root, TEXT("mine")});
  const func::Maybe<TArray<TSharedPtr<FJsonValue>>> OverlayValues =
      ReadRequiredArray({Root, TEXT("overlay_labels")});
  if (!Terrain.hasValue || !Town.hasValue || !Mine.hasValue ||
      !OverlayValues.hasValue) {
    return func::nothing<FLevelRuntimeLayoutSeed>();
  }

  const func::Maybe<TArray<FLevelRuntimeLabelSeed>> OverlayLabels =
      MapRequiredJsonValues<FLevelRuntimeLabelSeed>(OverlayValues.value,
                                                    LabelFromJson);
  if (!OverlayLabels.hasValue) {
    return func::nothing<FLevelRuntimeLayoutSeed>();
  }

  FLevelRuntimeLayoutSeed Seed;
  Seed.Terrain = Terrain.value;
  Seed.Town = Town.value;
  Seed.Mine = Mine.value;
  Seed.OverlayLabels = OverlayLabels.value;
  return func::just(Seed);
}

} // namespace

func::Maybe<FLevelRuntimeLayoutSeed> LoadFrenchGulchRuntimeLayoutSeed() {
  return func::match(
      DataAdapters::LoadObjectFromContent({RuntimeLevelDataPath}),
      LayoutFromJson,
      []() { return func::nothing<FLevelRuntimeLayoutSeed>(); });
}

} // namespace LevelAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
