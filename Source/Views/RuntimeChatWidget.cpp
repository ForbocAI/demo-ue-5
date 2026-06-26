#include "Views/RuntimeChatWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Features/Systems/UI/ConversationUI.h"

void URuntimeChatWidget::NativeConstruct() {
  Super::NativeConstruct();

  if (WidgetTree && !WidgetTree->RootWidget) {
    UBorder *Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
    UVerticalBox *Stack =
        WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

    const ForbocAI::Demo::UI::FRuntimeConversationViewModel Conversation =
        ForbocAI::Demo::UI::BuildRuntimeConversationPlaceholder();

    TitleText = BuildTextBlock(Conversation.Title, Conversation.TitleColor,
                               Conversation.TitleSize);
    PlayerText = BuildTextBlock(Conversation.PlayerLine,
                                Conversation.PlayerColor,
                                Conversation.BodySize);
    ReplyText = BuildTextBlock(Conversation.NpcReply, Conversation.ReplyColor,
                               Conversation.BodySize);

    Panel->SetPadding(FMargin(Conversation.PanelPadding));
    Panel->SetBrushColor(Conversation.PanelColor);
    Panel->SetContent(Stack);
    Stack->AddChildToVerticalBox(TitleText);
    Stack->AddChildToVerticalBox(PlayerText);
    Stack->AddChildToVerticalBox(ReplyText);
    WidgetTree->RootWidget = Panel;
  }
}

void URuntimeChatWidget::ShowConversation(
    const FString &NpcName, const FString &Role, const FString &PlayerLine,
    const FString &NpcReply) {
  SetVisibility(ESlateVisibility::Visible);
  ApplyConversationViewModel(
      ForbocAI::Demo::UI::BuildRuntimeConversationViewModel(
          NpcName, Role, PlayerLine, NpcReply));
}

void URuntimeChatWidget::ApplyConversationViewModel(
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation) {
  if (TitleText) {
    TitleText->SetText(FText::FromString(Conversation.Title));
  }
  if (PlayerText) {
    PlayerText->SetText(FText::FromString(Conversation.PlayerLine));
  }
  if (ReplyText) {
    ReplyText->SetText(FText::FromString(Conversation.NpcReply));
  }
}

UTextBlock *URuntimeChatWidget::BuildTextBlock(
    const FString &Text, const FLinearColor &Color, float Size) {
  UTextBlock *Block =
      WidgetTree ? WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass())
                 : nullptr;
  if (Block) {
    Block->SetText(FText::FromString(Text));
    Block->SetColorAndOpacity(FSlateColor(Color));
    Block->SetAutoWrapText(true);
    FSlateFontInfo Font = Block->GetFont();
    Font.Size = Size;
    Block->SetFont(Font);
  }
  return Block;
}
