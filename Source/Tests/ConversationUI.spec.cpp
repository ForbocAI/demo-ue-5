#include "CoreMinimal.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/UI/Chat/Rendering/ChatRenderingSlice.h"
#include "Features/Systems/UI/Chat/ChatSlice.h"
#include "Features/Systems/UI/Conversation/UIConversationSlice.h"
#include "Misc/AutomationTest.h"

namespace {

using FConversationUISettings =
    ForbocAI::Game::Data::Automation::Conversation::UI::FSettings;

struct FConversationUILabelCursor {
  const TArray<FString> *Labels = nullptr;
  int32 Index = int32();

  FString Next() {
    check(Labels != nullptr);
    check(Labels->IsValidIndex(Index));
    const FString Label = (*Labels)[Index];
    ++Index;
    return Label;
  }
};

FConversationUILabelCursor ConversationUILabels(
    const TArray<FString> &Labels) {
  return {&Labels, int32()};
}

} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConversationUIBuildsViewModels,
    "ForbocAI.UI.ConversationViewModels",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FConversationUIBuildsViewModels::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const ForbocAI::Game::Data::FUISettings UISettings = Settings.UI;
  const FConversationUISettings Automation =
      Settings.Automation.ConversationUI;
  FConversationUILabelCursor Labels =
      ConversationUILabels(Automation.Assertions);
  const ForbocAI::Game::UI::FChatMessageViewModel PlayerMessage =
      ForbocAI::Game::Level::UIReducers::ReduceChatMessageViewModel(
          {Automation.PlayerMessage.Role, Automation.PlayerMessage.Text},
          UISettings);
  TestEqual(Labels.Next(), PlayerMessage.Text,
            Automation.PlayerMessage.ExpectedText);
  TestEqual(Labels.Next(), PlayerMessage.Color.G,
            Automation.PlayerMessage.ExpectedGreen);

  const TArray<ForbocAI::Game::UI::FChatMessageViewModel> Messages =
      ForbocAI::Game::Level::UIReducers::ReduceChatHistoryViewModels(
          {Automation.History.Lines}, UISettings);
  TestEqual(Labels.Next(), Messages.Num(), Automation.History.ExpectedCount);
  TestEqual(Labels.Next(), Messages[Automation.History.Tagged.Index].Text,
            Automation.History.Tagged.ExpectedText);
  TestEqual(Labels.Next(), Messages[Automation.History.Untagged.Index].Text,
            Automation.History.Untagged.ExpectedText);

  TestEqual(Labels.Next(),
            ForbocAI::Game::Level::UIReducers::
                ReduceNormalizedSubmittedChatText(
                    {FText::FromString(Automation.Submitted.Input),
                     ETextCommit::OnEnter, true}),
            Automation.Submitted.ExpectedText);

  const ForbocAI::Game::UI::FRuntimeConversationViewModel Conversation =
      ForbocAI::Game::Level::UIReducers::ReduceRuntimeConversationViewModel(
          {Automation.Dialogue.SpeakerName, Automation.Dialogue.SpeakerRole,
           Automation.Dialogue.PlayerLine, Automation.Dialogue.NpcReply},
          UISettings);
  TestEqual(Labels.Next(), Conversation.Text.Title,
            Automation.Dialogue.ExpectedTitle);
  TestEqual(Labels.Next(), Conversation.Text.PlayerLine,
            Automation.Dialogue.ExpectedPlayerLine);
  TestEqual(Labels.Next(), Conversation.Text.NpcReply,
            Automation.Dialogue.ExpectedNpcReply);

  return true;
}
