#pragma once

#include "Features/Components/Projection/ComponentsProjectionAdapters.h"
#include "Features/Components/Projection/ProjectionSelectors.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"

// ECS boundary adapters: converting application domains into ECS components and projection.
// Owns ProjectComponentValue overloads and ECS binding mapping.

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

/** User Story: As a features components consumer, I need to invoke component binding through a stable signature so the features components workflow remains explicit and composable. @fn FComponentProjectionBinding ComponentBinding(const ecs::ComponentType &Type, const ecs::FComponentValue &Value) */
FComponentProjectionBinding
ComponentBinding(const ecs::ComponentType &Type,
                 const ecs::FComponentValue &Value);

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const ecs::FComponentValue &Value) */
ecs::FComponentValue ProjectComponentValue(const ecs::FComponentValue &Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const FString &Value) */
ecs::FComponentValue ProjectComponentValue(const FString &Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(bool Value) */
ecs::FComponentValue ProjectComponentValue(bool Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(int32 Value) */
ecs::FComponentValue ProjectComponentValue(int32 Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(int64 Value) */
ecs::FComponentValue ProjectComponentValue(int64 Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(float Value) */
ecs::FComponentValue ProjectComponentValue(float Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(double Value) */
ecs::FComponentValue ProjectComponentValue(double Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const FVector &Value) */
ecs::FComponentValue ProjectComponentValue(const FVector &Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const TArray<ecs::FComponentValue> &Value) */
ecs::FComponentValue
ProjectComponentValue(const TArray<ecs::FComponentValue> &Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const FLevelLocalPoint &Value) */
ecs::FComponentValue ProjectComponentValue(const FLevelLocalPoint &Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const FRotator &Value) */
ecs::FComponentValue ProjectComponentValue(const FRotator &Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const TArray<FLevelLocalPoint> &Value) */
ecs::FComponentValue ProjectComponentValue(const TArray<FLevelLocalPoint> &Value);
/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const TArray<FString> &Value) */
ecs::FComponentValue ProjectComponentValue(const TArray<FString> &Value);

/** User Story: As a features components consumer, I need to invoke fcomponent value field through a stable signature so the features components workflow remains explicit and composable. @fn template <typename ValueType> FComponentValueField::FComponentValueField(const char *InName, const ValueType &InValue) */
template <typename ValueType>
FComponentValueField::FComponentValueField(const char *InName, const ValueType &InValue)
    : Name(ComponentAtom(InName)), Value(ProjectComponentValue(InValue)) {}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Value> typename std::enable_if<std::is_enum<Value>::value, ecs::FComponentValue>::type ProjectComponentValue(Value ValueText) */
template <typename Value>
typename std::enable_if<std::is_enum<Value>::value,
                        ecs::FComponentValue>::type
ProjectComponentValue(Value ValueText) {
  return ProjectComponentValue(RegisteredComponentText<Value>(ValueText));
}

template <typename Source> struct TComponentSourceProjector;

/** User Story: As a features components consumer, I need to invoke project component source value through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> ecs::FComponentValue ProjectComponentSourceValue(const Source &SourceValue) */
template <typename Source>
ecs::FComponentValue ProjectComponentSourceValue(const Source &SourceValue) {
  return TComponentSourceProjector<Source>()(SourceValue);
}

/** User Story: As a features components consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration() */
template <typename Source>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration()
    : Name(),
      Project([](const Source &) {
        return ProjectComponentValue(FString());
      }) {}

/** User Story: As a features components consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> template <typename Value> TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration( const char *InName, Value Source::*Member) */
template <typename Source>
template <typename Value>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration(
    const char *InName, Value Source::*Member)
    : Name(ComponentAtom(InName)),
      Project([Member](const Source &SourceValue) {
        return ProjectComponentValue(SourceValue.*Member);
      }) {}

/** User Story: As a features components consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> template <typename Value, typename Output> TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration( const char *InName, Value Source::*Member, Output (*Convert)(Value)) */
template <typename Source>
template <typename Value, typename Output>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration(
    const char *InName, Value Source::*Member, Output (*Convert)(Value))
    : Name(ComponentAtom(InName)),
      Project([Member, Convert](const Source &SourceValue) {
        return ProjectComponentValue(Convert(SourceValue.*Member));
      }) {}

/** User Story: As a features components consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> template <typename Value, typename Output> TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration( const char *InName, Value Source::*Member, Output (*Convert)(const Value &)) */
template <typename Source>
template <typename Value, typename Output>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration(
    const char *InName, Value Source::*Member,
    Output (*Convert)(const Value &))
    : Name(ComponentAtom(InName)),
      Project([Member, Convert](const Source &SourceValue) {
        return ProjectComponentValue(Convert(SourceValue.*Member));
      }) {}

/** User Story: As a features components consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> template <typename Output> TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration( const char *InName, Output (*ProjectValue)(const Source &)) */
template <typename Source>
template <typename Output>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration(
    const char *InName, Output (*ProjectValue)(const Source &))
    : Name(ComponentAtom(InName)),
      Project([ProjectValue](const Source &SourceValue) {
        return ProjectComponentValue(ProjectValue(SourceValue));
      }) {}

/** User Story: As a features components consumer, I need to invoke component source field declarations through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> inline TArray<TComponentSourceValueFieldDeclaration<Source>> ComponentSourceFieldDeclarations( std::initializer_list<TComponentSourceValueFieldDeclaration<Source>> Declarations) */
template <typename Source>
inline TArray<TComponentSourceValueFieldDeclaration<Source>>
ComponentSourceFieldDeclarations(
    std::initializer_list<TComponentSourceValueFieldDeclaration<Source>>
        Declarations) {
  return TArray<TComponentSourceValueFieldDeclaration<Source>>(Declarations);
}

/** User Story: As a features components consumer, I need to invoke component source value fields through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> inline TMap<FString, ecs::FComponentValue> ComponentSourceValueFields( const Source &SourceValue, std::initializer_list<TComponentSourceValueFieldDeclaration<Source>> Fields) */
template <typename Source>
inline TMap<FString, ecs::FComponentValue> ComponentSourceValueFields(
    const Source &SourceValue,
    std::initializer_list<TComponentSourceValueFieldDeclaration<Source>>
        Fields) {
  const TArray<TComponentSourceValueFieldDeclaration<Source>> FieldList(Fields);
  return func::fold_array<TComponentSourceValueFieldDeclaration<Source>,
                          TMap<FString, ecs::FComponentValue>>(
      FieldList, TMap<FString, ecs::FComponentValue>(),
      [&SourceValue](const TMap<FString, ecs::FComponentValue> &Acc,
                     const TComponentSourceValueFieldDeclaration<Source>
                         &Field) {
        TMap<FString, ecs::FComponentValue> Next = Acc;
        Next.Add(Field.Name, Field.Project(SourceValue));
        return Next;
      });
}

/** User Story: As a features components consumer, I need to invoke find component source value field through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> inline func::Maybe<TComponentSourceValueFieldDeclaration<Source>> FindComponentSourceValueField(const char *FieldAtom) */
template <typename Source>
inline func::Maybe<TComponentSourceValueFieldDeclaration<Source>>
FindComponentSourceValueField(const char *FieldAtom) {
  const FString FieldName = ComponentAtom(FieldAtom);
  const TArray<TComponentSourceValueFieldDeclaration<Source>> &Fields =
      TComponentSourceValueFieldRegistry<Source>::Fields();
  return func::fold_array<
      TComponentSourceValueFieldDeclaration<Source>,
      func::Maybe<TComponentSourceValueFieldDeclaration<Source>>>(
      Fields, func::nothing<TComponentSourceValueFieldDeclaration<Source>>(),
      [FieldName](const func::Maybe<TComponentSourceValueFieldDeclaration<
                      Source>> &Current,
                  const TComponentSourceValueFieldDeclaration<Source> &Field) {
        return Current.hasValue || Field.Name != FieldName
                   ? Current
                   : func::just(Field);
      });
}

/** User Story: As a features components consumer, I need to invoke component source value fields through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> inline TMap<FString, ecs::FComponentValue> ComponentSourceValueFields( const Source &SourceValue, const TArray<const char *> &FieldAtoms) */
template <typename Source>
inline TMap<FString, ecs::FComponentValue> ComponentSourceValueFields(
    const Source &SourceValue, const TArray<const char *> &FieldAtoms) {
  return func::fold_array<const char *, TMap<FString, ecs::FComponentValue>>(
      FieldAtoms, TMap<FString, ecs::FComponentValue>(),
      [&SourceValue](const TMap<FString, ecs::FComponentValue> &Acc,
                     const char *FieldAtom) {
        const func::Maybe<TComponentSourceValueFieldDeclaration<Source>>
            Field = FindComponentSourceValueField<Source>(FieldAtom);
        check(Field.hasValue);
        TMap<FString, ecs::FComponentValue> Next = Acc;
        Next.Add(Field.value.Name, Field.value.Project(SourceValue));
        return Next;
      });
}

/** User Story: As a features components consumer, I need to invoke component source value map through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> inline ecs::FComponentValue ComponentSourceValueMap( const Source &SourceValue, std::initializer_list<TComponentSourceValueFieldDeclaration<Source>> Fields) */
template <typename Source>
inline ecs::FComponentValue ComponentSourceValueMap(
    const Source &SourceValue,
    std::initializer_list<TComponentSourceValueFieldDeclaration<Source>>
        Fields) {
  return ecs::createMapComponentValue(
      ComponentSourceValueFields<Source>(SourceValue, Fields));
}

/** User Story: As a features components consumer, I need to invoke component source value map through a stable signature so the features components workflow remains explicit and composable. @fn template <typename Source> inline ecs::FComponentValue ComponentSourceValueMap( const Source &SourceValue, std::initializer_list<const char *> FieldAtoms) */
template <typename Source>
inline ecs::FComponentValue ComponentSourceValueMap(
    const Source &SourceValue, std::initializer_list<const char *> FieldAtoms) {
  return ecs::createMapComponentValue(ComponentSourceValueFields<Source>(
      SourceValue, TArray<const char *>(FieldAtoms)));
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
