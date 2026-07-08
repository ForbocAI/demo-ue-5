#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Data/Json/Read/ReadAdapters.h"

#include <initializer_list>

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

void LogInvalidField(const FFieldRequest &Request);

func::Maybe<FString> ReadRequiredString(const FFieldRequest &Request);
func::Maybe<float> ReadRequiredFloat(const FFieldRequest &Request);
func::Maybe<TSharedPtr<FJsonObject>> ReadRequiredObject(const FFieldRequest &Request);
func::Maybe<TArray<TSharedPtr<FJsonValue>>> ReadRequiredArray(const FFieldRequest &Request);
func::Maybe<TSharedPtr<FJsonObject>> ReadArrayObject(const FArrayValueObjectRequest &Request);

template <typename Output>
TFunction<func::Maybe<TArray<Output>>(
    const TArray<TSharedPtr<FJsonValue>> &)>
MapRequiredJsonValuesWith(
    const FString &FieldName,
    TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue) {
  return [FieldName, MapValue](const TArray<TSharedPtr<FJsonValue>> &Values) {
    return func::traverse_maybe_array_with_index<TSharedPtr<FJsonValue>, Output>(
        Values,
        [FieldName, MapValue](const TSharedPtr<FJsonValue> &Value,
                              int32 Index) {
          return func::mbind(
              ReadArrayObject({Value, FieldName, Index}),
              [MapValue](const TSharedPtr<FJsonObject> &Object) {
                return MapValue(Object);
              });
      });
  };
}

inline FString RequiredFieldName(const char *FieldAtom) {
  return JsonAdapters::SettingsFieldName(FieldAtom);
}

inline FFieldRequest RequiredField(const TSharedPtr<FJsonObject> &Object,
                                       const char *FieldAtom) {
  return {Object, RequiredFieldName(FieldAtom)};
}

template <typename Value>
func::Maybe<Value> ReadRequiredValue(const FFieldRequest &Request);

template <>
inline func::Maybe<FString>
ReadRequiredValue<FString>(const FFieldRequest &Request) {
  return ReadRequiredString(Request);
}

template <>
inline func::Maybe<float>
ReadRequiredValue<float>(const FFieldRequest &Request) {
  return ReadRequiredFloat(Request);
}

template <>
inline func::Maybe<TSharedPtr<FJsonObject>>
ReadRequiredValue<TSharedPtr<FJsonObject>>(const FFieldRequest &Request) {
  return ReadRequiredObject(Request);
}

template <>
inline func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ReadRequiredValue<TArray<TSharedPtr<FJsonValue>>>(
    const FFieldRequest &Request) {
  return ReadRequiredArray(Request);
}

template <typename Value>
func::Maybe<Value> ReadRequiredField(const TSharedPtr<FJsonObject> &Object,
                                     const char *FieldAtom) {
  return ReadRequiredValue<Value>(RequiredField(Object, FieldAtom));
}

template <typename State, typename Value>
func::Maybe<State> AssignRequiredField(Value State::*Member,
                                       const State &Current,
                                       const Value &FieldValue) {
  State Next = Current;
  Next.*Member = FieldValue;
  return func::just(Next);
}

template <typename Output>
func::Maybe<TArray<Output>> ReadRequiredObjectArray(
    const FFieldRequest &Request,
    func::Maybe<Output> (*MapObject)(const TSharedPtr<FJsonObject> &)) {
  return func::mbind(
      ReadRequiredArray(Request),
      [Request,
       MapObject](const TArray<TSharedPtr<FJsonValue>> &Values) {
        return MapRequiredJsonValuesWith<Output>(Request.FieldName,
                                                 MapObject)(Values);
      });
}

template <typename Output> struct TRequiredJsonObjectMapper;

template <typename Output>
func::Maybe<TArray<Output>>
ReadRequiredMappedObjectArray(const FFieldRequest &Request) {
  return ReadRequiredObjectArray<Output>(
      Request, TRequiredJsonObjectMapper<Output>::Read);
}

