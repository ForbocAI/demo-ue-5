#include "Features/Systems/Rendering/Texture/TextureSlice.h"

#include "Core/ue_fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingTextureReducers {
namespace {

template <typename Item, typename Predicate>
Item RequireRenderingCatalogItem(const TArray<Item> &Catalog,
                                 Predicate Matches) {
  const func::Maybe<Item> Found = func::find_array<Item>(Catalog, Matches);
  check(Found.hasValue);
  return Found.value;
}

ELevelRetroTexture TextureValueForCatalogIndex(int32 Index) {
  check(Index >= int32{});
  return static_cast<ELevelRetroTexture>(Index);
}

FLevelRetroTextureSpec ReduceTextureCatalogItem(
    const TArray<ForbocAI::Game::Data::FSpecSettings> &Settings,
    int32 Index) {
  check(Settings.IsValidIndex(Index));
  const ForbocAI::Game::Data::FSpecSettings &Item = Settings[Index];
  return {TextureValueForCatalogIndex(Index), Item.Name, Item.Use,
          FIntPoint(Item.Size, Item.Size)};
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

TArray<FLevelRetroTextureSpec> ReduceTextureCatalog(
    const TArray<ForbocAI::Game::Data::FSpecSettings>
        &Settings) {
  const TArray<int32> Indices = func::index_range(Settings.Num());
  return func::map_array<int32, FLevelRetroTextureSpec>(
      Indices, [&Settings](int32 Index) {
        return ReduceTextureCatalogItem(Settings, Index);
      });
}

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
