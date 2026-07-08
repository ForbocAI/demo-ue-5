#pragma once

#include "Core/rtk.hpp"

class FJsonObject;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

enum class ENatureScaleMode { LongFeature, Pad };

struct FNatureScaleModeSource {
  FString Mode;
};

struct FNaturePadScaleSource {
  float WidthFeet;
  float DepthFeet;
  float HeightFeet;
};

struct FNatureLongFeatureScaleSource {
  float WidthFeet;
  float LengthLots;
  float HeightFeet;
};

struct FNatureFeatureSource {
  FString Id;
  FString Name;
  FString Kind;
  float EastLots;
  float NorthLots;
  TSharedPtr<FJsonObject> Scale;
};

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
