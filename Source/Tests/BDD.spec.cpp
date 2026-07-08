#include "Misc/AutomationTest.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"

namespace {

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
