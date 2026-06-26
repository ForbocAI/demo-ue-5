#include "DemoProject.h"
#include "Modules/ModuleManager.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "ForbocAILog.h"
#endif

class FDemoProjectModule : public FDefaultGameModuleImpl {
public:
  virtual void StartupModule() override {
    FDefaultGameModuleImpl::StartupModule();
#if WITH_FORBOC_AI_SDK_DEMO
    UE_LOG(LogForbocAIRedux, Display,
           TEXT("DemoProject: Redux logger active. Filter the Output Log by LogForbocAIRedux to inspect protocol actions and state deltas."));
#else
    UE_LOG(LogTemp, Display,
           TEXT("DemoProject: UE SDK feature gate closed. Running offline French Gulch prototype."));
#endif
  }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FDemoProjectModule, DemoProject, "DemoProject");
