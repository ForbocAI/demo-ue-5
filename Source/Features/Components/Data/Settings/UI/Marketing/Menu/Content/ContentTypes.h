#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FContentSettings {
  FString MenuTitle;
  FString MenuRetakeButtonText;
  FString MenuResumeButtonText;
};

/** User Story: As a marketing menu consumer, I need to compare content settings so menu language remains deterministic. @fn inline bool operator==(const FContentSettings &Left, const FContentSettings &Right) */
inline bool operator==(const FContentSettings &Left,
                       const FContentSettings &Right) {
  return Left.MenuTitle == Right.MenuTitle &&
         Left.MenuRetakeButtonText == Right.MenuRetakeButtonText &&
         Left.MenuResumeButtonText == Right.MenuResumeButtonText;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
