#include "ForbocAIDemo.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "ForbocAILog.h"
#include "Modules/ModuleManager.h"

class FForbocAIDemoModule : public FDefaultGameModuleImpl {
public:
  virtual void StartupModule() override {
    FDefaultGameModuleImpl::StartupModule();
    const ForbocAI::Game::Data::FRuntimeSettings Settings =
        ForbocAI::Game::Data::RuntimeSettingsAdapters::
            LoadRuntimeSettings();
    UE_LOG(LogForbocAIRedux, Display, TEXT("%s"),
           *Settings.RuntimeText.StartupSdkEnabled);
  }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FForbocAIDemoModule, ForbocAIDemo, "ForbocAIDemo");
