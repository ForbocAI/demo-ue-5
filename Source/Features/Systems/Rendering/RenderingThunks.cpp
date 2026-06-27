#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Texture2D.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Rendering/RenderingSelectors.h"
#include "HAL/IConsoleManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingThunks {
namespace {
constexpr int32 TextureChannels = 4;

struct FRetroCVarFloat {
  const TCHAR *Name;
  float Value;
};

struct FRetroCVarInt {
  const TCHAR *Name;
  int32 Value;
};

struct FRetroColorMix {
  FColor A;
  FColor B;
  int32 Numerator;
  int32 Denominator;
};

struct FRetroTextureCell {
  ELevelRetroTexture Texture;
  int32 X;
  int32 Y;
};

struct FRetroRgb {
  uint8 R;
  uint8 G;
  uint8 B;
};

struct FRetroHashCell {
  int32 X;
  int32 Y;
  int32 Salt;
};

void SetCVarFloat(const FRetroCVarFloat &CVarValue) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(CVarValue.Name);
  const func::Maybe<IConsoleVariable *> CVar =
      func::from_nullable_value(Found, Found != nullptr);
  CVar.hasValue && (CVar.value->Set(CVarValue.Value, ECVF_SetByGameSetting),
                    true);
}

void SetCVarInt(const FRetroCVarInt &CVarValue) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(CVarValue.Name);
  const func::Maybe<IConsoleVariable *> CVar =
      func::from_nullable_value(Found, Found != nullptr);
  CVar.hasValue && (CVar.value->Set(CVarValue.Value, ECVF_SetByGameSetting),
                    true);
}

FColor Color(FRetroRgb Rgb) { return FColor(Rgb.R, Rgb.G, Rgb.B, 255); }

FColor Mix(const FRetroColorMix &MixValue) {
  const int32 Inverse = MixValue.Denominator - MixValue.Numerator;
  return FColor(
      static_cast<uint8>((MixValue.A.R * Inverse +
                          MixValue.B.R * MixValue.Numerator) /
                         MixValue.Denominator),
      static_cast<uint8>((MixValue.A.G * Inverse +
                          MixValue.B.G * MixValue.Numerator) /
                         MixValue.Denominator),
      static_cast<uint8>((MixValue.A.B * Inverse +
                          MixValue.B.B * MixValue.Numerator) /
                         MixValue.Denominator),
      255);
}

int32 HashCell(FRetroHashCell Cell) {
  int32 Value = Cell.X * 73856093 ^ Cell.Y * 19349663 ^ Cell.Salt * 83492791;
  Value ^= Value >> 13;
  return FMath::Abs(Value);
}

FColor PaletteColor(const FRetroTextureCell &Cell) {
  const int32 Noise =
      HashCell({Cell.X, Cell.Y, static_cast<int32>(Cell.Texture)});
  switch (Cell.Texture) {
  case ELevelRetroTexture::TerrainOrtho:
    return Mix({Color({111, 103, 78}), Color({72, 96, 55}), Noise % 5, 8});
  case ELevelRetroTexture::BuildingTimber:
    if (Cell.X % 4 == 0 || Cell.Y % 7 == 0) {
      return Color({74, 47, 27});
    }
    return Mix({Color({128, 82, 43}), Color({171, 119, 66}), Noise % 4, 7});
  case ELevelRetroTexture::RoadDust:
    if ((Cell.X + Cell.Y + Noise) % 11 == 0) {
      return Color({86, 75, 59});
    }
    return Mix({Color({128, 104, 73}), Color({176, 145, 96}), Noise % 5, 9});
  case ELevelRetroTexture::WaterCreek:
    if ((Cell.X * 2 + Cell.Y) % 13 == 0) {
      return Color({152, 189, 177});
    }
    return (Cell.Y / 2) % 2 == 0 ? Color({38, 92, 118})
                                  : Color({28, 64, 94});
  case ELevelRetroTexture::FoliageRiparian:
    if (Noise % 9 < 2) {
      return Color({36, 77, 39});
    }
    return Mix({Color({74, 112, 48}), Color({122, 143, 66}), Noise % 4, 6});
  case ELevelRetroTexture::RockGranite:
    if ((Cell.X + Cell.Y + Noise) % 7 == 0) {
      return Color({74, 71, 66});
    }
    return Mix({Color({126, 121, 107}), Color({171, 160, 138}), Noise % 5, 8});
  case ELevelRetroTexture::MineTimber:
    if (Cell.X % 3 == 0) {
      return Color({45, 35, 27});
    }
    return Mix({Color({78, 58, 39}), Color({111, 82, 50}), Noise % 4, 7});
  case ELevelRetroTexture::MarkerPaint:
    return ((Cell.X / 2 + Cell.Y / 2) % 2 == 0) ? Color({171, 124, 28})
                                                : Color({40, 33, 24});
  case ELevelRetroTexture::NpcBody:
    return ((Cell.X + Cell.Y) % 5 == 0) ? Color({36, 45, 52})
                                        : Color({58, 73, 80});
  case ELevelRetroTexture::NpcHat:
    return (Cell.X % 5 == 0) ? Color({60, 39, 20}) : Color({93, 63, 31});
  case ELevelRetroTexture::HorseCoat:
    return Mix({Color({94, 47, 22}), Color({139, 75, 33}), Noise % 4, 7});
  case ELevelRetroTexture::HorseLeg:
    return Mix({Color({38, 25, 15}), Color({75, 44, 22}), Noise % 3, 6});
  case ELevelRetroTexture::HorseTack:
    return ((Cell.X + Cell.Y) % 6 == 0) ? Color({45, 28, 16})
                                        : Color({77, 48, 26});
  }
  return Color({255, 0, 255});
}

