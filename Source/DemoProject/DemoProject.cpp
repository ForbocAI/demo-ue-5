#include "DemoProject.h"
#include "ForbocAILog.h"
#include "Modules/ModuleManager.h"

class FDemoProjectModule : public FDefaultGameModuleImpl {
public:
  virtual void StartupModule() override {
    FDefaultGameModuleImpl::StartupModule();
    UE_LOG(LogForbocAIRedux, Display,
           TEXT("DemoProject: Redux logger active. Filter the Output Log by LogForbocAIRedux to inspect protocol actions and state deltas."));
  }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FDemoProjectModule, DemoProject, "DemoProject");
