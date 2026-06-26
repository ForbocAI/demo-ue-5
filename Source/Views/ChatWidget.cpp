#include "Views/ChatWidget.h"

#include "Features/Systems/UI/ConversationUI.h"

void UChatWidget::NativeConstruct() {
  Super::NativeConstruct();

  ChatInputBox
      ? (ChatInputBox->OnTextCommitted.AddDynamic(
             this, &UChatWidget::OnInputCommitted),
         void())
      : void();
}

void UChatWidget::NativeDestruct() {
  if (BoundDialogue) {
    BoundDialogue->DialogueResponse.RemoveDynamic(
        this, &UChatWidget::HandleDialogueResponse);
  }
  Super::NativeDestruct();
}

void UChatWidget::BindToDialogue(UDialogueComponent *InDialogueComp) {
  if (BoundDialogue) {
    BoundDialogue->DialogueResponse.RemoveDynamic(
        this, &UChatWidget::HandleDialogueResponse);
  }

  BoundDialogue = InDialogueComp;

  return !BoundDialogue
             ? void()
             : [this]() {
    // Set NPC name label
    NPCNameLabel
        ? (NPCNameLabel->SetText(
               FText::FromString(BoundDialogue->Persona)),
           void())
        : void();

    BoundDialogue->DialogueResponse.AddUniqueDynamic(
        this, &UChatWidget::HandleDialogueResponse);

    // Add system message
    AddChatMessage(TEXT("System"),
                   FString::Printf(TEXT("Connected to %s"),
                                   *BoundDialogue->Persona));

    const TArray<ForbocAI::Demo::UI::FChatMessageViewModel> History =
        ForbocAI::Demo::UI::BuildChatHistoryViewModels(
            BoundDialogue->GetConversationHistory());

    for (const ForbocAI::Demo::UI::FChatMessageViewModel &Message : History) {
      AddChatMessageViewModel(Message);
    }
  }();
}

void UChatWidget::AddChatMessage(const FString &Role, const FString &Text) {
  AddChatMessageViewModel(
      ForbocAI::Demo::UI::BuildChatMessageViewModel(Role, Text));
}

void UChatWidget::AddChatMessageViewModel(
    const ForbocAI::Demo::UI::FChatMessageViewModel &Message) {
  return !ChatScrollBox
             ? void()
             : [this, &Message]() {
    UTextBlock *MessageBlock = CreateMessageBlock(Message);

    MessageBlock
        ? (ChatScrollBox->AddChild(MessageBlock),
           // Auto-scroll to bottom
           ChatScrollBox->ScrollToEnd(), void())
        : void();
  }();
}

void UChatWidget::ClearChat() {
  ChatScrollBox ? (ChatScrollBox->ClearChildren(), void()) : void();
}

void UChatWidget::OnInputCommitted(const FText &Text,
                                    ETextCommit::Type CommitMethod) {
  // Only process on Enter key
  return CommitMethod != ETextCommit::OnEnter
             ? void()
             : [this, &Text]() {
    FString InputText = ForbocAI::Demo::UI::NormalizeSubmittedChatText(Text);

    return InputText.IsEmpty()
               ? void()
               : [this, &InputText]() {
      // Add player message to chat log
      AddChatMessage(TEXT("Player"), InputText);

      // Clear input box
      ChatInputBox ? (ChatInputBox->SetText(FText::GetEmpty()), void())
                   : void();

      // Send via DialogueComponent. It uses the SDK only when the feature gate is open.
      BoundDialogue
          ? (BoundDialogue->SendDialogue(InputText), void())
          : AddChatMessage(TEXT("System"),
                           TEXT("Error: No dialogue component bound"));
    }();
  }();
}

void UChatWidget::HandleDialogueResponse(const FString &NPCText) {
  AddChatMessage(BoundDialogue ? BoundDialogue->Persona : TEXT("NPC"),
                 NPCText);
}

UTextBlock *UChatWidget::CreateMessageBlock(
    const ForbocAI::Demo::UI::FChatMessageViewModel &Message) {
  UTextBlock *Block = NewObject<UTextBlock>(this);

  return !Block
             ? nullptr
             : [Block, &Message]() -> UTextBlock * {
    Block->SetText(FText::FromString(Message.Text));
    Block->SetColorAndOpacity(FSlateColor(Message.Color));
    Block->SetAutoWrapText(true);

    return Block;
  }();
}
