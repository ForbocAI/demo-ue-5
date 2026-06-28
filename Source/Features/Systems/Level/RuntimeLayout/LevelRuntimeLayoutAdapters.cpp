#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLabelAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeSectionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

struct FLevelRuntimeLayoutFieldsRequest {
  func::Maybe<FLevelRuntimeSectionSeed> Terrain =
      func::nothing<FLevelRuntimeSectionSeed>();
  func::Maybe<FLevelRuntimeSectionSeed> Town =
      func::nothing<FLevelRuntimeSectionSeed>();
  func::Maybe<FLevelRuntimeSectionSeed> Mine =
      func::nothing<FLevelRuntimeSectionSeed>();
  func::Maybe<TArray<TSharedPtr<FJsonValue>>> OverlayValues =
      func::nothing<TArray<TSharedPtr<FJsonValue>>>();
};

/**
 * @brief Maps authored overlay labels into typed label seeds.
 *
 * @signature func::Maybe<TArray<FLevelRuntimeLabelSeed>> ReadOverlayLabels(const TArray<TSharedPtr<FJsonValue>> &Values)
 *
 * User story: As a Level adapter maintainer, overlay labels use the same
 * neutral required-array mapping primitive as section labels.
 */
func::Maybe<TArray<FLevelRuntimeLabelSeed>>
ReadOverlayLabels(const TArray<TSharedPtr<FJsonValue>> &Values) {
  return JsonValues::MapRequiredJsonValuesWith<FLevelRuntimeLabelSeed>(
      TEXT("overlay_labels"),
      [](const TSharedPtr<FJsonObject> &LabelObject) {
        return LabelFromJson({LabelObject});
      })(Values);
}

/**
 * @brief Creates a layout seed from validated section and overlay fields.
 *
 * @signature func::Maybe<FLevelRuntimeLayoutSeed> BuildLayoutSeed(const FLevelRuntimeLayoutFieldsRequest &Request)
 *
 * User story: As a Runtime thunk author, complete authored layout data becomes
 * one typed payload before dispatching actions into the RTK store.
 */
func::Maybe<FLevelRuntimeLayoutSeed>
BuildLayoutSeed(const FLevelRuntimeLayoutFieldsRequest &Request) {
  return Request.Terrain.hasValue && Request.Town.hasValue &&
                 Request.Mine.hasValue && Request.OverlayValues.hasValue
             ? func::mbind(
                   ReadOverlayLabels(Request.OverlayValues.value),
                   [Request](
                       const TArray<FLevelRuntimeLabelSeed> &OverlayLabels) {
                     FLevelRuntimeLayoutSeed Seed;
                     Seed.Terrain = Request.Terrain.value;
                     Seed.Town = Request.Town.value;
                     Seed.Mine = Request.Mine.value;
                     Seed.OverlayLabels = OverlayLabels;
                     return func::just(Seed);
                   })
             : func::nothing<FLevelRuntimeLayoutSeed>();
}

} // namespace

func::Maybe<FLevelRuntimeLayoutSeed>
LayoutFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return BuildLayoutSeed(
      {ReadSection({Request.Object, TEXT("terrain")}),
       ReadSection({Request.Object, TEXT("town")}),
       ReadSection({Request.Object, TEXT("mine")}),
       JsonValues::ReadRequiredArray(
           {Request.Object, TEXT("overlay_labels")})});
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
