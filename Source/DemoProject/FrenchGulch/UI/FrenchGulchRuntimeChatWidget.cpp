#include "FrenchGulch/UI/FrenchGulchRuntimeChatWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UFrenchGulchRuntimeChatWidget::NativeConstruct() {
  Super::NativeConstruct();

  if (WidgetTree && !WidgetTree->RootWidget) {
    UBorder *Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
    UVerticalBox *Stack =
        WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());

    TitleText =
        BuildTextBlock(TEXT("French Gulch Conversation"), FLinearColor::Yellow,
                       22.0f);
    PlayerText =
        BuildTextBlock(TEXT("Player: ..."), FLinearColor(0.0f, 0.85f, 0.95f),
                       18.0f);
    ReplyText =
        BuildTextBlock(TEXT("NPC: ..."), FLinearColor(1.0f, 0.72f, 0.25f),
                       18.0f);

    Panel->SetPadding(FMargin(16.0f));
    Panel->SetBrushColor(FLinearColor(0.02f, 0.02f, 0.025f, 0.78f));
    Panel->SetContent(Stack);
    Stack->AddChildToVerticalBox(TitleText);
    Stack->AddChildToVerticalBox(PlayerText);
    Stack->AddChildToVerticalBox(ReplyText);
    WidgetTree->RootWidget = Panel;
  }
}

void UFrenchGulchRuntimeChatWidget::ShowConversation(
    const FString &NpcName, const FString &Role, const FString &PlayerLine,
    const FString &NpcReply) {
  SetVisibility(ESlateVisibility::Visible);

  if (TitleText) {
    TitleText->SetText(
        FText::FromString(FString::Printf(TEXT("%s - %s"), *NpcName, *Role)));
  }
  if (PlayerText) {
    PlayerText->SetText(
        FText::FromString(FString::Printf(TEXT("Player: %s"), *PlayerLine)));
  }
  if (ReplyText) {
    ReplyText->SetText(
        FText::FromString(FString::Printf(TEXT("NPC: %s"), *NpcReply)));
  }
}

UTextBlock *UFrenchGulchRuntimeChatWidget::BuildTextBlock(
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
