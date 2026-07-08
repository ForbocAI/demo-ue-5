#pragma once

#include "Features/Components/Data/Settings/Automation/Bot/Functional/Core/Types.h"
#include "Features/Components/Data/Settings/Automation/Content/Assets/Types.h"
#include "Features/Components/Data/Settings/Automation/Conversation/UI/Types.h"
#include "Features/Components/Data/Settings/Automation/Pipeline/Types.h"
#include "Features/Components/Data/Settings/Automation/Protocol/Loop/Types.h"
#include "Features/Components/Data/Settings/Automation/Rtk/Compliance/Types.h"
#include "Features/Components/Data/Settings/Automation/Store/Types.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {

struct FSettings {
  Store::FSettings Store;
  Content::Assets::FSettings ContentAssets;
  Rtk::Compliance::FSettings RtkCompliance;
  Bot::Functional::Core::FSettings BotFunctionalCore;
  Pipeline::FSettings Pipeline;
  Conversation::UI::FSettings ConversationUI;
  Protocol::Loop::FSettings ProtocolLoop;
};

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Store == Right.Store &&
         Left.ContentAssets == Right.ContentAssets &&
         Left.RtkCompliance == Right.RtkCompliance &&
         Left.BotFunctionalCore == Right.BotFunctionalCore &&
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
