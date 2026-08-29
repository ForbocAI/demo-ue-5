#pragma once

#include "Features/Components/Data/Settings/UI/Fly/FlyTypes.h"
#include "Features/Components/Data/Settings/UI/Marketing/MarketingTypes.h"
#include "Features/Components/Data/Settings/UI/Root/Conversation/Format/ConversationFormatTypes.h"
#include "Features/Components/Data/Settings/UI/Root/Conversation/Layout/ConversationLayoutTypes.h"
#include "Features/Components/Data/Settings/UI/Root/History/HistoryTypes.h"
#include "Features/Components/Data/Settings/UI/Root/Placeholder/PlaceholderTypes.h"
#include "Features/Components/Data/Settings/UI/Root/Role/Colors/RoleColorTypes.h"
#include "Features/Components/Data/Settings/UI/Root/Roles/RoleTypes.h"
#include "Features/Components/Data/Settings/UI/Root/Surface/Colors/SurfaceColorTypes.h"
#include "Features/Components/Data/Settings/UI/Scale/Audit/AuditTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FUISettings {
  FRoleSettings Roles;
  FHistorySettings History;
  FPlaceholderSettings Placeholder;
  FConversationFormatSettings Format;
  FSurfaceColorSettings SurfaceColors;
  FRoleColorSettings RoleColors;
  FConversationLayoutSettings ConversationLayout;
  FOverlaySettings StatsOverlay;
  FMarketingCaptureSettings MarketingCapture;
  FModeSettings FlyMode;
  FAuditCaptureSettings ScaleAuditCapture;
};

/** User Story: As a settings ui root consumer, I need to compare values for equality through a stable signature so the settings ui root workflow remains explicit and composable. @fn inline bool operator==(const FUISettings &Left, const FUISettings &Right) */
inline bool operator==(const FUISettings &Left,
                       const FUISettings &Right) {
  return Left.Roles == Right.Roles && Left.History == Right.History &&
         Left.Placeholder == Right.Placeholder &&
         Left.Format == Right.Format &&
         Left.SurfaceColors == Right.SurfaceColors &&
         Left.RoleColors == Right.RoleColors &&
         Left.ConversationLayout == Right.ConversationLayout &&
         Left.StatsOverlay == Right.StatsOverlay &&
         Left.MarketingCapture == Right.MarketingCapture &&
         Left.FlyMode == Right.FlyMode &&
         Left.ScaleAuditCapture == Right.ScaleAuditCapture;
}

/** User Story: As a settings ui root consumer, I need to compare values for inequality through a stable signature so the settings ui root workflow remains explicit and composable. @fn inline bool operator!=(const FUISettings &Left, const FUISettings &Right) */
inline bool operator!=(const FUISettings &Left,
                       const FUISettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
