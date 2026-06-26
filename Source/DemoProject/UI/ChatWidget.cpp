#include "UI/ChatWidget.h"

void UChatWidget::NativeConstruct() {
  Super::NativeConstruct();

  // Wire up input box commit event
  ChatInputBox
      ? (ChatInputBox->OnTextCommitted.AddDynamic(
             this, &UChatWidget::OnInputCommitted),
         void())
      : void();
}

void UChatWidget::BindToDialogue(UDialogueComponent *InDialogueComp) {
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

    // Add system message
    AddChatMessage(TEXT("System"),
                   FString::Printf(TEXT("Connected to %s"),
                                   *BoundDialogue->Persona));

    // Populate existing conversation history
    const TArray<FString> History =
        BoundDialogue->GetConversationHistory();

    const auto PopulateRecursive =
        [this, &History](int32 Idx, const auto &Self) -> void {
      return Idx >= History.Num()
                 ? void()
                 : ([this, &History, Idx]() {
                      // Parse "Role: Text" format
                      const FString &Entry = History[Idx];
                      int32 ColonIdx;
                      Entry.FindChar(TEXT(':'), ColonIdx);
                      ColonIdx > 0
                          ? AddChatMessage(Entry.Left(ColonIdx),
                                           Entry.Mid(ColonIdx + 2))
                          : AddChatMessage(TEXT("Unknown"), Entry);
                    }(),
                    Self(Idx + 1, Self));
    };
    PopulateRecursive(0, PopulateRecursive);
  }();
}

void UChatWidget::AddChatMessage(const FString &Role, const FString &Text) {
  return !ChatScrollBox
             ? void()
             : [this, &Role, &Text]() {
    UTextBlock *MessageBlock = CreateMessageBlock(Role, Text);

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
    FString InputText = Text.ToString().TrimStartAndEnd();

    return InputText.IsEmpty()
               ? void()
               : [this, &InputText]() {
      // Add player message to chat log
      AddChatMessage(TEXT("Player"), InputText);

      // Clear input box
      ChatInputBox->SetText(FText::GetEmpty());

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

UTextBlock *UChatWidget::CreateMessageBlock(const FString &Role,
                                             const FString &Text) {
  UTextBlock *Block = NewObject<UTextBlock>(this);

  return !Block
             ? nullptr
             : [this, Block, &Role, &Text]() -> UTextBlock * {
    FString FormattedText =
        FString::Printf(TEXT("[%s] %s"), *Role, *Text);
    Block->SetText(FText::FromString(FormattedText));

    // Color coding by role
    FSlateColor MessageColor =
        (Role == TEXT("Player"))
            ? FSlateColor(FLinearColor(0.0f, 0.85f, 0.95f)) // Cyan
        : (Role == TEXT("System"))
            ? FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f)) // Gray
            : FSlateColor(FLinearColor(1.0f, 0.75f, 0.2f)); // Amber

    Block->SetColorAndOpacity(MessageColor);

    // Auto-wrap for long messages
    Block->SetAutoWrapText(true);

    return Block;
  }();
}
