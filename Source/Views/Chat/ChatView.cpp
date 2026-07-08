// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Chat/ChatView.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Features/Systems/SystemsSelectors.h"

void URuntimeChatWidget::NativeConstruct() {
  Super::NativeConstruct();

  check(WidgetTree);
  WidgetTree->RootWidget
      ? void()
      : [&]() {
    UBorder *PanelElement =
        WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
    UVerticalBox *StackElement =
        WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

    const ForbocAI::Game::UI::FRuntimeConversationViewModel Conversation =
        ForbocAI::Game::Level::RuntimeSelectors::SelectRuntimeConversation();

    TitleTextElement = BuildTextElement(Conversation.Text.Title,
                                        Conversation.Colors.TitleColor,
                                        Conversation.Layout.TitleSize);
    PlayerTextElement = BuildTextElement(Conversation.Text.PlayerLine,
                                         Conversation.Colors.PlayerColor,
                                         Conversation.Layout.BodySize);
    ReplyTextElement = BuildTextElement(Conversation.Text.NpcReply,
                                        Conversation.Colors.ReplyColor,
                                        Conversation.Layout.BodySize);

    PanelElement->SetPadding(FMargin(Conversation.Layout.PanelPadding));
    PanelElement->SetBrushColor(Conversation.Colors.PanelColor);
    PanelElement->SetContent(StackElement);
    StackElement->AddChildToVerticalBox(TitleTextElement);
    StackElement->AddChildToVerticalBox(PlayerTextElement);
    StackElement->AddChildToVerticalBox(ReplyTextElement);
    WidgetTree->RootWidget = PanelElement;
  }();
}

void URuntimeChatWidget::ShowConversationViewModel(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation) {
  SetVisibility(ESlateVisibility::Visible);
  ApplyConversationViewModel(Conversation);
}

void URuntimeChatWidget::ApplyConversationViewModel(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation) {
  check(TitleTextElement);
  check(PlayerTextElement);
  check(ReplyTextElement);
  TitleTextElement->SetText(FText::FromString(Conversation.Text.Title));
  PlayerTextElement->SetText(FText::FromString(Conversation.Text.PlayerLine));
  ReplyTextElement->SetText(FText::FromString(Conversation.Text.NpcReply));
}

UTextBlock *URuntimeChatWidget::BuildTextElement(
    const FString &Text, const FLinearColor &Color, float Size) {
  check(WidgetTree);
  UTextBlock *Element =
      WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
  check(Element);
  Element->SetText(FText::FromString(Text));
  Element->SetColorAndOpacity(FSlateColor(Color));
  Element->SetAutoWrapText(true);
  FSlateFontInfo Font = Element->GetFont();
  Font.Size = Size;
  Element->SetFont(Font);
  return Element;
}
