#include "CoreMinimal.h"
#include "Core/ue_fp.hpp"
#include "Engine/SkeletalMesh.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/Speech/Component/ComponentAdapters.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/PackageName.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "UObject/SoftObjectPath.h"

namespace {

using FContentAssetExpectation =
    ForbocAI::Game::Data::Automation::Content::Assets::FPackage;
using FContentAssetsSettings =
    ForbocAI::Game::Data::Automation::Content::Assets::FSettings;
using FContentConfigExpectation =
    ForbocAI::Game::Data::Automation::Content::Assets::FConfig;

const FContentAssetsSettings &ContentAssetsSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.ContentAssets;
}

FString Label(const FString &Format,
              std::initializer_list<FStringFormatArg> Args) {
  return FString::Format(*Format, Args);
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
    const FContentAssetsSettings &Settings,
    const FContentAssetExpectation &Expectation) {
  USkeletalMesh *Mesh = LoadSkeletalMesh(Expectation.Path);
  Test.TestNotNull(*Label(Settings.SkeletalMeshLoadsLabelFormat,
                          {FStringFormatArg(Expectation.Label)}),
                   Mesh);
  const FSkeletalMeshRenderData *RenderData =
      Mesh ? Mesh->GetResourceForRendering() : nullptr;
  Test.TestTrue(*Label(Settings.SkeletalMeshLodDataLabelFormat,
                       {FStringFormatArg(Expectation.Label)}),
                RenderData != nullptr &&
                    !RenderData->LODRenderData.IsEmpty());
  func::match(
      func::from_nullable_value<const FSkeletalMeshRenderData *>(
          RenderData, RenderData != nullptr),
      [&Test, &Settings,
       &Expectation](const FSkeletalMeshRenderData *RenderDataValue) {
        Test.AddInfo(Label(
            Settings.NativeLodAuditCountFormat,
            {FStringFormatArg(Expectation.Label),
             FStringFormatArg(RenderDataValue->LODRenderData.Num())}));
        const TArray<int32> LodIndices =
            func::index_range(RenderDataValue->LODRenderData.Num());
        func::for_each_indexed(
            LodIndices, static_cast<size_t>(LodIndices.Num()),
            [&Test, &Settings, &Expectation,
             RenderDataValue](const int32 &LodIndex) {
              const FSkeletalMeshLODRenderData &Lod =
                  RenderDataValue->LODRenderData[LodIndex];
              Test.AddInfo(Label(
                  Settings.NativeLodAuditEntryFormat,
                  {FStringFormatArg(Expectation.Label),
                   FStringFormatArg(LodIndex),
                   FStringFormatArg(Lod.GetTotalFaces()),
                   FStringFormatArg(Lod.GetNumVertices())}));
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
  const FContentAssetsSettings &Settings = ContentAssetsSettings();

  func::for_each_indexed(
      Settings.Packages, static_cast<size_t>(Settings.Packages.Num()),
      [this](const FContentAssetExpectation &Expectation) {
        TestTrue(Expectation.Label, PackageExists(Expectation.Path));
      });
  func::for_each_indexed(
      Settings.Classes, static_cast<size_t>(Settings.Classes.Num()),
      [this](const FContentAssetExpectation &Expectation) {
        TestTrue(Expectation.Label, ClassLoads(Expectation.Path));
      });
  func::for_each_indexed(
      Settings.SpeechComponentClasses,
      static_cast<size_t>(Settings.SpeechComponentClasses.Num()),
      [this](const FContentAssetExpectation &Expectation) {
        TestTrue(Expectation.Label,
                 ActorClassHasSpeechComponent(Expectation.Path));
      });
  func::for_each_indexed(
      Settings.ConfigValues, static_cast<size_t>(Settings.ConfigValues.Num()),
      [this](const FContentConfigExpectation &Expectation) {
        TestEqual(Expectation.Label,
                  ConfigValue(Expectation.Section, Expectation.Key),
                  Expectation.Expected);
      });
  func::for_each_indexed(
      Settings.Assets, static_cast<size_t>(Settings.Assets.Num()),
      [this](const FContentAssetExpectation &Expectation) {
        TestTrue(Expectation.Label, AssetLoads(Expectation.Path));
      });
  func::for_each_indexed(
      Settings.SkeletalMeshLods,
      static_cast<size_t>(Settings.SkeletalMeshLods.Num()),
      [this, &Settings](const FContentAssetExpectation &Expectation) {
        AuditNativeSkeletalMeshLods(*this, Settings, Expectation);
      });
  func::for_each_indexed(
      Settings.MissingPackages,
      static_cast<size_t>(Settings.MissingPackages.Num()),
      [this](const FContentAssetExpectation &Expectation) {
        TestFalse(Expectation.Label, PackageExists(Expectation.Path));
      });

  return true;
}
