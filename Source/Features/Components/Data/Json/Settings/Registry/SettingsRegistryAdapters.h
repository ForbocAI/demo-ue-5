#pragma once

#include "Features/Components/Data/Json/Read/ReadAdapters.h"
#include "Features/Components/Data/Json/Settings/Field/SettingsFieldAdapters.h"

#include <initializer_list>

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

/** User Story: As a json settings registry consumer, I need to invoke read settings fields through a stable signature so the json settings registry workflow remains explicit and composable. @fn template <typename Settings> Settings ReadSettingsFields(const TSharedPtr<FJsonObject> &Object, const TArray<TField<Settings>> &Fields) */
template <typename Settings>
Settings
ReadSettingsFields(const TSharedPtr<FJsonObject> &Object,
                   const TArray<TField<Settings>> &Fields) {
  return func::fold_indexed(
      Fields, static_cast<size_t>(Fields.Num()), Settings{},
      [Object](const Settings &Current,
               const TField<Settings> &Binding) {
        return Binding.Apply(Field(Object, *Binding.FieldName), Current);
      });
}

/** User Story: As a json settings registry consumer, I need to invoke read settings fields through a stable signature so the json settings registry workflow remains explicit and composable. @fn template <typename Settings> Settings ReadSettingsFields(const TSharedPtr<FJsonObject> &Object, std::initializer_list<TField<Settings>> Fields) */
template <typename Settings>
Settings
ReadSettingsFields(const TSharedPtr<FJsonObject> &Object,
                   std::initializer_list<TField<Settings>> Fields) {
  return ReadSettingsFields<Settings>(
      Object, TArray<TField<Settings>>(Fields));
}

// --- Settings registry (RTK entity adapter pattern) ----------------------

#define JSON_SETTING_ATOM(Field) #Field
#define JSON_SETTING_ATOM_LIST_INDIRECT() JSON_SETTING_ATOM_LIST
#define JSON_SETTING_ATOM_LIST(Field, ...)                                   \
  JSON_SETTING_ATOM(Field)                                                   \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_SETTING_ATOM_LIST_INDIRECT)()(             \
                    __VA_ARGS__))
#define JSON_SETTINGS_ATOMS(...)                                             \
  {JSON_EXPAND(JSON_SETTING_ATOM_LIST(__VA_ARGS__))}

template <typename Settings> struct TJsonSettingsRegistry;

#define JSON_SETTINGS_REGISTRY(Type, ...)                                    \
  template <> struct TJsonSettingsRegistry<Type> {                           \
    static const TArray<TField<Type>> &Fields() {                \
      static const TArray<TField<Type>> RegisteredFields =       \
          JSON_SETTINGS_FIELDS(Type, __VA_ARGS__);                           \
      return RegisteredFields;                                               \
    }                                                                        \
  }

/** User Story: As a json settings registry consumer, I need to invoke find registered settings field through a stable signature so the json settings registry workflow remains explicit and composable. @fn template <typename Settings> func::Maybe<TField<Settings>> FindRegisteredSettingsField(const char *FieldAtom) */
template <typename Settings>
func::Maybe<TField<Settings>>
FindRegisteredSettingsField(const char *FieldAtom) {
  const FString FieldName = SettingsFieldName(FieldAtom);
  const TArray<TField<Settings>> &Fields =
      TJsonSettingsRegistry<Settings>::Fields();
  return func::fold_indexed(
      Fields, static_cast<size_t>(Fields.Num()),
      func::nothing<TField<Settings>>(),
      [FieldName](const func::Maybe<TField<Settings>> &Current,
                  const TField<Settings> &Binding) {
        return Current.hasValue || Binding.FieldName != FieldName
                   ? Current
                   : func::just(Binding);
      });
}

/** User Story: As a json settings registry consumer, I need to invoke read settings fields through a stable signature so the json settings registry workflow remains explicit and composable. @fn template <typename Settings> Settings ReadSettingsFields(const TSharedPtr<FJsonObject> &Object, const TArray<const char *> &FieldAtoms) */
template <typename Settings>
Settings ReadSettingsFields(const TSharedPtr<FJsonObject> &Object,
                            const TArray<const char *> &FieldAtoms) {
  return func::fold_indexed(
      FieldAtoms, static_cast<size_t>(FieldAtoms.Num()), Settings{},
      [Object](const Settings &Current, const char *FieldAtom) {
        const func::Maybe<TField<Settings>> Binding =
            FindRegisteredSettingsField<Settings>(FieldAtom);
        check(Binding.hasValue);
        return Binding.value.Apply(Field(Object, *Binding.value.FieldName),
                                   Current);
      });
}

/** User Story: As a json settings registry consumer, I need to invoke read settings fields through a stable signature so the json settings registry workflow remains explicit and composable. @fn template <typename Settings> Settings ReadSettingsFields(const TSharedPtr<FJsonObject> &Object, std::initializer_list<const char *> FieldAtoms) */
template <typename Settings>
Settings ReadSettingsFields(const TSharedPtr<FJsonObject> &Object,
                            std::initializer_list<const char *> FieldAtoms) {
  return ReadSettingsFields<Settings>(Object, TArray<const char *>(FieldAtoms));
}

/** User Story: As a json settings registry consumer, I need to invoke read settings with through a stable signature so the json settings registry workflow remains explicit and composable. @fn template <typename Settings> TJsonObjectSettingsReader<Settings> ReadSettingsWith(std::initializer_list<const char *> FieldAtoms) */
template <typename Settings>
TJsonObjectSettingsReader<Settings>
ReadSettingsWith(std::initializer_list<const char *> FieldAtoms) {
  const TArray<const char *> Atoms(FieldAtoms);
  return [Atoms](const TSharedPtr<FJsonObject> &Object) {
    return ReadSettingsFields<Settings>(Object, Atoms);
  };
}

/** User Story: As a json settings registry consumer, I need to invoke map settings json values through a stable signature so the json settings registry workflow remains explicit and composable. @fn template <typename Settings> TArray<Settings> MapSettingsJsonValues(const TArray<TSharedPtr<FJsonValue>> &Values, std::initializer_list<const char *> FieldAtoms) */
template <typename Settings>
TArray<Settings>
MapSettingsJsonValues(const TArray<TSharedPtr<FJsonValue>> &Values,
                      std::initializer_list<const char *> FieldAtoms) {
  return MapJsonValues<Settings>(Values, ReadSettingsWith<Settings>(FieldAtoms));
}

/** User Story: As a json settings registry consumer, I need to invoke read settings object array field through a stable signature so the json settings registry workflow remains explicit and composable. @fn template <typename Settings> std::function<TArray<Settings>(const TSharedPtr<FJsonObject> &)> ReadSettingsObjectArrayField(const char *FieldAtom, std::initializer_list<const char *> FieldAtoms) */
template <typename Settings>
std::function<TArray<Settings>(const TSharedPtr<FJsonObject> &)>
ReadSettingsObjectArrayField(const char *FieldAtom, std::initializer_list<const char *> FieldAtoms) {
  FString AtomStr(FieldAtom);
  TArray<const char *> AtomsArray(FieldAtoms);
  return [AtomStr, AtomsArray](const TSharedPtr<FJsonObject> &Object) {
    return MapSettingsJsonValues<Settings>(
        ReadArray(Field(Object, *SettingsFieldName(*AtomStr))),
        std::initializer_list<const char *>(AtomsArray.GetData(), AtomsArray.GetData() + AtomsArray.Num()));
  };
}

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
