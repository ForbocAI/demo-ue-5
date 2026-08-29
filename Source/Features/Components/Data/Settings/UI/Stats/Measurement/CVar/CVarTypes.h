#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCVarSettings {
  FString IdleWhenNotForegroundCVarName;
  FString MaxFpsCVarName;
  FString VsyncCVarName;
};

/** User Story: As a stats measurement consumer, I need to compare console-variable settings so diagnostics query one authored contract. @fn inline bool operator==(const FCVarSettings &Left, const FCVarSettings &Right) */
inline bool operator==(const FCVarSettings &Left,
                       const FCVarSettings &Right) {
  return Left.IdleWhenNotForegroundCVarName ==
             Right.IdleWhenNotForegroundCVarName &&
         Left.MaxFpsCVarName == Right.MaxFpsCVarName &&
         Left.VsyncCVarName == Right.VsyncCVarName;
}

/** User Story: As a stats measurement consumer, I need to compare console-variable settings for inequality so diagnostics query one authored contract. @fn inline bool operator!=(const FCVarSettings &Left, const FCVarSettings &Right) */
inline bool operator!=(const FCVarSettings &Left,
                       const FCVarSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
