#include "ForbocAIDemo.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "ForbocAILog.h"
#include "Modules/ModuleManager.h"

class FForbocAIDemoModule : public FDefaultGameModuleImpl {
public:
  virtual void StartupModule() override {
    FDefaultGameModuleImpl::StartupModule();
    const ForbocAI::Game::Data::FSettings Settings =
        ForbocAI::Game::Data::SettingsAdapters::
            LoadSettings();
    UE_LOG(LogForbocAIRedux, Display, TEXT("%s"),
           *Settings.Text.StartupSdkEnabled);
  }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FForbocAIDemoModule, ForbocAIDemo, "ForbocAIDemo");
