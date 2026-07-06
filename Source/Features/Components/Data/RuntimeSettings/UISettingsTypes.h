#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRuntimeStatsOverlaySettings {
  FString FramesPerSecondLabel;
  FString StackDepthLabel;
  FString PolyCountLabel;
  FString LabelValueSeparator;
  FString ValueFormat;
  FString DebugMessageFormat;
  int32 DebugMessageKey;
  float DebugMessageDurationSeconds;
  float ViewportLeft;
  float ViewportTop;
  float ViewportWidth;
  float ViewportHeight;
  float PanelPadding;
  float StatsRefreshIntervalSeconds;
  float PolyCountRefreshIntervalSeconds;
  float BudgetLogIntervalSeconds;
  float IntervalResetElapsedSeconds;
  float BudgetScreenshotIntervalSeconds;
  float BudgetScreenshotDisabledIntervalSeconds;
  FString BudgetScreenshotIntervalCommandLineKey;
  FString BudgetScreenshotDirectory;
  FString BudgetScreenshotFileNameFormat;
  int32 BudgetScreenshotInitialIndex;
  int32 BudgetScreenshotIndexStep;
  float FramesPerSecondNumerator;
  float MinimumDeltaSeconds;
  float InitialDeltaSeconds;
  int32 InitialFramesPerSecond;
  int32 EmptyStackDepth;
  int32 EmptyPolyCount;
  int32 EmptyTriangleCount;
  int32 MeshLodIndex;
  int32 ForcedLodAutomaticModel;
  int32 LodModelIndexOffset;
  int32 ProcMeshFirstSectionIndex;
  int32 ProcMeshSectionStep;
  int32 TriangleIndexDivisor;
  int32 ZOrder;
  int32 FontSize;
  int32 FramesPerSecondMediumThreshold;
  int32 FramesPerSecondHighThreshold;
  int32 StackDepthMediumThreshold;
  int32 StackDepthHighThreshold;
  int32 PolyCountMediumThreshold;
  int32 PolyCountHighThreshold;
  bool bRemoveDpIScale;
  bool bAutoWrapText;
  bool bBudgetScreenshotCreateDirectoryTree;
  bool bBudgetScreenshotShowUI;
  bool bBudgetScreenshotAddFilenameSuffix;
  FLinearColor PanelColor;
  FLinearColor TextColor;
  FLinearColor LowValueColor;
  FLinearColor MediumValueColor;
  FLinearColor HighValueColor;
};

struct FUIRuntimeSettings {
  FString PlayerRoleLabel;
  FString SystemRoleLabel;
  FString NpcRoleLabel;
  FString UnknownRoleLabel;
  FString ChatMessageFormat;
  FString HistoryRoleSeparator;
  int32 HistoryTextStartOffset;
  int32 HistoryMinimumRoleIndex;
  FString PlaceholderTitle;
  FString PlaceholderPlayerLine;
  FString PlaceholderNpcReply;
  FString ConversationTitleFormat;
  FString PlayerLineFormat;
  FString NpcReplyFormat;
  FString ConnectionMessageFormat;
  FString UnboundDialogueError;
  FString PayloadIdFormat;
  FLinearColor PanelColor;
  FLinearColor TitleColor;
  FLinearColor PlayerColor;
  FLinearColor SystemColor;
  FLinearColor NpcColor;
  FLinearColor UnknownColor;
  FLinearColor RuntimeReplyColor;
  float PanelPadding;
  float TitleSize;
  float BodySize;
  FRuntimeStatsOverlaySettings StatsOverlay;
};

