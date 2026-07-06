#include "Misc/AutomationTest.h"

DEFINE_SPEC(FNewBDDSpec, "ForbocAI.Game.NewBDD",
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags_ApplicationContextMask)

void FNewBDDSpec::Define() {
  Describe("New Feature BDD", [this]() {
    It("Should prove BDD works in ForbocAIDemo", [this]() {
      bool bWorks = true;
      TestTrue("BDD Works", bWorks);
    });
  });
}
