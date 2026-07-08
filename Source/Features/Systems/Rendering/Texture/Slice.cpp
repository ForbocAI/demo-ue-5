#include "Features/Systems/Rendering/Texture/Slice.h"

#include "Core/ue_fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingTextureReducers {
namespace {

inline FString RenderingReducerAtom(const char *Atom) {
  return FString(UTF8_TO_TCHAR(Atom));
}

template <typename Output> struct TRenderingTextValueDeclaration {
  FString Text;
  Output Value;

  TRenderingTextValueDeclaration(const char *InText, Output InValue)
      : Text(RenderingReducerAtom(InText)), Value(InValue) {}
};

template <typename Output> struct TRenderingTextValueRegistry;

template <> struct TRenderingTextValueRegistry<ELevelRetroTexture> {
  static const TArray<TRenderingTextValueDeclaration<ELevelRetroTexture>>
      &Values() {
    static const TArray<TRenderingTextValueDeclaration<ELevelRetroTexture>>
        RegisteredValues = {
            {"terrain_ortho", ELevelRetroTexture::TerrainOrtho},
            {"building_timber", ELevelRetroTexture::BuildingTimber},
            {"road_dust", ELevelRetroTexture::RoadDust},
            {"water_creek", ELevelRetroTexture::WaterCreek},
            {"foliage_riparian", ELevelRetroTexture::FoliageRiparian},
            {"rock_granite", ELevelRetroTexture::RockGranite},
            {"mine_timber", ELevelRetroTexture::MineTimber},
            {"marker_paint", ELevelRetroTexture::MarkerPaint},
            {"npc_body", ELevelRetroTexture::NpcBody},
            {"npc_hat", ELevelRetroTexture::NpcHat},
            {"horse_coat", ELevelRetroTexture::HorseCoat},
            {"horse_leg", ELevelRetroTexture::HorseLeg},
            {"horse_tack", ELevelRetroTexture::HorseTack}};
    return RegisteredValues;
  }
};

template <typename Output>
func::Maybe<Output> ReadRenderingTextValue(const FString &Text) {
  const FString LowerText = Text.ToLower();
  const TArray<TRenderingTextValueDeclaration<Output>> &Values =
      TRenderingTextValueRegistry<Output>::Values();
  return func::fold_array<TRenderingTextValueDeclaration<Output>,
                          func::Maybe<Output>>(
      Values, func::nothing<Output>(),
      [LowerText](const func::Maybe<Output> &Current,
                  const TRenderingTextValueDeclaration<Output> &Declaration) {
        return Current.hasValue || Declaration.Text != LowerText
                   ? Current
                   : func::just(Declaration.Value);
      });
}

template <typename Output> Output RequireRenderingTextValue(const FString &Text) {
  const func::Maybe<Output> Resolved = ReadRenderingTextValue<Output>(Text);
  check(Resolved.hasValue);
  return Resolved.value;
}

template <typename Item, typename Predicate>
Item RequireRenderingCatalogItem(const TArray<Item> &Catalog,
                                 Predicate Matches) {
  const func::Maybe<Item> Found = func::find_array<Item>(Catalog, Matches);
  check(Found.hasValue);
  return Found.value;
}

FLevelRetroTextureSpec ReduceTextureCatalogItem(
    const ForbocAI::Game::Data::FRenderingTextureSpecSettings &Settings) {
  return {ReduceTextureKind(Settings.Texture), Settings.Name, Settings.Use,
          FIntPoint(Settings.Size, Settings.Size)};
}

} // namespace

FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input) {
  return (func::pipe(Input) |
          [](FLevelRetroTextureApply Value) -> FLevelRetroTextureApply {
    return Value;
  })
      .val;
}

ELevelRetroTexture ReduceTextureKind(const FString &Texture) {
  return RequireRenderingTextValue<ELevelRetroTexture>(Texture);
}

TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(
    const TArray<ForbocAI::Game::Data::FRenderingTextureSpecSettings>
        &Settings) {
  return func::map_array<
      ForbocAI::Game::Data::FRenderingTextureSpecSettings,
      FLevelRetroTextureSpec>(Settings, ReduceTextureCatalogItem);
}

FLevelRetroTextureSpec
ReduceTextureSpec(const FRenderingTextureSpecRequest &Request) {
  check(!Request.Catalog.IsEmpty());
  return RequireRenderingCatalogItem<FLevelRetroTextureSpec>(
      Request.Catalog, [&Request](const FLevelRetroTextureSpec &Spec) {
        return Spec.Texture == Request.Texture;
      });
}

} // namespace RenderingTextureReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
