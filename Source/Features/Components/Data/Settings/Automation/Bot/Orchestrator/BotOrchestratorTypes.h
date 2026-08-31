#pragma once

#include "CoreMinimal.h"
#include "Features/Components/Data/Settings/Automation/Bot/Orchestrator/Protocol/Assertions/ProtocolAssertionsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Bot {

struct FOrchestratorGroups {
  FString Registration;
  FString Cycle;
  FString RuntimeStore;
  FString Protocol;
};

struct FRegistrationCaseLabels {
  FString RegisterBot;
  FString RegisterBots;
};

struct FCycleCaseLabels {
  FString RespectObservationInterval;
  FString DispatchMovement;
};

struct FProtocolCaseLabels {
  FString RejectRouteCrossover;
  FString ProjectAction;
  FString ReduceProtocolLifecycle;
  FString ExecuteLiveFlows;
};

struct FProtocolRunSettings {
  float TimeoutSeconds = float{};
  FString QualifiedDiagnosticMarker;
  FString FailurePrefix;
};

struct FOrchestratorCaseLabels {
  FRegistrationCaseLabels Registration;
  FCycleCaseLabels Cycle;
  FProtocolCaseLabels Protocol;
};

struct FRootStateAssertions {
  FString ThreeBotsInRootState;
  FString BotSelectable;
  FString HorseSelectable;
  FString PositionSelectable;
  FString WorldPositionUpdated;
};

struct FOrchestratorAssertions {
  FRootStateAssertions RootState;
  FProtocolAssertions Protocol;
};

struct FOrchestratorSettings {
  FString Spec;
  FString MultiBotSpec;
  FString ProtocolSpec;
  FString Persona;
  int32 WorldContextIndex;
  FOrchestratorGroups Groups;
  FOrchestratorCaseLabels Cases;
  FOrchestratorAssertions Assertions;
  FProtocolRunSettings ProtocolRun;
};

/** User Story: As bot automation, I need orchestrator groups compared as one composed value. @fn inline bool operator==(const FOrchestratorGroups &Left, const FOrchestratorGroups &Right) */
inline bool operator==(const FOrchestratorGroups &Left,
                       const FOrchestratorGroups &Right) {
  return Left.Registration == Right.Registration &&
         Left.Cycle == Right.Cycle &&
         Left.RuntimeStore == Right.RuntimeStore &&
         Left.Protocol == Right.Protocol;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FOrchestratorGroups &Left, const FOrchestratorGroups &Right) */
inline bool operator!=(const FOrchestratorGroups &Left,
                       const FOrchestratorGroups &Right) {
  return !(Left == Right);
}

/** User Story: As a automation bot orchestrator state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FRegistrationCaseLabels &Left, const FRegistrationCaseLabels &Right) */
inline bool operator==(const FRegistrationCaseLabels &Left,
                       const FRegistrationCaseLabels &Right) {
  return Left.RegisterBot == Right.RegisterBot &&
         Left.RegisterBots == Right.RegisterBots;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FRegistrationCaseLabels &Left, const FRegistrationCaseLabels &Right) */
inline bool operator!=(const FRegistrationCaseLabels &Left,
                       const FRegistrationCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a automation bot orchestrator state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FCycleCaseLabels &Left, const FCycleCaseLabels &Right) */
inline bool operator==(const FCycleCaseLabels &Left,
                       const FCycleCaseLabels &Right) {
  return Left.RespectObservationInterval == Right.RespectObservationInterval &&
         Left.DispatchMovement == Right.DispatchMovement;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FCycleCaseLabels &Left, const FCycleCaseLabels &Right) */
inline bool operator!=(const FCycleCaseLabels &Left,
                       const FCycleCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a automation bot orchestrator state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FProtocolCaseLabels &Left, const FProtocolCaseLabels &Right) */
