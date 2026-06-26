#include "Features/Components/RetroStyle.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace RetroStyle {
namespace {
constexpr int32 BlockTextureSize = 16;
constexpr int32 CharacterTextureSize = 16;

struct FRetroTextureSpecSource {
  EMapRetroTexture Texture;
  const TCHAR *Name;
  const TCHAR *Use;
  int32 Size;
};

FMapRetroTextureSpec Spec(const FRetroTextureSpecSource &Source) {
  return {Source.Texture, FString(Source.Name), FString(Source.Use),
          FIntPoint(Source.Size, Source.Size)};
}
} // namespace

const FMapRetroRenderProfile &RuntimeProfile() {
  static const FMapRetroRenderProfile Profile = {
      15.0f, // 3pm
      0,     // no AA
      0,     // no post-process AA
      55.0f, 1.65f, 0.85f, 0.85f, -56.0f, -64.0f,
      8.5f,  0.08f, 1,     512};
  return Profile;
}

TArray<FMapRetroTextureSpec> TextureCatalog() {
  return {
      Spec({EMapRetroTexture::TerrainOrtho,
            TEXT("terrain-ortho-dither"),
            TEXT("dithered terrain color from the French Gulch ortho sample"),
            BlockTextureSize}),
      Spec({EMapRetroTexture::BuildingTimber,
            TEXT("building-timber"),
            TEXT("false-front buildings, cabins, porch boards, and hotel/store silhouettes"),
            BlockTextureSize}),
      Spec({EMapRetroTexture::RoadDust, TEXT("road-dust"),
            TEXT("Main Street, French Gulch Road, wagon yard, and mine road"),
            BlockTextureSize}),
      Spec({EMapRetroTexture::WaterCreek, TEXT("clear-creek-water"),
            TEXT("Clear Creek ribbon, shallow bends, and future Water System guide surfaces"),
            BlockTextureSize}),
      Spec({EMapRetroTexture::FoliageRiparian,
            TEXT("riparian-foliage"),
            TEXT("cottonwoods, willow shrubs, chaparral, and PCG foliage markers"),
            BlockTextureSize}),
      Spec({EMapRetroTexture::RockGranite,
            TEXT("creek-granite-rock"),
            TEXT("boulders, creek stone shelves, cemetery terrace, and hillside rock scatter"),
            BlockTextureSize}),
      Spec({EMapRetroTexture::MineTimber,
            TEXT("mine-dark-timber"),
            TEXT("mine entrance, timber supports, and waste-rock edge pieces"),
            BlockTextureSize}),
      Spec({EMapRetroTexture::MarkerPaint,
            TEXT("prototype-marker-paint"),
            TEXT("PCG/Water staging markers and remaining non-final blockout markers"),
            BlockTextureSize}),
      Spec({EMapRetroTexture::NpcBody, TEXT("npc-work-cloth"),
            TEXT("primitive townspeople clothing"), CharacterTextureSize}),
      Spec({EMapRetroTexture::NpcHat, TEXT("npc-worn-hat"),
            TEXT("primitive townspeople hats"), CharacterTextureSize}),
      Spec({EMapRetroTexture::HorseCoat, TEXT("horse-coat"),
            TEXT("walking horse body, neck, and head primitives"),
            CharacterTextureSize}),
      Spec({EMapRetroTexture::HorseLeg, TEXT("horse-leg-dark"),
            TEXT("walking horse legs and tail primitives"),
            CharacterTextureSize}),
      Spec({EMapRetroTexture::HorseTack,
            TEXT("horse-tack-saddle"),
            TEXT("walking horse saddle and tack primitives"),
            CharacterTextureSize}),
  };
}

const FMapRetroTextureSpec &
TextureSpec(EMapRetroTexture Texture) {
  static const TArray<FMapRetroTextureSpec> Catalog = TextureCatalog();
  for (const FMapRetroTextureSpec &Spec : Catalog) {
    if (Spec.Texture == Texture) {
      return Spec;
    }
  }
  return Catalog[0];
}

} // namespace RetroStyle
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
