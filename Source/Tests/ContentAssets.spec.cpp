#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Misc/PackageName.h"
#include "UObject/SoftObjectPath.h"

namespace {
bool PackageExists(const TCHAR *PackageName) {
  return FPackageName::DoesPackageExist(PackageName);
}

bool AssetLoads(const TCHAR *ObjectPath) {
  return FSoftObjectPath(ObjectPath).TryLoad() != nullptr;
}
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContentAssetsProjectOwnedRuntimeSurface,
    "ForbocAI.Content.ProjectOwnedRuntimeSurface",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FContentAssetsProjectOwnedRuntimeSurface::RunTest(
    const FString &Parameters) {
  TestTrue(TEXT("Default French Gulch runtime map is tracked"),
           PackageExists(TEXT("/Game/Map/Maps/Runtime")));

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