inline bool operator==(const FProtocolCaseLabels &Left,
                       const FProtocolCaseLabels &Right) {
  return Left.RejectRouteCrossover == Right.RejectRouteCrossover &&
         Left.ProjectAction == Right.ProjectAction &&
         Left.ReduceProtocolLifecycle == Right.ReduceProtocolLifecycle &&
         Left.ExecuteLiveFlows == Right.ExecuteLiveFlows;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FProtocolCaseLabels &Left, const FProtocolCaseLabels &Right) */
inline bool operator!=(const FProtocolCaseLabels &Left,
                       const FProtocolCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a automation bot orchestrator state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FProtocolRunSettings &Left, const FProtocolRunSettings &Right) */
inline bool operator==(const FProtocolRunSettings &Left,
                       const FProtocolRunSettings &Right) {
  return Left.TimeoutSeconds == Right.TimeoutSeconds &&
         Left.QualifiedDiagnosticMarker == Right.QualifiedDiagnosticMarker &&
         Left.FailurePrefix == Right.FailurePrefix;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FProtocolRunSettings &Left, const FProtocolRunSettings &Right) */
inline bool operator!=(const FProtocolRunSettings &Left,
                       const FProtocolRunSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a automation bot orchestrator state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FOrchestratorCaseLabels &Left, const FOrchestratorCaseLabels &Right) */
inline bool operator==(const FOrchestratorCaseLabels &Left,
                       const FOrchestratorCaseLabels &Right) {
  return Left.Registration == Right.Registration &&
         Left.Cycle == Right.Cycle && Left.Protocol == Right.Protocol;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FOrchestratorCaseLabels &Left, const FOrchestratorCaseLabels &Right) */
inline bool operator!=(const FOrchestratorCaseLabels &Left,
                       const FOrchestratorCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a automation bot orchestrator state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FRootStateAssertions &Left, const FRootStateAssertions &Right) */
inline bool operator==(const FRootStateAssertions &Left,
                       const FRootStateAssertions &Right) {
  return Left.ThreeBotsInRootState == Right.ThreeBotsInRootState &&
         Left.BotSelectable == Right.BotSelectable &&
         Left.HorseSelectable == Right.HorseSelectable &&
         Left.PositionSelectable == Right.PositionSelectable &&
         Left.WorldPositionUpdated == Right.WorldPositionUpdated;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FRootStateAssertions &Left, const FRootStateAssertions &Right) */
inline bool operator!=(const FRootStateAssertions &Left,
                       const FRootStateAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a automation bot orchestrator state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FOrchestratorAssertions &Left, const FOrchestratorAssertions &Right) */
inline bool operator==(const FOrchestratorAssertions &Left,
                       const FOrchestratorAssertions &Right) {
  return Left.RootState == Right.RootState &&
         Left.Protocol == Right.Protocol;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FOrchestratorAssertions &Left, const FOrchestratorAssertions &Right) */
inline bool operator!=(const FOrchestratorAssertions &Left,
                       const FOrchestratorAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a automation bot orchestrator state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FOrchestratorSettings &Left, const FOrchestratorSettings &Right) */
inline bool operator==(const FOrchestratorSettings &Left,
                       const FOrchestratorSettings &Right) {
  return Left.Spec == Right.Spec && Left.MultiBotSpec == Right.MultiBotSpec &&
         Left.ProtocolSpec == Right.ProtocolSpec &&
         Left.Persona == Right.Persona &&
         Left.WorldContextIndex == Right.WorldContextIndex &&
         Left.Groups == Right.Groups && Left.Cases == Right.Cases &&
         Left.Assertions == Right.Assertions &&
         Left.ProtocolRun == Right.ProtocolRun;
}

/** User Story: As a automation bot orchestrator state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FOrchestratorSettings &Left, const FOrchestratorSettings &Right) */
inline bool operator!=(const FOrchestratorSettings &Left,
                       const FOrchestratorSettings &Right) {
  return !(Left == Right);
}

} // namespace Bot
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
