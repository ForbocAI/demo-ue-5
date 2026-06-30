#include "DemoProject.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "ForbocAILog.h"
#include "Modules/ModuleManager.h"

class FDemoProjectModule : public FDefaultGameModuleImpl {
public:
  virtual void StartupModule() override {
    FDefaultGameModuleImpl::StartupModule();
    const ForbocAI::Demo::Data::FDemoRuntimeSettings Settings =
        ForbocAI::Demo::Data::RuntimeSettingsAdapters::
            LoadDemoRuntimeSettings();
    UE_LOG(LogForbocAIRedux, Display, TEXT("%s"),
           *Settings.RuntimeText.StartupSdkEnabled);
  }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FDemoProjectModule, DemoProject, "DemoProject");
