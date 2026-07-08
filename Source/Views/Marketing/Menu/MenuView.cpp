// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Runtime capture state and side effects stay in PlayerRuntimeControllerView.

#include "Views/Marketing/Menu/MenuView.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Input/Events.h"
#include "InputCoreTypes.h"
#include "Views/Player/Controller/ControllerView.h"

namespace FG = ForbocAI::Game::Level;

void URuntimeMarketingMenuWidget::NativeConstruct() {
  Super::NativeConstruct();

  const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings =
      FG::RuntimeSelectors::SelectMarketingCaptureSettings();
  SetIsFocusable(true);
  check(WidgetTree);
  WidgetTree->RootWidget
      ? void()
      : [&]() {
          UBorder *PanelElement =
              WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
          UVerticalBox *StackElement =
              WidgetTree->ConstructWidget<UVerticalBox>(
                  UVerticalBox::StaticClass());

          UTextBlock *TitleElement =
              BuildTextElement(Settings.MenuTitle, Settings.MenuTitleColor,
                               Settings.MenuTitleSize);
          RetakeButton = BuildButtonElement(
              Settings.MenuRetakeButtonText, Settings.MenuButtonTextColor,
              Settings.MenuButtonTextSize);
          ResumeButton = BuildButtonElement(Settings.MenuResumeButtonText,
                                            Settings.MenuButtonTextColor,
                                            Settings.MenuButtonTextSize);

          PanelElement->SetPadding(FMargin(Settings.MenuPanelPadding));
          PanelElement->SetBrushColor(Settings.MenuPanelColor);
          PanelElement->SetContent(StackElement);
          StackElement->AddChildToVerticalBox(TitleElement);
          StackElement->AddChildToVerticalBox(RetakeButton);
          StackElement->AddChildToVerticalBox(ResumeButton);
          WidgetTree->RootWidget = PanelElement;
        }();

  RetakeButton ? (RetakeButton->OnClicked.RemoveDynamic(
                      this,
                      &URuntimeMarketingMenuWidget::HandleRetakeClicked),
                  RetakeButton->OnClicked.AddDynamic(
                      this,
                      &URuntimeMarketingMenuWidget::HandleRetakeClicked),
                  void())
               : void();
  ResumeButton ? (ResumeButton->OnClicked.RemoveDynamic(
                      this,
                      &URuntimeMarketingMenuWidget::HandleResumeClicked),
                  ResumeButton->OnClicked.AddDynamic(
                      this,
                      &URuntimeMarketingMenuWidget::HandleResumeClicked),
                  void())
               : void();
}

FReply URuntimeMarketingMenuWidget::NativeOnKeyDown(
    const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) {
  const bool bDismissRequested =
      InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::P;
  return bDismissRequested
             ? ([this]() {
                 HandleResumeClicked();
                 return FReply::Handled();
               })()
             : Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void URuntimeMarketingMenuWidget::BindController(
    APlayerRuntimeControllerView *Controller) {
  BoundController = Controller;
}

void URuntimeMarketingMenuWidget::HandleRetakeClicked() {
  BoundController ? (BoundController->RetakeMarketingBrochureShots(), void())
                  : void();
}

void URuntimeMarketingMenuWidget::HandleResumeClicked() {
  BoundController ? (BoundController->DismissMarketingCaptureMenu(), void())
                  : void();
}

UTextBlock *URuntimeMarketingMenuWidget::BuildTextElement(
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

UButton *URuntimeMarketingMenuWidget::BuildButtonElement(
    const FString &Text, const FLinearColor &Color, float Size) {
  check(WidgetTree);
  UButton *Button =
      WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
  check(Button);
  UTextBlock *Label = BuildTextElement(Text, Color, Size);
  Button->SetContent(Label);
  return Button;
}
