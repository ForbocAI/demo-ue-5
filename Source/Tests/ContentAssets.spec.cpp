#include "CoreMinimal.h"
#include "Core/ue_fp.hpp"
#include "Engine/SkeletalMesh.h"
#include "Features/Components/Data/Settings/Adapters.h"
#include "Features/Systems/Speech/Adapters.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/PackageName.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "UObject/SoftObjectPath.h"

namespace {

using FContentAssetExpectationSettings =
    ForbocAI::Game::Data::FContentAssetExpectationSettings;
using FContentAssetsAutomationSettings =
    ForbocAI::Game::Data::FContentAssetsAutomationSettings;
using FContentConfigExpectationSettings =
    ForbocAI::Game::Data::FContentConfigExpectationSettings;

const FContentAssetsAutomationSettings &ContentAssetsSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.ContentAssets;
}

FString Label(const FString &Format, const FString &Value) {
  return FString::Format(*Format, {FStringFormatArg(Value)});
}

FString Label(const FString &Format, const FString &First, int32 Second) {
  return FString::Format(
      *Format, {FStringFormatArg(First), FStringFormatArg(Second)});
}

FString Label(const FString &Format, const FString &First, int32 Second,
              int32 Third, uint32 Fourth) {
  return FString::Format(*Format,
                         {FStringFormatArg(First), FStringFormatArg(Second),
                          FStringFormatArg(Third), FStringFormatArg(Fourth)});
}

bool PackageExists(const FString &PackageName) {
  return FPackageName::DoesPackageExist(*PackageName);
}

bool AssetLoads(const FString &ObjectPath) {
  return FSoftObjectPath(ObjectPath).TryLoad() != nullptr;
}

bool ClassLoads(const FString &ClassPath) {
  return FSoftClassPath(ClassPath).TryLoadClass<UObject>() != nullptr;
}

UClass *LoadClass(const FString &ClassPath) {
  return FSoftClassPath(ClassPath).TryLoadClass<UObject>();
}

USkeletalMesh *LoadSkeletalMesh(const FString &ObjectPath) {
  return Cast<USkeletalMesh>(FSoftObjectPath(ObjectPath).TryLoad());
}

bool ActorClassHasSpeechComponent(const FString &ClassPath) {
  const UClass *Class = LoadClass(ClassPath);
  const AActor *DefaultActor =
      Class ? Cast<AActor>(Class->GetDefaultObject()) : nullptr;
  return DefaultActor &&
         DefaultActor->FindComponentByClass<USpeechComponent>() != nullptr;
}

FString ConfigValue(const FString &Section, const FString &Key) {
  FString Value;
  GConfig->GetString(*Section, *Key, Value, GEngineIni);
  return Value;
}

void AuditNativeSkeletalMeshLods(
    FAutomationTestBase &Test,
    const FContentAssetsAutomationSettings &Settings,
    const FContentAssetExpectationSettings &Expectation) {
  USkeletalMesh *Mesh = LoadSkeletalMesh(Expectation.Path);
  Test.TestNotNull(*Label(Settings.SkeletalMeshLoadsLabelFormat,
                          Expectation.Label),
                   Mesh);
  const FSkeletalMeshRenderData *RenderData =
      Mesh ? Mesh->GetResourceForRendering() : nullptr;
  Test.TestTrue(*Label(Settings.SkeletalMeshLodDataLabelFormat,
                       Expectation.Label),
                RenderData != nullptr &&
                    !RenderData->LODRenderData.IsEmpty());
  func::match(
      func::from_nullable_value<const FSkeletalMeshRenderData *>(
          RenderData, RenderData != nullptr),
      [&Test, &Settings,
       &Expectation](const FSkeletalMeshRenderData *RenderDataValue) {
        Test.AddInfo(Label(Settings.NativeLodAuditCountFormat,
                           Expectation.Label,
                           RenderDataValue->LODRenderData.Num()));
        const TArray<int32> LodIndices =
            func::index_range(RenderDataValue->LODRenderData.Num());
        func::for_each_indexed(
            LodIndices, static_cast<size_t>(LodIndices.Num()),
            [&Test, &Settings, &Expectation,
             RenderDataValue](const int32 &LodIndex) {
              const FSkeletalMeshLODRenderData &Lod =
                  RenderDataValue->LODRenderData[LodIndex];
              Test.AddInfo(Label(Settings.NativeLodAuditEntryFormat,
                                 Expectation.Label, LodIndex,
                                 Lod.GetTotalFaces(), Lod.GetNumVertices()));
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
  const FContentAssetsAutomationSettings &Settings = ContentAssetsSettings();

  func::for_each_indexed(
      Settings.Packages, static_cast<size_t>(Settings.Packages.Num()),
      [this](const FContentAssetExpectationSettings &Expectation) {
        TestTrue(Expectation.Label, PackageExists(Expectation.Path));
      });
  func::for_each_indexed(
      Settings.Classes, static_cast<size_t>(Settings.Classes.Num()),
      [this](const FContentAssetExpectationSettings &Expectation) {
        TestTrue(Expectation.Label, ClassLoads(Expectation.Path));
      });
  func::for_each_indexed(
      Settings.SpeechComponentClasses,
      static_cast<size_t>(Settings.SpeechComponentClasses.Num()),
      [this](const FContentAssetExpectationSettings &Expectation) {
        TestTrue(Expectation.Label,
                 ActorClassHasSpeechComponent(Expectation.Path));
      });
  func::for_each_indexed(
      Settings.ConfigValues, static_cast<size_t>(Settings.ConfigValues.Num()),
      [this](const FContentConfigExpectationSettings &Expectation) {
        TestEqual(Expectation.Label,
                  ConfigValue(Expectation.Section, Expectation.Key),
                  Expectation.Expected);
      });
  func::for_each_indexed(
      Settings.Assets, static_cast<size_t>(Settings.Assets.Num()),
      [this](const FContentAssetExpectationSettings &Expectation) {
        TestTrue(Expectation.Label, AssetLoads(Expectation.Path));
      });
  func::for_each_indexed(
      Settings.SkeletalMeshLods,
      static_cast<size_t>(Settings.SkeletalMeshLods.Num()),
      [this, &Settings](const FContentAssetExpectationSettings &Expectation) {
        AuditNativeSkeletalMeshLods(*this, Settings, Expectation);
      });
  func::for_each_indexed(
      Settings.MissingPackages,
      static_cast<size_t>(Settings.MissingPackages.Num()),
      [this](const FContentAssetExpectationSettings &Expectation) {
        TestFalse(Expectation.Label, PackageExists(Expectation.Path));
      });

  return true;
}
