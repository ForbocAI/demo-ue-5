#include "CoreMinimal.h"
#include "Core/ue_fp.hpp"
#include "Engine/SkeletalMesh.h"
#include "Features/Systems/Speech/SpeechAdapters.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/PackageName.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "UObject/SoftObjectPath.h"

namespace {
constexpr const TCHAR *RuntimeMapPackage = TEXT("/Game/Map/Maps/Runtime");
constexpr const TCHAR *RuntimeGameModeClass =
    TEXT("/Script/ForbocAIDemo.LevelGameModeView");
constexpr const TCHAR *RuntimePlayerControllerClass =
    TEXT("/Script/ForbocAIDemo.PlayerRuntimeControllerView");
constexpr const TCHAR *RuntimeLevelViewClass =
    TEXT("/Script/ForbocAIDemo.RuntimeLevelView");
constexpr const TCHAR *RuntimeTownspersonViewClass =
    TEXT("/Script/ForbocAIDemo.TownspersonView");
constexpr const TCHAR *RuntimeSpeechPresenterClass =
    TEXT("/Script/ForbocAIDemo.RuntimeSpeechPresenterView");
constexpr const TCHAR *RuntimeChatWidgetClass =
    TEXT("/Script/ForbocAIDemo.RuntimeChatWidget");
constexpr const TCHAR *MannyMeshPath =
    TEXT("/Game/Characters/Mannequins/Meshes/"
         "SKM_Manny_Simple.SKM_Manny_Simple");
constexpr const TCHAR *ClassicHorseMeshPath =
    TEXT("/Game/Characters/Horses/ClassicHorse/"
         "SkeletalMesh/ClassicHorse_Equipment_A."
         "ClassicHorse_Equipment_A");
constexpr const TCHAR *MountedRiderMeshPath =
    TEXT("/Game/Characters/Horses/ClassicHorse/Rider/"
         "SkeletalMesh/Rider_SkeletalMesh.Rider_SkeletalMesh");
constexpr const TCHAR *EngineNightSkyDomeMeshPath =
    TEXT("/Engine/MapTemplates/Sky/SM_SkySphere.SM_SkySphere");
constexpr const TCHAR *EngineNightSkyDomeMaterialPath =
    TEXT("/Engine/MapTemplates/Sky/"
         "M_Procedural_Sky_Night.M_Procedural_Sky_Night");
constexpr const TCHAR *EngineMoonPixelMeshPath =
    TEXT("/Engine/BasicShapes/Cube.Cube");
constexpr const TCHAR *EngineMoonDiscMaterialPath =
    TEXT("/Engine/EngineDebugMaterials/"
         "LevelColorationUnlitMaterial.LevelColorationUnlitMaterial");
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

USkeletalMesh *LoadSkeletalMesh(const TCHAR *ObjectPath) {
  return Cast<USkeletalMesh>(FSoftObjectPath(ObjectPath).TryLoad());
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

void AuditNativeSkeletalMeshLods(FAutomationTestBase &Test,
                                 const FString &Label,
                                 const TCHAR *ObjectPath) {
  USkeletalMesh *Mesh = LoadSkeletalMesh(ObjectPath);
  Test.TestNotNull(*FString::Printf(TEXT("%s skeletal mesh loads"), *Label),
                   Mesh);
  const FSkeletalMeshRenderData *RenderData =
      Mesh ? Mesh->GetResourceForRendering() : nullptr;
  Test.TestTrue(*FString::Printf(TEXT("%s exposes native render LOD data"),
                                 *Label),
                RenderData != nullptr &&
                    RenderData->LODRenderData.Num() > 0);
  func::match(
      func::from_nullable_value<const FSkeletalMeshRenderData *>(
          RenderData, RenderData != nullptr),
      [&Test, &Label](const FSkeletalMeshRenderData *RenderDataValue) {
        Test.AddInfo(FString::Printf(
            TEXT("Native LOD audit %s lod_count=%d"), *Label,
            RenderDataValue->LODRenderData.Num()));
        const TArray<int32> LodIndices =
            func::index_range(RenderDataValue->LODRenderData.Num());
        func::for_each_indexed(
            LodIndices, static_cast<size_t>(LodIndices.Num()),
            [&Test, &Label,
             RenderDataValue](const int32 &LodIndex) {
              const FSkeletalMeshLODRenderData &Lod =
                  RenderDataValue->LODRenderData[LodIndex];
              Test.AddInfo(FString::Printf(
                  TEXT("Native LOD audit %s lod=%d faces=%d vertices=%u"),
                  *Label, LodIndex, Lod.GetTotalFaces(), Lod.GetNumVertices()));
            });
      },
      []() {});
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
  TestTrue(TEXT("Runtime player controller native class loads"),
           ClassLoads(RuntimePlayerControllerClass));
  TestTrue(TEXT("Runtime level native class loads"),
           ClassLoads(RuntimeLevelViewClass));
  TestTrue(TEXT("Runtime townsperson native class loads"),
           ClassLoads(RuntimeTownspersonViewClass));
  TestTrue(TEXT("Runtime speech presenter native class loads"),
           ClassLoads(RuntimeSpeechPresenterClass));
  TestTrue(TEXT("Runtime chat widget native class loads"),
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

  TestTrue(TEXT("Manny mesh asset loads"), AssetLoads(MannyMeshPath));
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
           AssetLoads(ClassicHorseMeshPath));
  TestTrue(TEXT("Classic Horse walk animation loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Animations/"
                           "Horse_Walk.Horse_Walk")));
  TestTrue(TEXT("Classic Horse idle animation loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Animations/"
                           "Horse_Idle.Horse_Idle")));
  TestTrue(TEXT("Mounted rider skeletal mesh loads"),
           AssetLoads(MountedRiderMeshPath));
  TestTrue(TEXT("Mounted rider walk animation loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Rider/"
                           "Animations/Rider_Walk.Rider_Walk")));
  TestTrue(TEXT("Mounted rider idle animation loads"),
           AssetLoads(TEXT("/Game/Characters/Horses/ClassicHorse/Rider/"
                           "Animations/Rider_Idle.Rider_Idle")));
  TestTrue(TEXT("Built-in night sky dome mesh loads"),
           AssetLoads(EngineNightSkyDomeMeshPath));
  TestTrue(TEXT("Built-in night sky dome material loads"),
           AssetLoads(EngineNightSkyDomeMaterialPath));
  TestTrue(TEXT("Built-in moon pixel mesh loads"),
           AssetLoads(EngineMoonPixelMeshPath));
  TestTrue(TEXT("Built-in moon disc material loads"),
           AssetLoads(EngineMoonDiscMaterialPath));
  AuditNativeSkeletalMeshLods(*this, TEXT("Manny"), MannyMeshPath);
  AuditNativeSkeletalMeshLods(*this, TEXT("Classic Horse"),
                              ClassicHorseMeshPath);
  AuditNativeSkeletalMeshLods(*this, TEXT("Mounted Rider"),
                              MountedRiderMeshPath);

  TestFalse(TEXT("Old UE4 mannequin package is not tracked"),
            PackageExists(TEXT("/Game/Character/Mesh/SK_Mannequin")));
  TestFalse(TEXT("Old ThirdPerson animation package is not tracked"),
            PackageExists(TEXT("/Game/Animations/ThirdPerson_AnimBP")));
  TestFalse(TEXT("Old ThirdPerson template mesh package is not tracked"),
            PackageExists(
                TEXT("/Game/ThirdPersonTemplate/Meshes/Ramp_StaticMesh")));

  return true;
}
