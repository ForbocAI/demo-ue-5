#include "Misc/AutomationTest.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"

namespace {

/** User Story: As a tests consumer, I need to invoke bdd automation settings through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Tests::FBddSettings & BddAutomationSettings() */
const ForbocAI::Game::Data::Automation::Tests::FBddSettings &
BddAutomationSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.Tests.Bdd;
}

} // namespace

DEFINE_SPEC(FNewBDDSpec, BddAutomationSettings().Spec,
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags_ApplicationContextMask)

/** User Story: As a tests consumer, I need to invoke define through a stable signature so the tests workflow remains explicit and composable. @fn void FNewBDDSpec::Define() */
void FNewBDDSpec::Define() {
  const ForbocAI::Game::Data::Automation::Tests::FBddSettings &Settings =
      BddAutomationSettings();
  Describe(Settings.Group, [this, &Settings]() {
    It(Settings.Case, [this, &Settings]() {
      bool bWorks = true;
      TestTrue(Settings.Assertion, bWorks);
    });
  });
}
