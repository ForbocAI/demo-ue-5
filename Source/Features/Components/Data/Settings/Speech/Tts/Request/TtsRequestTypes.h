#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FTtsRequestSettings {
  FString TtsEndpoint;
  FString TtsVerb;
  FString TtsContentTypeHeader;
  FString TtsContentType;
  FString TtsRequestFormat;
};

/** User Story: As a speech request consumer, I need to compare TTS request settings so transport remains deterministic. @fn inline bool operator==(const FTtsRequestSettings &Left, const FTtsRequestSettings &Right) */
inline bool operator==(const FTtsRequestSettings &Left,
                       const FTtsRequestSettings &Right) {
  return Left.TtsEndpoint == Right.TtsEndpoint &&
         Left.TtsVerb == Right.TtsVerb &&
         Left.TtsContentTypeHeader == Right.TtsContentTypeHeader &&
         Left.TtsContentType == Right.TtsContentType &&
         Left.TtsRequestFormat == Right.TtsRequestFormat;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
