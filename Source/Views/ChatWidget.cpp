#include "Views/ChatWidget.h"

#include "Features/Systems/UI/UIReducers.h"

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

  const ForbocAI::Demo::Level::FUIBindDialogueViewModel Model =
      ForbocAI::Demo::Level::UIReducers::BuildBindDialogueViewModel(
          BoundDialogue ? BoundDialogue->Persona : FString(),
          BoundDialogue ? BoundDialogue->GetConversationHistory()
                        : TArray<FString>(),
          BoundDialogue != nullptr);
  ApplyBindDialogueViewModel(Model);
}

void UChatWidget::AddChatMessage(const FString &Role, const FString &Text) {
  AddChatMessageViewModel(
      ForbocAI::Demo::Level::UIReducers::BuildChatMessageViewModel(Role,
                                                                   Text));
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
  ApplyChatInputViewModel(
      ForbocAI::Demo::Level::UIReducers::BuildChatInputViewModel(
          Text, CommitMethod, BoundDialogue != nullptr));
}

void UChatWidget::HandleDialogueResponse(const FString &NPCText) {
  AddChatMessageViewModel(
      ForbocAI::Demo::Level::UIReducers::BuildDialogueResponseViewModel(
          BoundDialogue ? BoundDialogue->Persona : FString(), NPCText,
          BoundDialogue != nullptr)
          .Message);
}

void UChatWidget::ApplyBindDialogueViewModel(
    const ForbocAI::Demo::Level::FUIBindDialogueViewModel &Model) {
  return !Model.bBound
             ? void()
             : [this, &Model]() {
    NPCNameLabel
        ? (NPCNameLabel->SetText(FText::FromString(Model.Persona)), void())
        : void();

    BoundDialogue->DialogueResponse.AddUniqueDynamic(
        this, &UChatWidget::HandleDialogueResponse);

    AddChatMessageViewModel(Model.ConnectionMessage);

    for (const ForbocAI::Demo::UI::FChatMessageViewModel &Message :
         Model.HistoryMessages) {
      AddChatMessageViewModel(Message);
    }
  }();
}

void UChatWidget::ApplyChatInputViewModel(
    const ForbocAI::Demo::Level::FUIChatInputViewModel &Model) {
  return !Model.bShouldSend
             ? void()
             : [this, &Model]() {
    AddChatMessageViewModel(Model.PlayerMessage);

    ChatInputBox ? (ChatInputBox->SetText(FText::GetEmpty()), void()) : void();

    BoundDialogue ? (BoundDialogue->SendDialogue(Model.InputText), void())
                  : AddChatMessageViewModel(Model.ErrorMessage);
  }();
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
