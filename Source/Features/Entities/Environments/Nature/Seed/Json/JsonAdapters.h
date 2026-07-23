#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/Json/SeedJsonTypes.h"
#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"

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
  /** User Story: As a nature seed json consumer, I need to invoke values through a stable signature so the nature seed json workflow remains explicit and composable. @fn static const TArray<TValueDeclaration<EFeatureKind>> &Values() */
  static const TArray<TValueDeclaration<EFeatureKind>> &Values() {
    static const TArray<TValueDeclaration<EFeatureKind>>
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
  /** User Story: As a nature seed json consumer, I need to invoke values through a stable signature so the nature seed json workflow remains explicit and composable. @fn static const TArray<TValueDeclaration<ENatureScaleMode>> &Values() */
  static const TArray<TValueDeclaration<ENatureScaleMode>> &Values() {
    static const TArray<TValueDeclaration<ENatureScaleMode>>
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
