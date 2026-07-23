#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FValidationSettings {
  int32 TerrainGridSize;
  int32 OrthoGridSize;
  float TerrainMinReliefMeters;
};

} // namespace Data
} // namespace Game
} // namespace ForbocAI
