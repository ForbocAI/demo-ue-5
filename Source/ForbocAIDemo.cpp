#include "ForbocAIDemo.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocAIDemo, Log, All);

class FForbocAIDemoModule : public FDefaultGameModuleImpl {
public:
  /** User Story: As a forboc aidemo consumer, I need to invoke startup module through a stable signature so the forboc aidemo workflow remains explicit and composable. @fn virtual void StartupModule() override */
  virtual void StartupModule() override {
    FDefaultGameModuleImpl::StartupModule();
    const ForbocAI::Game::Data::FSettings Settings =
        ForbocAI::Game::Data::SettingsAdapters::
            LoadSettings();
    UE_LOG(LogForbocAIDemo, Display, TEXT("%s"),
           *Settings.Text.Startup.SdkEnabled);
  }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FForbocAIDemoModule, ForbocAIDemo, "ForbocAIDemo");
