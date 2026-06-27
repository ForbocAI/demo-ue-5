#include "Views/RuntimeChatWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Features/Systems/UI/UISelectors.h"

void URuntimeChatWidget::NativeConstruct() {
  Super::NativeConstruct();

  if (WidgetTree && !WidgetTree->RootWidget) {
    UBorder *PanelElement =
        WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
    UVerticalBox *StackElement =
        WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

    const ForbocAI::Demo::UI::FRuntimeConversationViewModel Conversation =
        ForbocAI::Demo::Level::UISelectors::
            SelectRuntimeConversationPlaceholder();

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
  }
}

void URuntimeChatWidget::ShowConversationViewModel(
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation) {
  SetVisibility(ESlateVisibility::Visible);
  ApplyConversationViewModel(Conversation);
}

void URuntimeChatWidget::ApplyConversationViewModel(
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation) {
  if (TitleTextElement) {
    TitleTextElement->SetText(FText::FromString(Conversation.Title));
  }
  if (PlayerTextElement) {
    PlayerTextElement->SetText(FText::FromString(Conversation.PlayerLine));
  }
  if (ReplyTextElement) {
    ReplyTextElement->SetText(FText::FromString(Conversation.NpcReply));
  }
}

UTextBlock *URuntimeChatWidget::BuildTextElement(
    const FString &Text, const FLinearColor &Color, float Size) {
  UTextBlock *Element =
      WidgetTree ? WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass())
                 : nullptr;
  if (Element) {
    Element->SetText(FText::FromString(Text));
    Element->SetColorAndOpacity(FSlateColor(Color));
    Element->SetAutoWrapText(true);
    FSlateFontInfo Font = Element->GetFont();
    Font.Size = Size;
    Element->SetFont(Font);
  }
  return Element;
}
