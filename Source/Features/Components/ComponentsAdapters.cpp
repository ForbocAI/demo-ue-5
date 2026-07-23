#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

/** User Story: As a features components consumer, I need to invoke component binding through a stable signature so the features components workflow remains explicit and composable. @fn FComponentProjectionBinding ComponentBinding(const ecs::ComponentType &Type, const ecs::FComponentValue &Value) */
FComponentProjectionBinding
ComponentBinding(const ecs::ComponentType &Type,
                 const ecs::FComponentValue &Value) {
  return {Type, Value};
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const ecs::FComponentValue &Value) */
ecs::FComponentValue ProjectComponentValue(const ecs::FComponentValue &Value) {
  return Value;
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const FString &Value) */
ecs::FComponentValue ProjectComponentValue(const FString &Value) {
  return ecs::createTextComponentValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(bool Value) */
ecs::FComponentValue ProjectComponentValue(bool Value) {
  return ecs::createBoolComponentValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(int32 Value) */
ecs::FComponentValue ProjectComponentValue(int32 Value) {
  return ecs::createIntComponentValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(int64 Value) */
ecs::FComponentValue ProjectComponentValue(int64 Value) {
  return ecs::createIntComponentValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(float Value) */
ecs::FComponentValue ProjectComponentValue(float Value) {
  return ecs::createFloatComponentValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(double Value) */
ecs::FComponentValue ProjectComponentValue(double Value) {
  return ecs::createFloatComponentValue(static_cast<float>(Value));
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const FVector &Value) */
ecs::FComponentValue ProjectComponentValue(const FVector &Value) {
  return ecs::createVec3ComponentValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const TArray<ecs::FComponentValue> &Value) */
ecs::FComponentValue
ProjectComponentValue(const TArray<ecs::FComponentValue> &Value) {
  return ecs::createListComponentValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const FLevelLocalPoint &Value) */
ecs::FComponentValue ProjectComponentValue(const FLevelLocalPoint &Value) {
  return LocalPointValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const FRotator &Value) */
ecs::FComponentValue ProjectComponentValue(const FRotator &Value) {
  return RotationValue(Value);
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const TArray<FLevelLocalPoint> &Value) */
ecs::FComponentValue ProjectComponentValue(const TArray<FLevelLocalPoint> &Value) {
  return ecs::createListComponentValue(LocalPointList(Value));
}

/** User Story: As a features components consumer, I need to invoke project component value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue ProjectComponentValue(const TArray<FString> &Value) */
ecs::FComponentValue ProjectComponentValue(const TArray<FString> &Value) {
  return ecs::createListComponentValue(StringList(Value));
}

/** User Story: As a features components consumer, I need to invoke project resource through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FWorld ProjectResource(const FProjectResourcePayload &Payload) */
ecs::FWorld ProjectResource(const FProjectResourcePayload &Payload) {
  return ecs::setResource({Payload.World, Payload.Name, Payload.Value});
}

namespace {

struct FProjectDomainPayload {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  TArray<FString> Segments;
};

struct FProjectComponentPayload {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  ecs::FComponentValue Value;
};

struct FDomainProjectionStep {
  ecs::EntityKey Entity;
  TArray<FString> Segments;
};

struct FComponentProjectionStep {
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  ecs::FComponentValue Value;
};

struct FProjectDomainStepsPayload {
  ecs::FWorld World;
  TArray<FDomainProjectionStep> Steps;
};

struct FProjectComponentStepsPayload {
  ecs::FWorld World;
  TArray<FComponentProjectionStep> Steps;
};

struct FProjectStepsPayload {
  ecs::FWorld World;
  TArray<FDomainProjectionStep> Domains;
  TArray<FComponentProjectionStep> Components;
};

/** User Story: As a features components consumer, I need to invoke domain key through a stable signature so the features components workflow remains explicit and composable. @fn ecs::DomainPathKey DomainKey(const TArray<FString> &Segments) */
ecs::DomainPathKey DomainKey(const TArray<FString> &Segments) {
  return ecs::createDomainPathKey(ecs::createDomainPath(Segments));
}

/** User Story: As a features components consumer, I need to invoke domain step through a stable signature so the features components workflow remains explicit and composable. @fn FDomainProjectionStep DomainStep(const ecs::EntityKey &Entity, const TArray<FString> &Segments) */
FDomainProjectionStep DomainStep(const ecs::EntityKey &Entity,
                                 const TArray<FString> &Segments) {
  return {Entity, Segments};
}

/** User Story: As a features components consumer, I need to invoke domain steps through a stable signature so the features components workflow remains explicit and composable. @fn TArray<FDomainProjectionStep> DomainSteps(const ecs::EntityKey &Entity, const TArray<TArray<FString>> &SegmentGroups) */
TArray<FDomainProjectionStep>
DomainSteps(const ecs::EntityKey &Entity,
            const TArray<TArray<FString>> &SegmentGroups) {
  return func::map_array<TArray<FString>, FDomainProjectionStep>(
      SegmentGroups, [&Entity](const TArray<FString> &Segments) {
        return DomainStep(Entity, Segments);
      });
}

/** User Story: As a features components consumer, I need to invoke component step through a stable signature so the features components workflow remains explicit and composable. @fn FComponentProjectionStep ComponentStep(const ecs::EntityKey &Entity, const FComponentProjectionBinding &Binding) */
FComponentProjectionStep ComponentStep(const ecs::EntityKey &Entity,
                                       const FComponentProjectionBinding &Binding) {
  return {Entity, Binding.Type, Binding.Value};
}

/** User Story: As a features components consumer, I need to invoke component steps through a stable signature so the features components workflow remains explicit and composable. @fn TArray<FComponentProjectionStep> ComponentSteps(const ecs::EntityKey &Entity, const TArray<FComponentProjectionBinding> &Bindings) */
TArray<FComponentProjectionStep>
ComponentSteps(const ecs::EntityKey &Entity,
               const TArray<FComponentProjectionBinding> &Bindings) {
  return func::map_array<FComponentProjectionBinding, FComponentProjectionStep>(
      Bindings, [&Entity](const FComponentProjectionBinding &Binding) {
        return ComponentStep(Entity, Binding);
      });
}

/** User Story: As a features components consumer, I need to invoke project domain through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FWorld ProjectDomain(const FProjectDomainPayload &Payload) */
ecs::FWorld ProjectDomain(const FProjectDomainPayload &Payload) {
  return ecs::setEntityDomain(
      {Payload.World, Payload.Entity, DomainKey(Payload.Segments)});
}

/** User Story: As a features components consumer, I need to invoke project component through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FWorld ProjectComponent(const FProjectComponentPayload &Payload) */
ecs::FWorld ProjectComponent(const FProjectComponentPayload &Payload) {
  return ecs::setComponent(
      {Payload.World, Payload.Entity, Payload.Type, Payload.Value});
}

/** User Story: As a features components consumer, I need to invoke project domain step through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FWorld ProjectDomainStep(const ecs::FWorld &World, const FDomainProjectionStep &Step) */
ecs::FWorld ProjectDomainStep(const ecs::FWorld &World,
                              const FDomainProjectionStep &Step) {
  return ProjectDomain({World, Step.Entity, Step.Segments});
}

/**
 * @fn ecs::FWorld ProjectDomainSteps(const FProjectDomainStepsPayload &Payload)
 * @brief Applies domain projection steps to a world through the ECS fold.
 *
 * User Story: As a systems feature author, I need repeated ECS domain
 * projection to live in neutral component infrastructure instead of sibling
 * systems helpers.
 */
ecs::FWorld ProjectDomainSteps(const FProjectDomainStepsPayload &Payload) {
  return func::fold_array<FDomainProjectionStep, ecs::FWorld>(
      Payload.Steps, Payload.World,
      [](const ecs::FWorld &Acc, const FDomainProjectionStep &Step) {
        return ProjectDomainStep(Acc, Step);
      });
}

/** User Story: As a features components consumer, I need to invoke project component step through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FWorld ProjectComponentStep(const ecs::FWorld &World, const FComponentProjectionStep &Step) */
ecs::FWorld ProjectComponentStep(const ecs::FWorld &World,
                                 const FComponentProjectionStep &Step) {
  return ProjectComponent({World, Step.Entity, Step.Type, Step.Value});
}

/**
 * @fn ecs::FWorld ProjectComponentSteps(const FProjectComponentStepsPayload &Payload)
 * @brief Applies component projection steps to a world through the ECS fold.
 *
 * User Story: As feature projection code, I need reusable component batch
 * application that keeps reducers and adapters small.
 */
ecs::FWorld
ProjectComponentSteps(const FProjectComponentStepsPayload &Payload) {
  return func::fold_array<FComponentProjectionStep, ecs::FWorld>(
      Payload.Steps, Payload.World,
      [](const ecs::FWorld &Acc, const FComponentProjectionStep &Step) {
        return ProjectComponentStep(Acc, Step);
      });
}

/**
 * @fn ecs::FWorld ProjectSteps(const FProjectStepsPayload &Payload)
 * @brief Applies domain and component projection steps through one ECS world transition.
 *
 * User Story: As projection features, domain and component batches should use
 * one neutral application primitive instead of each feature owning the same
 * pipe shape.
 */
ecs::FWorld ProjectSteps(const FProjectStepsPayload &Payload) {
  return (func::pipe(Payload.World) |
          [&Payload](ecs::FWorld Next) {
            return ProjectDomainSteps({Next, Payload.Domains});
          } |
          [&Payload](ecs::FWorld Next) {
            return ProjectComponentSteps({Next, Payload.Components});
          })
      .val;
}

} // namespace

/**
 * @fn ecs::FWorld ProjectEntity(const FProjectEntityPayload &Payload)
 * @brief Projects one entity from domain path data and component bindings.
 *
 * User Story: As feature projection code, I need the repeated entity/domain/
 * component projection idiom to live in one ECS-facing adapter boundary.
 */
ecs::FWorld ProjectEntity(const FProjectEntityPayload &Payload) {
  return ProjectSteps(
      {Payload.World, DomainSteps(Payload.Entity, Payload.Domains),
       ComponentSteps(Payload.Entity, Payload.Components)});
}

/** User Story: As a features components consumer, I need to invoke local point value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point) */
ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point) {
  return ComponentValueMap({{"eastWest", Point.EastWest},
                            {"northSouth", Point.NorthSouth},
                            {"heightOffset", Point.HeightOffset}});
}

/** User Story: As a features components consumer, I need to invoke rotation value through a stable signature so the features components workflow remains explicit and composable. @fn ecs::FComponentValue RotationValue(const FRotator &Rotation) */
ecs::FComponentValue RotationValue(const FRotator &Rotation) {
  return ComponentValueMap({{"pitch", Rotation.Pitch},
                            {"yaw", Rotation.Yaw},
                            {"roll", Rotation.Roll}});
}

/** User Story: As a features components consumer, I need to invoke local point list through a stable signature so the features components workflow remains explicit and composable. @fn TArray<ecs::FComponentValue> LocalPointList(const TArray<FLevelLocalPoint> &Points) */
TArray<ecs::FComponentValue>
LocalPointList(const TArray<FLevelLocalPoint> &Points) {
  return ecs::mapComponentValues<FLevelLocalPoint>(
      Points, [](const FLevelLocalPoint &Point) {
        return LocalPointValue(Point);
      });
}

/** User Story: As a features components consumer, I need to invoke string list through a stable signature so the features components workflow remains explicit and composable. @fn TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values) */
TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values) {
  return ecs::mapComponentValues<FString>(
      Values, [](const FString &Value) { return ecs::createTextComponentValue(Value); });
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
