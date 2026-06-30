#include "CoreMinimal.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Systems/UI/UIReducers.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConversationUIBuildsViewModels,
    "ForbocAI.UI.ConversationViewModels",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FConversationUIBuildsViewModels::RunTest(const FString &Parameters) {
  const ForbocAI::Demo::Data::FUIRuntimeSettings UISettings =
      ForbocAI::Demo::Data::RuntimeSettingsAdapters::LoadDemoRuntimeSettings()
          .UIRuntime;
  const ForbocAI::Demo::UI::FChatMessageViewModel PlayerMessage =
      ForbocAI::Demo::Level::UIReducers::ReduceChatMessageViewModel(
          {TEXT("Player"), TEXT("Hello")}, UISettings);
  TestEqual(TEXT("Player message is role-prefixed"), PlayerMessage.Text,
            FString(TEXT("[Player] Hello")));
  TestEqual(TEXT("Player message uses cyan channel"), PlayerMessage.Color.G,
            0.85f);

  const TArray<FString> History = {TEXT("NPC: Welcome back"),
                                   TEXT("Unlabeled message")};
  const TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Messages =
      ForbocAI::Demo::Level::UIReducers::ReduceChatHistoryViewModels(
          {History}, UISettings);
  TestEqual(TEXT("History produces view models"), Messages.Num(), 2);
  TestEqual(TEXT("Tagged history keeps role"), Messages[0].Text,
            FString(TEXT("[NPC] Welcome back")));
  TestEqual(TEXT("Untagged history is marked unknown"), Messages[1].Text,
            FString(TEXT("[Unknown] Unlabeled message")));

  TestEqual(TEXT("Submitted text is trimmed"),
            ForbocAI::Demo::Level::UIReducers::
                ReduceNormalizedSubmittedChatText(
                    {FText::FromString(TEXT("  Ask around  ")),
                     ETextCommit::OnEnter, true}),
            FString(TEXT("Ask around")));

  const ForbocAI::Demo::UI::FRuntimeConversationViewModel Conversation =
      ForbocAI::Demo::Level::UIReducers::ReduceRuntimeConversationViewModel(
          {TEXT("Clara Bell"), TEXT("Postmaster"), TEXT("Any mail?"),
           TEXT("Only dust today.")},
          UISettings);
  TestEqual(TEXT("Conversation title is composed"), Conversation.Title,
            FString(TEXT("Clara Bell - Postmaster")));
  TestEqual(TEXT("Conversation player line is labeled"),
            Conversation.PlayerLine, FString(TEXT("Player: Any mail?")));
  TestEqual(TEXT("Conversation reply is labeled"), Conversation.NpcReply,
            FString(TEXT("NPC: Only dust today.")));

  return true;
}