inline bool operator==(const FRuntimeStatsOverlaySettings &Left,
                       const FRuntimeStatsOverlaySettings &Right) {
  return Left.FramesPerSecondLabel == Right.FramesPerSecondLabel &&
         Left.StackDepthLabel == Right.StackDepthLabel &&
         Left.PolyCountLabel == Right.PolyCountLabel &&
         Left.LabelValueSeparator == Right.LabelValueSeparator &&
         Left.ValueFormat == Right.ValueFormat &&
         Left.DebugMessageFormat == Right.DebugMessageFormat &&
         Left.DebugMessageKey == Right.DebugMessageKey &&
         FMath::IsNearlyEqual(Left.DebugMessageDurationSeconds,
                              Right.DebugMessageDurationSeconds) &&
         FMath::IsNearlyEqual(Left.ViewportLeft, Right.ViewportLeft) &&
         FMath::IsNearlyEqual(Left.ViewportTop, Right.ViewportTop) &&
         FMath::IsNearlyEqual(Left.ViewportWidth, Right.ViewportWidth) &&
         FMath::IsNearlyEqual(Left.ViewportHeight, Right.ViewportHeight) &&
         FMath::IsNearlyEqual(Left.PanelPadding, Right.PanelPadding) &&
         FMath::IsNearlyEqual(Left.StatsRefreshIntervalSeconds,
                              Right.StatsRefreshIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.PolyCountRefreshIntervalSeconds,
                              Right.PolyCountRefreshIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetLogIntervalSeconds,
                              Right.BudgetLogIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.IntervalResetElapsedSeconds,
                              Right.IntervalResetElapsedSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetScreenshotIntervalSeconds,
                              Right.BudgetScreenshotIntervalSeconds) &&
         FMath::IsNearlyEqual(
             Left.BudgetScreenshotDisabledIntervalSeconds,
             Right.BudgetScreenshotDisabledIntervalSeconds) &&
         Left.BudgetScreenshotIntervalCommandLineKey ==
             Right.BudgetScreenshotIntervalCommandLineKey &&
         Left.BudgetScreenshotDirectory == Right.BudgetScreenshotDirectory &&
         Left.BudgetScreenshotFileNameFormat ==
             Right.BudgetScreenshotFileNameFormat &&
         Left.BudgetScreenshotInitialIndex ==
             Right.BudgetScreenshotInitialIndex &&
         Left.BudgetScreenshotIndexStep == Right.BudgetScreenshotIndexStep &&
         FMath::IsNearlyEqual(Left.FramesPerSecondNumerator,
                              Right.FramesPerSecondNumerator) &&
         FMath::IsNearlyEqual(Left.MinimumDeltaSeconds,
                              Right.MinimumDeltaSeconds) &&
         FMath::IsNearlyEqual(Left.InitialDeltaSeconds,
                              Right.InitialDeltaSeconds) &&
         Left.InitialFramesPerSecond == Right.InitialFramesPerSecond &&
         Left.EmptyStackDepth == Right.EmptyStackDepth &&
         Left.EmptyPolyCount == Right.EmptyPolyCount &&
         Left.EmptyTriangleCount == Right.EmptyTriangleCount &&
         Left.MeshLodIndex == Right.MeshLodIndex &&
         Left.ForcedLodAutomaticModel == Right.ForcedLodAutomaticModel &&
         Left.LodModelIndexOffset == Right.LodModelIndexOffset &&
         Left.ProcMeshFirstSectionIndex == Right.ProcMeshFirstSectionIndex &&
         Left.ProcMeshSectionStep == Right.ProcMeshSectionStep &&
         Left.TriangleIndexDivisor == Right.TriangleIndexDivisor &&
         Left.ZOrder == Right.ZOrder && Left.FontSize == Right.FontSize &&
         Left.FramesPerSecondMediumThreshold ==
             Right.FramesPerSecondMediumThreshold &&
         Left.FramesPerSecondHighThreshold ==
             Right.FramesPerSecondHighThreshold &&
         Left.StackDepthMediumThreshold == Right.StackDepthMediumThreshold &&
         Left.StackDepthHighThreshold == Right.StackDepthHighThreshold &&
         Left.PolyCountMediumThreshold == Right.PolyCountMediumThreshold &&
         Left.PolyCountHighThreshold == Right.PolyCountHighThreshold &&
         Left.bRemoveDpIScale == Right.bRemoveDpIScale &&
         Left.bAutoWrapText == Right.bAutoWrapText &&
         Left.bBudgetScreenshotCreateDirectoryTree ==
             Right.bBudgetScreenshotCreateDirectoryTree &&
         Left.bBudgetScreenshotShowUI == Right.bBudgetScreenshotShowUI &&
         Left.bBudgetScreenshotAddFilenameSuffix ==
             Right.bBudgetScreenshotAddFilenameSuffix &&
         Left.PanelColor == Right.PanelColor &&
         Left.TextColor == Right.TextColor &&
         Left.LowValueColor == Right.LowValueColor &&
         Left.MediumValueColor == Right.MediumValueColor &&
         Left.HighValueColor == Right.HighValueColor;
}

inline bool operator!=(const FRuntimeStatsOverlaySettings &Left,
                       const FRuntimeStatsOverlaySettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FUIRuntimeSettings &Left,
                       const FUIRuntimeSettings &Right) {
  return Left.PlayerRoleLabel == Right.PlayerRoleLabel &&
         Left.SystemRoleLabel == Right.SystemRoleLabel &&
         Left.NpcRoleLabel == Right.NpcRoleLabel &&
         Left.UnknownRoleLabel == Right.UnknownRoleLabel &&
         Left.ChatMessageFormat == Right.ChatMessageFormat &&
         Left.HistoryRoleSeparator == Right.HistoryRoleSeparator &&
         Left.HistoryTextStartOffset == Right.HistoryTextStartOffset &&
         Left.HistoryMinimumRoleIndex == Right.HistoryMinimumRoleIndex &&
         Left.PlaceholderTitle == Right.PlaceholderTitle &&
         Left.PlaceholderPlayerLine == Right.PlaceholderPlayerLine &&
         Left.PlaceholderNpcReply == Right.PlaceholderNpcReply &&
         Left.ConversationTitleFormat == Right.ConversationTitleFormat &&
         Left.PlayerLineFormat == Right.PlayerLineFormat &&
         Left.NpcReplyFormat == Right.NpcReplyFormat &&
         Left.ConnectionMessageFormat == Right.ConnectionMessageFormat &&
         Left.UnboundDialogueError == Right.UnboundDialogueError &&
         Left.PayloadIdFormat == Right.PayloadIdFormat &&
         Left.PanelColor == Right.PanelColor &&
         Left.TitleColor == Right.TitleColor &&
         Left.PlayerColor == Right.PlayerColor &&
         Left.SystemColor == Right.SystemColor &&
         Left.NpcColor == Right.NpcColor &&
         Left.UnknownColor == Right.UnknownColor &&
         Left.RuntimeReplyColor == Right.RuntimeReplyColor &&
         FMath::IsNearlyEqual(Left.PanelPadding, Right.PanelPadding) &&
         FMath::IsNearlyEqual(Left.TitleSize, Right.TitleSize) &&
         FMath::IsNearlyEqual(Left.BodySize, Right.BodySize) &&
         Left.StatsOverlay == Right.StatsOverlay;
}

inline bool operator!=(const FUIRuntimeSettings &Left,
                       const FUIRuntimeSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
