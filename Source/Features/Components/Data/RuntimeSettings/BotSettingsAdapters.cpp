#include "Features/Components/Data/RuntimeSettings/BotSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace BotSettingsAdapters {
namespace Json = JsonAdapters;
namespace {

template <typename Settings> struct TBotSettingsSchema;

#define BOT_SETTINGS_SCHEMA(Type, ...)                                       \
  template <> struct TBotSettingsSchema<Type> {                              \
    static const TArray<Json::TJsonSettingsField<Type>> &Fields() {           \
      static const TArray<Json::TJsonSettingsField<Type>> DeclaredFields =    \
          JSON_SETTINGS_FIELDS(Type, __VA_ARGS__);                           \
      return DeclaredFields;                                                 \
    }                                                                        \
  }

BOT_SETTINGS_SCHEMA(FTownspersonDefaultsSettings, Id, Name, Role, Persona,
                    InteractionPrompt, DefaultPlayerLine);

BOT_SETTINGS_SCHEMA(FTownspersonPresentationSettings, CharacterHeightFeet,
                    CharacterShoulderWidthFeet, PatrolPauseSeconds,
                    PromptAboveHeadFeet, NameAbovePromptFeet,
                    DialogueAboveNameHeightRatio, InteractionRadiusLots,
                    WalkSpeedHeightRatio, MannequinScale,
                    MannequinOffsetFeet, MannequinRotation, PromptTextScale,
                    DialogueTextScale, MeshPath,
                    AnimationBlueprintClassPath);

BOT_SETTINGS_SCHEMA(FHorsePresentationSettings, DefaultName, HorseLengthFeet,
                    BodyHeightFeet, LegHeightFeet, NeckHeightFeet,
                    HeadHeightFeet, SaddleHeightFeet, PatrolPauseSeconds,
                    WalkSpeedHorseLengthRatio, RouteArrivalLegRatio,
                    ImportedHorseScale, MountedRiderScale,
                    NameTextWorldSizeFeet, HorseMeshPath,
                    HorseWalkAnimationPath, RiderMeshPath,
                    RiderWalkAnimationPath);

#undef BOT_SETTINGS_SCHEMA

template <typename Settings>
Settings ReadBotSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<Settings>(
      Object, TBotSettingsSchema<Settings>::Fields());
}

} // namespace

FTownspersonDefaultsSettings
ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object) {
  return ReadBotSettings<FTownspersonDefaultsSettings>(Object);
}

FTownspersonPresentationSettings
ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return ReadBotSettings<FTownspersonPresentationSettings>(Object);
}

FHorsePresentationSettings
ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return ReadBotSettings<FHorsePresentationSettings>(Object);
}

} // namespace BotSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
