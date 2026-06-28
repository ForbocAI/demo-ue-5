#include "CoreMinimal.h"
#include "Features/Systems/Speech/SpeechAdapters.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/PackageName.h"
#include "UObject/SoftObjectPath.h"

namespace {
constexpr const TCHAR *RuntimeMapPackage = TEXT("/Game/Map/Maps/Runtime");
constexpr const TCHAR *RuntimeGameModeClass =
    TEXT("/Game/Blueprints/BP_LevelGameMode.BP_LevelGameMode_C");
constexpr const TCHAR *RuntimePlayerControllerClass =
    TEXT("/Game/Blueprints/BP_PlayerRuntimeController."
         "BP_PlayerRuntimeController_C");
constexpr const TCHAR *RuntimeLevelViewClass =
    TEXT("/Game/Blueprints/BP_RuntimeLevelView.BP_RuntimeLevelView_C");
constexpr const TCHAR *RuntimeTownspersonViewClass =
    TEXT("/Game/Blueprints/BP_TownspersonView.BP_TownspersonView_C");
constexpr const TCHAR *RuntimeSpeechPresenterClass =
    TEXT("/Game/Blueprints/BP_SpeechPresenter.BP_SpeechPresenter_C");
constexpr const TCHAR *RuntimeChatWidgetClass =
    TEXT("/Game/UI/WBP_Chat.WBP_Chat_C");
constexpr const TCHAR *GameMapsSettingsSection =
    TEXT("/Script/EngineSettings.GameMapsSettings");

bool PackageExists(const TCHAR *PackageName) {
  return FPackageName::DoesPackageExist(PackageName);
}

bool AssetLoads(const TCHAR *ObjectPath) {
  return FSoftObjectPath(ObjectPath).TryLoad() != nullptr;
}

bool ClassLoads(const TCHAR *ClassPath) {
  return FSoftClassPath(ClassPath).TryLoadClass<UObject>() != nullptr;
}

UClass *LoadClass(const TCHAR *ClassPath) {
  return FSoftClassPath(ClassPath).TryLoadClass<UObject>();
}

bool ActorClassHasSpeechComponent(const TCHAR *ClassPath) {
  const UClass *Class = LoadClass(ClassPath);
  const AActor *DefaultActor =
      Class ? Cast<AActor>(Class->GetDefaultObject()) : nullptr;
  return DefaultActor &&
         DefaultActor->FindComponentByClass<USpeechComponent>() != nullptr;
}

FString ConfigValue(const TCHAR *Section, const TCHAR *Key) {
  FString Value;
  GConfig->GetString(Section, Key, Value, GEngineIni);
  return Value;
}
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContentAssetsProjectOwnedRuntimeSurface,
    "ForbocAI.Content.ProjectOwnedRuntimeSurface",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FContentAssetsProjectOwnedRuntimeSurface::RunTest(
    const FString &Parameters) {
  TestTrue(TEXT("Default French Gulch runtime map is tracked"),
           PackageExists(RuntimeMapPackage));
  TestTrue(TEXT("Default runtime game mode class loads"),
           ClassLoads(RuntimeGameModeClass));
  TestTrue(TEXT("Runtime player controller Blueprint class loads"),
           ClassLoads(RuntimePlayerControllerClass));
  TestTrue(TEXT("Runtime level Blueprint class loads"),
           ClassLoads(RuntimeLevelViewClass));
  TestTrue(TEXT("Runtime townsperson Blueprint class loads"),
           ClassLoads(RuntimeTownspersonViewClass));
  TestTrue(TEXT("Runtime speech presenter Blueprint class loads"),
           ClassLoads(RuntimeSpeechPresenterClass));
  TestTrue(TEXT("Runtime WBP_Chat widget class loads"),
           ClassLoads(RuntimeChatWidgetClass));
  TestTrue(TEXT("Runtime speech presenter owns USpeechComponent"),
           ActorClassHasSpeechComponent(RuntimeSpeechPresenterClass));
  TestEqual(TEXT("Game default map opens the tracked runtime map"),
            ConfigValue(GameMapsSettingsSection, TEXT("GameDefaultMap")),
            FString(RuntimeMapPackage));
  TestEqual(TEXT("Editor startup map opens the tracked runtime map"),
            ConfigValue(GameMapsSettingsSection, TEXT("EditorStartupMap")),
            FString(RuntimeMapPackage));
  TestEqual(TEXT("Server default map opens the tracked runtime map"),
            ConfigValue(GameMapsSettingsSection, TEXT("ServerDefaultMap")),
            FString(RuntimeMapPackage));
  TestEqual(TEXT("Runtime map uses source-controlled game mode"),
            ConfigValue(GameMapsSettingsSection,
                        TEXT("GlobalDefaultGameMode")),
            FString(RuntimeGameModeClass));

  TestTrue(TEXT("Manny mesh asset loads"),
           AssetLoads(TEXT("/Game/Characters/Mannequins/Meshes/"
                           "SKM_Manny_Simple.SKM_Manny_Simple")));
  TestTrue(TEXT("Manny unarmed animation blueprint asset loads"),
           AssetLoads(TEXT("/Game/Characters/Mannequins/Anims/Unarmed/"
                           "ABP_Unarmed.ABP_Unarmed")));

  TestTrue(TEXT("Third Person move input action loads"),
           AssetLoads(TEXT("/Game/Input/Actions/IA_Move.IA_Move")));
  TestTrue(TEXT("Third Person look input action loads"),
           AssetLoads(TEXT("/Game/Input/Actions/IA_Look.IA_Look")));
  TestTrue(TEXT("Third Person mouse-look input action loads"),
           AssetLoads(TEXT("/Game/Input/Actions/IA_MouseLook.IA_MouseLook")));
  TestTrue(TEXT("Third Person jump input action loads"),
           AssetLoads(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump")));
  TestTrue(TEXT("Third Person default mapping context loads"),
           AssetLoads(TEXT("/Game/Input/IMC_Default.IMC_Default")));
  TestTrue(TEXT("Third Person mouse-look mapping context loads"),
           AssetLoads(TEXT("/Game/Input/IMC_MouseLook.IMC_MouseLook")));

  TestTrue(TEXT("Classic Horse skeletal mesh loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/"
                           "SkeletalMesh/ClassicHorse_Equipment_A."
                           "ClassicHorse_Equipment_A")));
  TestTrue(TEXT("Classic Horse walk animation loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Animations/"
                           "Horse_Walk.Horse_Walk")));
  TestTrue(TEXT("Classic Horse idle animation loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Animations/"
                           "Horse_Idle.Horse_Idle")));
  TestTrue(TEXT("Mounted rider skeletal mesh loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Rider/"
                           "SkeletalMesh/Rider_SkeletalMesh."
                           "Rider_SkeletalMesh")));
  TestTrue(TEXT("Mounted rider walk animation loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Rider/"
                           "Animations/Rider_Walk.Rider_Walk")));
  TestTrue(TEXT("Mounted rider idle animation loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Rider/"
                           "Animations/Rider_Idle.Rider_Idle")));

  TestFalse(TEXT("Legacy UE4 mannequin package is not tracked"),
            PackageExists(TEXT("/Game/Character/Mesh/SK_Mannequin")));
  TestFalse(TEXT("Legacy ThirdPerson animation package is not tracked"),
            PackageExists(TEXT("/Game/Animations/ThirdPerson_AnimBP")));
  TestFalse(TEXT("Legacy ThirdPerson template mesh package is not tracked"),
            PackageExists(
                TEXT("/Game/ThirdPersonTemplate/Meshes/Ramp_StaticMesh")));

  return true;
}