template <typename State> struct TRequiredJsonFieldDeclaration {
  FString FieldName;
  TFunction<func::Maybe<State>(const FFieldRequest &, const State &)> Apply;

  TRequiredJsonFieldDeclaration()
      : FieldName(),
        Apply([](const FFieldRequest &, const State &Current) {
          return func::just(Current);
        }) {}

  template <typename Output>
  TRequiredJsonFieldDeclaration(const char *FieldAtom,
                                TArray<Output> State::*Member)
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member](const FFieldRequest &Request,
                       const State &Current) {
          return func::mbind(
              ReadRequiredMappedObjectArray<Output>(Request),
              [Member, Current](const TArray<Output> &FieldValue) {
                return AssignRequiredField<State, TArray<Output>>(
                    Member, Current, FieldValue);
              });
        }) {}

  template <typename Value>
  TRequiredJsonFieldDeclaration(const char *FieldAtom, Value State::*Member)
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member](const FFieldRequest &Request,
                       const State &Current) {
          return func::mbind(
              ReadRequiredValue<Value>(Request),
              [Member, Current](const Value &FieldValue) {
                return AssignRequiredField<State, Value>(Member, Current,
                                                         FieldValue);
              });
        }) {}

  template <typename Value>
  TRequiredJsonFieldDeclaration(
      const char *FieldAtom, Value State::*Member,
      func::Maybe<Value> (*ReadField)(const FFieldRequest &))
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member, ReadField](const FFieldRequest &Request,
                                  const State &Current) {
          return func::mbind(
              ReadField(Request), [Member, Current](const Value &FieldValue) {
                return AssignRequiredField<State, Value>(Member, Current,
                                                         FieldValue);
              });
        }) {}

  template <typename Output>
  TRequiredJsonFieldDeclaration(
      const char *FieldAtom, TArray<Output> State::*Member,
      func::Maybe<Output> (*MapObject)(const TSharedPtr<FJsonObject> &))
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member, MapObject](const FFieldRequest &Request,
                                  const State &Current) {
          return func::mbind(
              ReadRequiredObjectArray<Output>(Request, MapObject),
              [Member, Current](const TArray<Output> &FieldValue) {
                return AssignRequiredField<State, TArray<Output>>(
                    Member, Current, FieldValue);
              });
        }) {}
};

template <typename State>
struct FJsonReadStateRequest {
  State Initial;
  const TSharedPtr<FJsonObject> &Object;
};

template <typename State>
func::Maybe<State> ReadRequiredFields(
    const FJsonReadStateRequest<State> &Request,
    const TArray<TRequiredJsonFieldDeclaration<State>> &Fields) {
  return func::fold_array<TRequiredJsonFieldDeclaration<State>,
                          func::Maybe<State>>(
      Fields, func::just<State>(Request.Initial),
      [Object = Request.Object](const func::Maybe<State> &Current,
               const TRequiredJsonFieldDeclaration<State> &Field) {
        return func::mbind(Current, [Object, Field](const State &Value) {
          return Field.Apply({Object, Field.FieldName}, Value);
        });
      });
}

template <typename State>
func::Maybe<State> ReadRequiredFields(
    const FJsonReadStateRequest<State> &Request,
    std::initializer_list<TRequiredJsonFieldDeclaration<State>> Fields) {
  return ReadRequiredFields<State>(Request, TArray<TRequiredJsonFieldDeclaration<State>>(Fields));
}

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
    static const TArray<TRequiredJsonFieldDeclaration<Type>> &Fields() {      \
      static const TArray<TRequiredJsonFieldDeclaration<Type>>                \
          RegisteredFields = {JSON_REQUIRED_FIELDS(Type, __VA_ARGS__)};       \
      return RegisteredFields;                                               \
    }                                                                        \
  }

template <typename State>
func::Maybe<TRequiredJsonFieldDeclaration<State>>
FindRequiredJsonField(const char *FieldAtom) {
  const FString FieldName = RequiredFieldName(FieldAtom);
  const TArray<TRequiredJsonFieldDeclaration<State>> &Fields =
      TRequiredJsonFieldRegistry<State>::Fields();
  return func::fold_array<
      TRequiredJsonFieldDeclaration<State>,
      func::Maybe<TRequiredJsonFieldDeclaration<State>>>(
      Fields, func::nothing<TRequiredJsonFieldDeclaration<State>>(),
      [FieldName](const func::Maybe<TRequiredJsonFieldDeclaration<State>>
                      &Current,
                  const TRequiredJsonFieldDeclaration<State> &Field) {
        return Current.hasValue || Field.FieldName != FieldName
                   ? Current
                   : func::just(Field);
      });
}

template <typename State>
func::Maybe<State> ReadRequiredFields(
    const FJsonReadStateRequest<State> &Request,
    const TArray<const char *> &FieldAtoms) {
  return func::fold_array<const char *, func::Maybe<State>>(
      FieldAtoms, func::just<State>(Request.Initial),
      [Object = Request.Object](const func::Maybe<State> &Current, const char *FieldAtom) {
        return func::mbind(Current, [Object, FieldAtom](const State &Value) {
          const func::Maybe<TRequiredJsonFieldDeclaration<State>> Field =
              FindRequiredJsonField<State>(FieldAtom);
          check(Field.hasValue);
          return Field.value.Apply({Object, Field.value.FieldName}, Value);
        });
      });
}

template <typename State>
func::Maybe<State> ReadRequiredFields(
    const FJsonReadStateRequest<State> &Request,
    std::initializer_list<const char *> FieldAtoms) {
  return ReadRequiredFields<State>(Request, TArray<const char *>(FieldAtoms));
}

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