UTexture2D *CreateRetroTexture(ELevelRetroTexture Texture) {
  const FLevelRetroTextureSpec &Spec =
      RenderingSelectors::SelectTextureSpec(Texture);
  UTexture2D *Result =
      UTexture2D::CreateTransient(Spec.Size.X, Spec.Size.Y, PF_B8G8R8A8);
  if (!Result || !Result->GetPlatformData() ||
      Result->GetPlatformData()->Mips.Num() == 0) {
    return nullptr;
  }

  Result->Filter = TF_Nearest;
  Result->AddressX = TA_Wrap;
  Result->AddressY = TA_Wrap;
  Result->MipGenSettings = TMGS_NoMipmaps;
  Result->NeverStream = true;
  Result->SRGB = true;

  FTexture2DMipMap &Mip = Result->GetPlatformData()->Mips[0];
  void *Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
  if (!Data) {
    Mip.BulkData.Unlock();
    return nullptr;
  }

  TArray<FColor> Pixels;
  Pixels.Reserve(Spec.Size.X * Spec.Size.Y);
  for (int32 Y = 0; Y < Spec.Size.Y; ++Y) {
    for (int32 X = 0; X < Spec.Size.X; ++X) {
      Pixels.Add(PaletteColor({Texture, X, Y}));
    }
  }
  FMemory::Memcpy(Data, Pixels.GetData(),
                  Pixels.Num() * TextureChannels * sizeof(uint8));
  Mip.BulkData.Unlock();

  Result->UpdateResource();
  Result->AddToRoot();
  return Result;
}

UTexture2D *TextureFor(ELevelRetroTexture Texture) {
  static TMap<uint8, UTexture2D *> TextureCache;
  const uint8 Key = static_cast<uint8>(Texture);
  if (UTexture2D **Cached = TextureCache.Find(Key)) {
    return *Cached;
  }

  UTexture2D *Created = CreateRetroTexture(Texture);
  TextureCache.Add(Key, Created);
  return Created;
}
} // namespace

rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id) {
  return [Id](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
              std::function<FRuntimeState()> GetState)
             -> func::AsyncResult<FRenderingPayload> {
    const FRenderingPayload Payload{
        Id, RenderingSelectors::SelectRuntimeProfile(),
        RenderingSelectors::SelectTextureCatalog()};
    return func::createAsyncResult<FRenderingPayload>(
        [Dispatch, Payload](std::function<void(FRenderingPayload)> Resolve,
                            std::function<void(std::string)> Reject) {
          Dispatch(RenderingActions::RenderingProfileObserved()(Payload));
          Resolve(Payload);
        });
  };
}

void ApplyRuntimeProfile() {
  const FLevelRetroRenderProfile &Profile =
      RenderingSelectors::SelectRuntimeProfile();
  SetCVarInt({TEXT("r.AntiAliasingMethod"), Profile.AntiAliasingMethod});
  SetCVarInt({TEXT("r.PostProcessAAQuality"), Profile.PostProcessAAQuality});
  SetCVarInt({TEXT("r.TemporalAA.Upsampling"), 0});
  SetCVarFloat({TEXT("r.ScreenPercentage"), Profile.ScreenPercentage});
  SetCVarFloat({TEXT("r.ViewDistanceScale"), Profile.ViewDistanceScale});
  SetCVarFloat({TEXT("foliage.DensityScale"), Profile.FoliageDensityScale});
  SetCVarFloat({TEXT("grass.DensityScale"), Profile.GrassDensityScale});
  SetCVarInt({TEXT("r.MotionBlurQuality"), 0});
  SetCVarInt({TEXT("r.BloomQuality"), 0});
  SetCVarInt({TEXT("r.DepthOfFieldQuality"), 0});
  SetCVarInt({TEXT("r.AmbientOcclusionLevels"), 0});
  SetCVarInt({TEXT("r.SSR.Quality"), 0});
  SetCVarInt({TEXT("r.Shadow.CSM.MaxCascades"), Profile.ShadowCascades});
  SetCVarInt({TEXT("r.Shadow.MaxResolution"), Profile.ShadowMaxResolution});
  SetCVarFloat({TEXT("r.Shadow.RadiusThreshold"), 0.02f});
  SetCVarFloat({TEXT("r.MipMapLODBias"), 2.0f});
  SetCVarFloat({TEXT("r.Streaming.MipBias"), 2.0f});
}

UMaterialInterface *LoadBlockoutMaterial() {
  return LoadObject<UMaterialInterface>(
      nullptr,
      TEXT("/Engine/EngineMaterials/EmissiveTexturedMaterial.EmissiveTexturedMaterial"));
}

void ApplyTexture(const FLevelRetroTextureApply &Request) {
  if (!Request.Part || !Request.BaseMaterial) {
    return;
  }

  UTexture2D *RetroTexture = TextureFor(Request.Texture);
  if (!RetroTexture) {
    return;
  }

  UMaterialInstanceDynamic *Material =
      UMaterialInstanceDynamic::Create(Request.BaseMaterial, Request.Part);
  if (!Material) {
    return;
  }

  Material->SetTextureParameterValue(TEXT("Texture"), RetroTexture);
  Request.Part->SetMaterial(0, Material);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
