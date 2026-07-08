#pragma once

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Game::Level;
namespace NatureTypes = ForbocAI::Game::Level::NatureAdapters;

using EFeatureKind = LevelTypes::EFeatureKind;
using ENatureScaleMode = NatureTypes::ENatureScaleMode;
using FNatureFeatureSource = NatureTypes::FNatureFeatureSource;
using FNatureLongFeatureScaleSource =
    NatureTypes::FNatureLongFeatureScaleSource;
using FNaturePadScaleSource = NatureTypes::FNaturePadScaleSource;
using FNatureScaleModeSource = NatureTypes::FNatureScaleModeSource;

template <>
struct TJsonTextValueRegistry<EFeatureKind> {
  static const TArray<TTextValueDeclaration<EFeatureKind>> &Values() {
    static const TArray<TTextValueDeclaration<EFeatureKind>>
        RegisteredValues = {{"water", EFeatureKind::Water},
                            {"rock", EFeatureKind::Rock},
                            {"tree_grove", EFeatureKind::TreeGrove},
                            {"shrub", EFeatureKind::Shrub},
                            {"water_system_marker",
                             EFeatureKind::WaterSystemMarker},
                            {"pcg_marker", EFeatureKind::PCGMarker}};
    return RegisteredValues;
  }
};

template <>
struct TJsonTextValueRegistry<ENatureScaleMode> {
  static const TArray<TTextValueDeclaration<ENatureScaleMode>> &Values() {
    static const TArray<TTextValueDeclaration<ENatureScaleMode>>
        RegisteredValues = {{"long_feature", ENatureScaleMode::LongFeature},
                            {"pad", ENatureScaleMode::Pad},
                            {"pad_feet", ENatureScaleMode::Pad}};
    return RegisteredValues;
  }
};

JSON_SETTINGS_REGISTRY(FNatureScaleModeSource, Mode);
JSON_SETTINGS_REGISTRY(FNaturePadScaleSource, WidthFeet, DepthFeet, HeightFeet);
JSON_SETTINGS_REGISTRY(FNatureLongFeatureScaleSource, WidthFeet, LengthLots,
                       HeightFeet);
JSON_SETTINGS_REGISTRY(FNatureFeatureSource, Id, Name, Kind, EastLots,
                       NorthLots, Scale);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
