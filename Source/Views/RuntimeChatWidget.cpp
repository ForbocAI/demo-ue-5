// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/RuntimeChatWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Store.h"

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
        ForbocAI::Game::Level::RuntimeSelectors::SelectRuntimeConversation(
            ForbocAI::Game::Level::Store::GetStore().getState());

    TitleTextElement = BuildTextElement(Conversation.Title,
                                        Conversation.TitleColor,
                                        Conversation.TitleSize);
    PlayerTextElement = BuildTextElement(Conversation.PlayerLine,
                                         Conversation.PlayerColor,
                                         Conversation.BodySize);
    ReplyTextElement = BuildTextElement(Conversation.NpcReply,
                                        Conversation.ReplyColor,
                                        Conversation.BodySize);

    PanelElement->SetPadding(FMargin(Conversation.PanelPadding));
    PanelElement->SetBrushColor(Conversation.PanelColor);
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
  TitleTextElement->SetText(FText::FromString(Conversation.Title));
  PlayerTextElement->SetText(FText::FromString(Conversation.PlayerLine));
  ReplyTextElement->SetText(FText::FromString(Conversation.NpcReply));
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
