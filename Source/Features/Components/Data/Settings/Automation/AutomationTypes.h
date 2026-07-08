#pragma once

#include "Features/Components/Data/Settings/Automation/Bot/AutomationBotTypes.h"
#include "Features/Components/Data/Settings/Automation/Content/Assets/AssetsTypes.h"
#include "Features/Components/Data/Settings/Automation/Conversation/UI/ConversationUITypes.h"
#include "Features/Components/Data/Settings/Automation/Pipeline/AutomationPipelineTypes.h"
#include "Features/Components/Data/Settings/Automation/Protocol/Loop/LoopTypes.h"
#include "Features/Components/Data/Settings/Automation/Rtk/Compliance/ComplianceTypes.h"
#include "Features/Components/Data/Settings/Automation/Store/StoreTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {

struct FSettings {
  Store::FSettings Store;
  Content::Assets::FSettings ContentAssets;
  Rtk::Compliance::FSettings RtkCompliance;
  Bot::FSettings Bot;
  Pipeline::FSettings Pipeline;
  Conversation::UI::FSettings ConversationUI;
  Protocol::Loop::FSettings ProtocolLoop;
};

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Store == Right.Store &&
         Left.ContentAssets == Right.ContentAssets &&
         Left.RtkCompliance == Right.RtkCompliance &&
         Left.Bot == Right.Bot &&
         Left.Pipeline == Right.Pipeline &&
         Left.ConversationUI == Right.ConversationUI &&
         Left.ProtocolLoop == Right.ProtocolLoop;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
