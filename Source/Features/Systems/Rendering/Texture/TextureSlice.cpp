#include "Features/Systems/Rendering/Texture/TextureSlice.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

#include "Core/fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingTextureReducers {
namespace {

/** User Story: As a systems rendering texture consumer, I need to invoke require rendering catalog item through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn template <typename Item, typename Predicate> Item RequireRenderingCatalogItem(const TArray<Item> &Catalog, Predicate Matches) */
template <typename Item, typename Predicate>
Item RequireRenderingCatalogItem(const TArray<Item> &Catalog,
                                 Predicate Matches) {
  const func::Maybe<Item> Found = func::find_array<Item>(Catalog, Matches);
  check(Found.hasValue);
  return Found.value;
}

/** User Story: As a systems rendering texture consumer, I need to invoke texture value for catalog index through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn ELevelRetroTexture TextureValueForCatalogIndex(int32 Index) */
ELevelRetroTexture TextureValueForCatalogIndex(int32 Index) {
  check(Index >= int32{});
  return static_cast<ELevelRetroTexture>(Index);
}

/** User Story: As a systems rendering texture consumer, I need to invoke reduce texture catalog item through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn FLevelRetroTextureSpec ReduceTextureCatalogItem( const TArray<ForbocAI::Game::Data::FSpecSettings> &Settings, int32 Index) */
FLevelRetroTextureSpec ReduceTextureCatalogItem(
    const TArray<ForbocAI::Game::Data::FSpecSettings> &Settings,
    int32 Index) {
  check(Settings.IsValidIndex(Index));
  const ForbocAI::Game::Data::FSpecSettings &Item = Settings[Index];
  return {TextureValueForCatalogIndex(Index), Item.Name, Item.Use,
          FIntPoint(Item.Size, Item.Size)};
}

} // namespace

/** User Story: As a systems rendering texture consumer, I need to invoke normalize texture apply through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn FLevelRetroTextureApply NormalizeTextureApply(const FLevelRetroTextureApply &Input) */
FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input) {
  return (func::pipe(Input) |
          [](FLevelRetroTextureApply Value) -> FLevelRetroTextureApply {
    return Value;
  })
      .val;
}

/** User Story: As a systems rendering texture consumer, I need to invoke reduce texture catalog through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn TArray<FLevelRetroTextureSpec> ReduceTextureCatalog( const TArray<ForbocAI::Game::Data::FSpecSettings> &Settings) */
TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(
    const TArray<ForbocAI::Game::Data::FSpecSettings>
        &Settings) {
  const TArray<int32> Indices = func::index_range(Settings.Num());
  return func::map_array<int32, FLevelRetroTextureSpec>(
      Indices, [&Settings](int32 Index) {
        return ReduceTextureCatalogItem(Settings, Index);
      });
}

/** User Story: As a systems rendering texture consumer, I need to invoke reduce texture spec through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn FLevelRetroTextureSpec ReduceTextureSpec(const FSpecRequest &Request) */
FLevelRetroTextureSpec
ReduceTextureSpec(const FSpecRequest &Request) {
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
