#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FLookupAssertionSettings {
  FString KnownPhonemeLabelFormat;
  FString UnknownPhonemeLabelFormat;
};

/** User Story: As a speech automation consumer, I need to compare lookup assertion labels so diagnostics remain data-driven. @fn inline bool operator==(const FLookupAssertionSettings &Left, const FLookupAssertionSettings &Right) */
inline bool operator==(const FLookupAssertionSettings &Left,
                       const FLookupAssertionSettings &Right) {
  return Left.KnownPhonemeLabelFormat == Right.KnownPhonemeLabelFormat &&
         Left.UnknownPhonemeLabelFormat == Right.UnknownPhonemeLabelFormat;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
