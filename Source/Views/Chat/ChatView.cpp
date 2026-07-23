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

/** User Story: As a views chat consumer, I need to invoke native construct through a stable signature so the views chat workflow remains explicit and composable. @fn void URuntimeChatWidget::NativeConstruct() */
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

/** User Story: As a views chat consumer, I need to invoke show conversation view model through a stable signature so the views chat workflow remains explicit and composable. @fn void URuntimeChatWidget::ShowConversationViewModel( const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation) */
void URuntimeChatWidget::ShowConversationViewModel(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation) {
  SetVisibility(ESlateVisibility::Visible);
  ApplyConversationViewModel(Conversation);
}

/** User Story: As a views chat consumer, I need to invoke apply conversation view model through a stable signature so the views chat workflow remains explicit and composable. @fn void URuntimeChatWidget::ApplyConversationViewModel( const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation) */
void URuntimeChatWidget::ApplyConversationViewModel(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation) {
  check(TitleTextElement);
  check(PlayerTextElement);
  check(ReplyTextElement);
  TitleTextElement->SetText(FText::FromString(Conversation.Text.Title));
  PlayerTextElement->SetText(FText::FromString(Conversation.Text.PlayerLine));
  ReplyTextElement->SetText(FText::FromString(Conversation.Text.NpcReply));
}

/** User Story: As a views chat consumer, I need to invoke build text element through a stable signature so the views chat workflow remains explicit and composable. @fn UTextBlock *URuntimeChatWidget::BuildTextElement( const FString &Text, const FLinearColor &Color, float Size) */
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
