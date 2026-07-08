#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"
#include "Features/Components/Data/Settings/UI/Marketing/MarketingTypes.h"
#include "Features/Components/Data/Settings/UI/Fly/FlyTypes.h"
#include "Features/Components/Data/Settings/UI/Scale/Audit/AuditTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FUISettings {
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
  FLinearColor ReplyColor;
  float PanelPadding;
  float TitleSize;
  float BodySize;
  FStatsOverlaySettings StatsOverlay;
  FMarketingCaptureSettings MarketingCapture;
  FFlyModeSettings FlyMode;
  FScaleAuditCaptureSettings ScaleAuditCapture;

inline bool operator==(const FUISettings &Left,
                       const FUISettings &Right) {
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
         Left.ReplyColor == Right.ReplyColor &&
         FMath::IsNearlyEqual(Left.PanelPadding, Right.PanelPadding) &&
         FMath::IsNearlyEqual(Left.TitleSize, Right.TitleSize) &&
         FMath::IsNearlyEqual(Left.BodySize, Right.BodySize) &&
         Left.StatsOverlay == Right.StatsOverlay &&
         Left.MarketingCapture == Right.MarketingCapture &&
         Left.FlyMode == Right.FlyMode &&
         Left.ScaleAuditCapture == Right.ScaleAuditCapture;
}

inline bool operator!=(const FUISettings &Left,
                       const FUISettings &Right) {
  return !(Left == Right);

} // namespace Data
} // namespace Game
} // namespace ForbocAI
