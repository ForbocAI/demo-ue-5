#pragma once

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Bot {

struct FProtocolCoreAssertions {
  FString RouteCrossoverRejected;
  FString ActionProjected;
  FString CorrelationPreserved;
  FString ProtocolFailurePreserved;
  FString TownspersonAvailable;
};

struct FTraceAssertions {
  FString CorrelationPresent;
  FString FamilyMatched;
  FString CommandMatched;
};

struct FResultAssertions {
  FString ActorPresent;
  FString OutputPresent;
  FString ErrorAbsent;
};

struct FGhostResultAssertions {
  FString ActorPresent;
  FString SessionPresent;
  FString OutputPresent;
  FString QualifiedOutput;
  FString ErrorAbsent;
};

struct FNpcAssertions {
  FString Succeeded;
  FTraceAssertions Trace;
  FResultAssertions Result;
};

struct FGhostAssertions {
  FString Succeeded;
  FTraceAssertions Trace;
  FGhostResultAssertions Result;
};

struct FLogAssertions {
  FString SummaryNpcCorrelation;
  FString SummaryGhostCorrelation;
  FString SummaryGhostSession;
  FString NpcOutputRedacted;
  FString GhostOutputRedacted;
};

struct FProtocolAssertions {
  FProtocolCoreAssertions Core;
  FNpcAssertions Npc;
  FGhostAssertions Ghost;
  FLogAssertions Log;
};

inline bool operator==(const FProtocolCoreAssertions &Left,
                       const FProtocolCoreAssertions &Right) {
  return Left.RouteCrossoverRejected == Right.RouteCrossoverRejected &&
         Left.ActionProjected == Right.ActionProjected &&
         Left.CorrelationPreserved == Right.CorrelationPreserved &&
         Left.ProtocolFailurePreserved == Right.ProtocolFailurePreserved &&
         Left.TownspersonAvailable == Right.TownspersonAvailable;
}

inline bool operator!=(const FProtocolCoreAssertions &Left,
                       const FProtocolCoreAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTraceAssertions &Left,
                       const FTraceAssertions &Right) {
  return Left.CorrelationPresent == Right.CorrelationPresent &&
         Left.FamilyMatched == Right.FamilyMatched &&
         Left.CommandMatched == Right.CommandMatched;
}

inline bool operator!=(const FTraceAssertions &Left,
                       const FTraceAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FResultAssertions &Left,
                       const FResultAssertions &Right) {
  return Left.ActorPresent == Right.ActorPresent &&
         Left.OutputPresent == Right.OutputPresent &&
         Left.ErrorAbsent == Right.ErrorAbsent;
}

inline bool operator!=(const FResultAssertions &Left,
                       const FResultAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FGhostResultAssertions &Left,
                       const FGhostResultAssertions &Right) {
  return Left.ActorPresent == Right.ActorPresent &&
         Left.SessionPresent == Right.SessionPresent &&
         Left.OutputPresent == Right.OutputPresent &&
         Left.QualifiedOutput == Right.QualifiedOutput &&
         Left.ErrorAbsent == Right.ErrorAbsent;
}

inline bool operator!=(const FGhostResultAssertions &Left,
                       const FGhostResultAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FNpcAssertions &Left,
                       const FNpcAssertions &Right) {
  return Left.Succeeded == Right.Succeeded && Left.Trace == Right.Trace &&
         Left.Result == Right.Result;
}

inline bool operator!=(const FNpcAssertions &Left,
                       const FNpcAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FGhostAssertions &Left,
                       const FGhostAssertions &Right) {
  return Left.Succeeded == Right.Succeeded && Left.Trace == Right.Trace &&
         Left.Result == Right.Result;
}

inline bool operator!=(const FGhostAssertions &Left,
                       const FGhostAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLogAssertions &Left,
                       const FLogAssertions &Right) {
  return Left.SummaryNpcCorrelation == Right.SummaryNpcCorrelation &&
         Left.SummaryGhostCorrelation == Right.SummaryGhostCorrelation &&
         Left.SummaryGhostSession == Right.SummaryGhostSession &&
         Left.NpcOutputRedacted == Right.NpcOutputRedacted &&
         Left.GhostOutputRedacted == Right.GhostOutputRedacted;
}

inline bool operator!=(const FLogAssertions &Left,
                       const FLogAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FProtocolAssertions &Left,
                       const FProtocolAssertions &Right) {
  return Left.Core == Right.Core && Left.Npc == Right.Npc &&
         Left.Ghost == Right.Ghost && Left.Log == Right.Log;
}

inline bool operator!=(const FProtocolAssertions &Left,
                       const FProtocolAssertions &Right) {
  return !(Left == Right);
}

} // namespace Bot
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
