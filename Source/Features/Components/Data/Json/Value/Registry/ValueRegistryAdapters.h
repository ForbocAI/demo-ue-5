#pragma once

#include "Features/Components/Data/Json/Value/Required/RequiredAdapters.h"

namespace ForbocAI::Game::Data::JsonValueAdapters {

#define JSON_REQUIRED_FIELD(Type, Field) {#Field, &Type::Field}
#define JSON_REQUIRED_FIELD_READER(Type, Reader, Field)                      \
  {#Field, &Type::Field, Reader}
#define JSON_REQUIRED_FIELD_LIST_INDIRECT() JSON_REQUIRED_FIELD_LIST
#define JSON_REQUIRED_FIELD_LIST(Type, Field, ...)                           \
  JSON_REQUIRED_FIELD(Type, Field)                                           \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_REQUIRED_FIELD_LIST_INDIRECT)()(           \
                    Type, __VA_ARGS__))
#define JSON_REQUIRED_FIELDS(Type, ...)                                      \
  JSON_EXPAND(JSON_REQUIRED_FIELD_LIST(Type, __VA_ARGS__))
#define JSON_REQUIRED_FIELD_READER_LIST_INDIRECT()                           \
  JSON_REQUIRED_FIELD_READER_LIST
#define JSON_REQUIRED_FIELD_READER_LIST(Type, Reader, Field, ...)            \
  JSON_REQUIRED_FIELD_READER(Type, Reader, Field)                            \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_REQUIRED_FIELD_READER_LIST_INDIRECT)()(    \
                    Type, Reader, __VA_ARGS__))
#define JSON_REQUIRED_FIELD_READERS(Type, Reader, ...)                       \
  JSON_EXPAND(JSON_REQUIRED_FIELD_READER_LIST(Type, Reader, __VA_ARGS__))

#define JSON_REQUIRED_ATOM(Field) #Field
#define JSON_REQUIRED_ATOM_LIST_INDIRECT() JSON_REQUIRED_ATOM_LIST
#define JSON_REQUIRED_ATOM_LIST(Field, ...)                                  \
  JSON_REQUIRED_ATOM(Field)                                                  \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_REQUIRED_ATOM_LIST_INDIRECT)()(            \
                    __VA_ARGS__))
#define JSON_REQUIRED_ATOMS(...)                                             \
  {JSON_EXPAND(JSON_REQUIRED_ATOM_LIST(__VA_ARGS__))}

template <typename State> struct TRequiredJsonFieldRegistry;

#define JSON_REQUIRED_FIELD_REGISTRY(Type, ...)                              \
  template <> struct TRequiredJsonFieldRegistry<Type> {                      \
    static const TArray<TJsonFieldDeclaration<Type>> &Fields() {      \
      static const TArray<TJsonFieldDeclaration<Type>>                \
          RegisteredFields = {JSON_REQUIRED_FIELDS(Type, __VA_ARGS__)};       \
      return RegisteredFields;                                               \
    }                                                                        \
  }

/** User Story: As a required-field registry consumer, I need to find field declarations through a stable signature so authored JSON fields resolve compositionally. @fn template <typename State> func::Maybe<TJsonFieldDeclaration<State>> FindRequiredJsonField(const char *FieldAtom) */
template <typename State>
func::Maybe<TJsonFieldDeclaration<State>>
FindRequiredJsonField(const char *FieldAtom) {
  const FString FieldName = RequiredFieldName(FieldAtom);
  const TArray<TJsonFieldDeclaration<State>> &Fields =
      TRequiredJsonFieldRegistry<State>::Fields();
  return func::fold_array<
      TJsonFieldDeclaration<State>,
      func::Maybe<TJsonFieldDeclaration<State>>>(
      Fields, func::nothing<TJsonFieldDeclaration<State>>(),
      [FieldName](const func::Maybe<TJsonFieldDeclaration<State>>
                      &Current,
                  const TJsonFieldDeclaration<State> &Field) {
        return Current.hasValue || Field.FieldName != FieldName
                   ? Current
                   : func::just(Field);
      });
}

/** User Story: As a required-field registry consumer, I need to read an authored field set through a stable signature so state construction remains data-driven. @fn template <typename State> func::Maybe<State> ReadRequiredFields( const FJsonReadStateRequest<State> &Request, const TArray<const char *> &FieldAtoms) */
template <typename State>
func::Maybe<State> ReadRequiredFields(
    const FJsonReadStateRequest<State> &Request,
    const TArray<const char *> &FieldAtoms) {
  return func::fold_array<const char *, func::Maybe<State>>(
      FieldAtoms, func::just<State>(Request.Initial),
      [Object = Request.Object](const func::Maybe<State> &Current,
                                const char *FieldAtom) {
        return func::mbind(Current, [Object, FieldAtom](const State &Value) {
          const func::Maybe<TJsonFieldDeclaration<State>> Field =
              FindRequiredJsonField<State>(FieldAtom);
          check(Field.hasValue);
          return Field.value.Apply({Object, Field.value.FieldName}, Value);
        });
      });
}

/** User Story: As a required-field registry consumer, I need to read initializer-list fields through a stable signature so small schemas remain composable. @fn template <typename State> func::Maybe<State> ReadRequiredFields( const FJsonReadStateRequest<State> &Request, std::initializer_list<const char *> FieldAtoms) */
template <typename State>
func::Maybe<State> ReadRequiredFields(
    const FJsonReadStateRequest<State> &Request,
    std::initializer_list<const char *> FieldAtoms) {
  return ReadRequiredFields<State>(Request, TArray<const char *>(FieldAtoms));
}

} // namespace ForbocAI::Game::Data::JsonValueAdapters
