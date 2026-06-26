#include "CoreMinimal.h"
#include "Features/Systems/UI/ConversationUI.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConversationUIBuildsViewModels,
    "ForbocAI.UI.ConversationViewModels",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FConversationUIBuildsViewModels::RunTest(const FString &Parameters) {
  const ForbocAI::Demo::UI::FChatMessageViewModel PlayerMessage =
      ForbocAI::Demo::UI::BuildChatMessageViewModel(TEXT("Player"),
                                                    TEXT("Hello"));
  TestEqual(TEXT("Player message is role-prefixed"), PlayerMessage.Text,
            FString(TEXT("[Player] Hello")));
  TestEqual(TEXT("Player message uses cyan channel"), PlayerMessage.Color.G,
            0.85f);

  const TArray<FString> History = {TEXT("NPC: Welcome back"),
                                   TEXT("Unlabeled message")};
  const TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Messages =
      ForbocAI::Demo::UI::BuildChatHistoryViewModels(History);
  TestEqual(TEXT("History produces view models"), Messages.Num(), 2);
  TestEqual(TEXT("Tagged history keeps role"), Messages[0].Text,
            FString(TEXT("[NPC] Welcome back")));
  TestEqual(TEXT("Untagged history is marked unknown"), Messages[1].Text,
            FString(TEXT("[Unknown] Unlabeled message")));

  TestEqual(TEXT("Submitted text is trimmed"),
            ForbocAI::Demo::UI::NormalizeSubmittedChatText(
                FText::FromString(TEXT("  Ask around  "))),
            FString(TEXT("Ask around")));

  const ForbocAI::Demo::UI::FRuntimeConversationViewModel Conversation =
      ForbocAI::Demo::UI::BuildRuntimeConversationViewModel(
          TEXT("Clara Bell"), TEXT("Postmaster"), TEXT("Any mail?"),
          TEXT("Only dust today."));
  TestEqual(TEXT("Conversation title is composed"), Conversation.Title,
            FString(TEXT("Clara Bell - Postmaster")));
  TestEqual(TEXT("Conversation player line is labeled"),
            Conversation.PlayerLine, FString(TEXT("Player: Any mail?")));
  TestEqual(TEXT("Conversation reply is labeled"), Conversation.NpcReply,
            FString(TEXT("NPC: Only dust today.")));

  return true;
}
