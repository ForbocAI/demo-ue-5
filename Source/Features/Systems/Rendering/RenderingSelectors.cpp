#include "Features/Systems/Rendering/RenderingSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingSelectors {
namespace {

constexpr int32 BlockTextureSize = 16;
constexpr int32 CharacterTextureSize = 16;

struct FRetroTextureSpecSource {
  ELevelRetroTexture Texture;
  const TCHAR *Name;
  const TCHAR *Use;
  int32 Size;
};

FLevelRetroTextureSpec Spec(const FRetroTextureSpecSource &Source) {
  return {Source.Texture, FString(Source.Name), FString(Source.Use),
          FIntPoint(Source.Size, Source.Size)};
}

const FLevelRetroTextureSpec &SelectTextureSpecRecursive(
    const TArray<FLevelRetroTextureSpec> &Catalog, ELevelRetroTexture Texture,
    int32 Index) {
  return Index >= Catalog.Num()
             ? Catalog[0]
             : (Catalog[Index].Texture == Texture
                    ? Catalog[Index]
                    : SelectTextureSpecRecursive(Catalog, Texture, Index + 1));
}

} // namespace

const FLevelRetroRenderProfile &SelectRuntimeProfile() {
  static const func::Lazy<FLevelRetroRenderProfile> Profile =
      func::lazy([]() -> FLevelRetroRenderProfile {
        return {15.0f, 0,     0,     55.0f, 1.65f, 0.85f, 0.85f,
                -56.0f, -64.0f, 8.5f, 0.08f, 1,     512};
      });
  return func::eval(Profile);
}

TArray<FLevelRetroTextureSpec> SelectTextureCatalog() {
  return {
      Spec({ELevelRetroTexture::TerrainOrtho,
            TEXT("terrain-ortho-dither"),
            TEXT("dithered terrain color from the French Gulch ortho sample"),
            BlockTextureSize}),
      Spec({ELevelRetroTexture::BuildingTimber,
            TEXT("building-timber"),
            TEXT("false-front buildings, cabins, porch boards, and hotel/store silhouettes"),
            BlockTextureSize}),
      Spec({ELevelRetroTexture::RoadDust, TEXT("road-dust"),
            TEXT("Main Street, French Gulch Road, wagon yard, and mine road"),
            BlockTextureSize}),
      Spec({ELevelRetroTexture::WaterCreek, TEXT("clear-creek-water"),
            TEXT("Clear Creek ribbon, shallow bends, and future Water System guide surfaces"),
            BlockTextureSize}),
      Spec({ELevelRetroTexture::FoliageRiparian,
            TEXT("riparian-foliage"),
            TEXT("cottonwoods, willow shrubs, chaparral, and PCG foliage markers"),
            BlockTextureSize}),
      Spec({ELevelRetroTexture::RockGranite,
            TEXT("creek-granite-rock"),
            TEXT("boulders, creek stone shelves, cemetery terrace, and hillside rock scatter"),
            BlockTextureSize}),
      Spec({ELevelRetroTexture::MineTimber,
            TEXT("mine-dark-timber"),
            TEXT("mine entrance, timber supports, and waste-rock edge pieces"),
            BlockTextureSize}),
      Spec({ELevelRetroTexture::MarkerPaint,
            TEXT("prototype-marker-paint"),
            TEXT("PCG/Water staging markers and remaining non-final blockout markers"),
            BlockTextureSize}),
      Spec({ELevelRetroTexture::NpcBody, TEXT("npc-work-cloth"),
            TEXT("primitive townspeople clothing"), CharacterTextureSize}),
      Spec({ELevelRetroTexture::NpcHat, TEXT("npc-worn-hat"),
            TEXT("primitive townspeople hats"), CharacterTextureSize}),
      Spec({ELevelRetroTexture::HorseCoat, TEXT("horse-coat"),
            TEXT("walking horse body, neck, and head primitives"),
            CharacterTextureSize}),
      Spec({ELevelRetroTexture::HorseLeg, TEXT("horse-leg-dark"),
            TEXT("walking horse legs and tail primitives"),
            CharacterTextureSize}),
      Spec({ELevelRetroTexture::HorseTack,
            TEXT("horse-tack-saddle"),
            TEXT("walking horse saddle and tack primitives"),
            CharacterTextureSize}),
  };
}

const FLevelRetroTextureSpec &SelectTextureSpec(ELevelRetroTexture Texture) {
  static const func::Lazy<TArray<FLevelRetroTextureSpec>> Catalog =
      func::lazy([]() -> TArray<FLevelRetroTextureSpec> {
        return SelectTextureCatalog();
      });
  return SelectTextureSpecRecursive(func::eval(Catalog), Texture, 0);
}

} // namespace RenderingSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
