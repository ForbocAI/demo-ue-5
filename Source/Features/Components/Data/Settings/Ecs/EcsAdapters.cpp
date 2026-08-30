#include "Features/Components/Data/Settings/Ecs/SettingsEcsAdapters.h"

#include "Core/fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace SettingsAdapters {
namespace {

/** User Story: As ECS settings composition, I need authored domain paths split into immutable registry segments. @fn TArray<FString> EcsDomainSegments(const FString &Path) */
TArray<FString> EcsDomainSegments(const FString &Path) {
  TArray<FString> Segments;
  const FString Separator = FString::Chr(TCHAR('/'));
  Path.ParseIntoArray(Segments, *Separator, true);
  return Segments;
}

/** User Story: As ECS settings composition, I need one authored registration projected into the ECS path contract. @fn ecs::FPathRegistration EcsDomainPathRegistration(const FDomainRegistrationSettings &Settings) */
ecs::FPathRegistration
EcsDomainPathRegistration(const FDomainRegistrationSettings &Settings) {
  ecs::FPathRegistration Registration;
  Registration.Segments = EcsDomainSegments(Settings.Path);
  Registration.Kind = static_cast<ecs::EKind>(Settings.Kind);
  return Registration;
}

} // namespace

/** User Story: As ECS initialization, I need every authored domain registration composed into one immutable registry graph. @fn ecs::FGraph EcsDomainRegistry(const FEcsSettings &Settings) */
ecs::FGraph EcsDomainRegistry(const FEcsSettings &Settings) {
  return ecs::createDomainRegistry(
      func::map_array<FDomainRegistrationSettings, ecs::FPathRegistration>(
          Settings.DomainRegistry, EcsDomainPathRegistration));
}

} // namespace SettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
