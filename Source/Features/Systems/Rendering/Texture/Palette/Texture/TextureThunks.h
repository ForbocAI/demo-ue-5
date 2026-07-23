#pragma once

#include "Features/Systems/Rendering/Texture/Palette/Rules/RulesThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

/** User Story: As a palette texture consumer, I need to resolve the configured palette through a stable signature so texture generation remains data-driven. @fn ForbocAI::Game::Data::FPaletteSettings RequiredPalette(const FRetroTextureCell &Cell) */
ForbocAI::Game::Data::FPaletteSettings
RequiredPalette(const FRetroTextureCell &Cell) {
  check(Cell.Settings);
  const TArray<int32> Indices =
      func::index_range(Cell.Settings->TexturePalettes.Num());
  const func::Maybe<int32> PaletteIndex = func::find_indexed(
      Indices, static_cast<size_t>(Indices.Num()), [&Cell](int32 Index) {
        return static_cast<ELevelRetroTexture>(Index) == Cell.Texture;
      });
  check(PaletteIndex.hasValue);
  return Cell.Settings->TexturePalettes[PaletteIndex.value];
}

/** User Story: As a palette texture consumer, I need to resolve a cell color through a stable signature so texture generation remains deterministic. @fn FColor PaletteColor(const FRetroTextureCell &Cell) */
FColor PaletteColor(const FRetroTextureCell &Cell) {
  check(Cell.Settings);
  const int32 Noise =
      HashCell({Cell.X, Cell.Y, static_cast<int32>(Cell.Texture),
                &Cell.Settings->TextureHash});
  const ForbocAI::Game::Data::FPaletteSettings Palette =
      RequiredPalette(Cell);
  const func::Maybe<ForbocAI::Game::Data::FRuleSettings> Rule =
      func::find_indexed(
          Palette.Rules, static_cast<size_t>(Palette.Rules.Num()),
          [&Cell, Noise](
              const ForbocAI::Game::Data::FRuleSettings &Candidate) {
            return PredicateMatches(
                {Candidate.Predicate, Cell.X, Cell.Y, Noise, Cell.Settings});
          });
  check(Rule.hasValue);
  return ResolveColor({Rule.value.Result, Noise,
                       Cell.Settings->Buffer.Alpha, Cell.Settings});
}

/**
 * User Story: As a palette texture consumer, I need to derive a cache key through a stable signature so generated textures are reused consistently.
 * @fn FString TextureCacheKey( const FLevelRetroTextureSpec &Spec, const ForbocAI::Game::Data::FRenderingSettings &Settings)
 */
FString TextureCacheKey(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  return frmt::RuntimeString(
      Settings.Format.CacheKeyFormat,
      frmt::Args({frmt::Arg(static_cast<int32>(Spec.Texture)),
                  frmt::Arg(Spec.Size.X), frmt::Arg(Spec.Size.Y)}));
}

/**
 * User Story: As a palette texture consumer, I need to create a retro texture through a stable signature so rendering assets are generated from authored rules.
 * @fn UTexture2D *CreateRetroTexture( const FLevelRetroTextureSpec &Spec, const ForbocAI::Game::Data::FRenderingSettings &Settings)
 */
UTexture2D *CreateRetroTexture(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  check(Spec.Size.X > int32{});
  check(Spec.Size.Y > int32{});

  UTexture2D *Result =
      UTexture2D::CreateTransient(Spec.Size.X, Spec.Size.Y, PF_B8G8R8A8);
  check(Result);
  check(Result->GetPlatformData());
  check(Settings.Buffer.MipIndex >= int32{});
  check(Result->GetPlatformData()->Mips.Num() > Settings.Buffer.MipIndex);

  Result->Filter = TF_Nearest;
  Result->AddressX = TA_Wrap;
  Result->AddressY = TA_Wrap;
  Result->MipGenSettings = TMGS_NoMipmaps;
  Result->NeverStream = true;
  Result->SRGB = true;

  FTexture2DMipMap &Mip =
      Result->GetPlatformData()->Mips[Settings.Buffer.MipIndex];
  void *Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
  check(Data);

  const std::vector<FColor> Pixels = func::map_grid<FColor>(
      static_cast<size_t>(Spec.Size.Y), static_cast<size_t>(Spec.Size.X),
      [&Spec, &Settings](const func::GridIndex &Index) {
        return PaletteColor({Spec.Texture, static_cast<int32>(Index.Column),
                             static_cast<int32>(Index.Row),
                             &Settings});
      });
  FMemory::Memcpy(Data, Pixels.data(),
                  Pixels.size() * Settings.Buffer.Channels * sizeof(uint8));
  Mip.BulkData.Unlock();

  Result->UpdateResource();
  Result->AddToRoot();
  return Result;
}

/**
 * User Story: As a palette texture consumer, I need to retrieve or create a texture through a stable signature so cache ownership remains encapsulated.
 * @fn UTexture2D *TextureFor( const FLevelRetroTextureSpec &Spec, const ForbocAI::Game::Data::FRenderingSettings &Settings)
 */
UTexture2D *TextureFor(
    const FLevelRetroTextureSpec &Spec,
    const ForbocAI::Game::Data::FRenderingSettings &Settings) {
  static TMap<FString, UTexture2D *> TextureCache;
  const FString Key = TextureCacheKey(Spec, Settings);
  UTexture2D **Cached = TextureCache.Find(Key);
  return Cached ? *Cached
                : [&]() {
                    UTexture2D *Created =
                        CreateRetroTexture(Spec, Settings);
                    TextureCache.Add(Key, Created);
                    return Created;
                  }();
}

} // namespace
} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
