#include "ForbocAIDemo.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"
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
    UE_LOG(LogForbocAIDemo, Display, TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C),
           *Settings.Text.Startup.SdkEnabled);
  }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FForbocAIDemoModule, ForbocAIDemo, FORBOCAI_DEMOUE5_AUTHORED_STRINGVA86AD88A8861);
