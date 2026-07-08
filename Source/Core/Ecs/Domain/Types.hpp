#pragma once

namespace ecs {


enum class EDomainKind {
  Component,
  Entity,
  System,
  Ui,
  Service,
  Session,
  Data,
  Input,
  Audio,
  Rendering,
  Physics,
  Ai,
  Narrative,
  Procgen,
  Unknown
};

struct FDomainPath {
  TArray<FString> Segments;
};

struct FFieldSchema {
  FString Name;
  FString Kind;
  bool bRequired;
  FString Description;
};

struct FComponentSchema {
  ComponentType Type;
  DomainPathKey Domain;
  TArray<FFieldSchema> Fields;
  TArray<Tag> Tags;
  FString Description;
};

struct FCapabilitySpec {
  FString Name;
  DomainPathKey Domain;
  TArray<ComponentType> Reads;
  TArray<ComponentType> Writes;
  TArray<ResourceName> Resources;
  TArray<EventType> Emits;
};

struct FSystemSpec {
  SystemName Name;
  DomainPathKey Domain;
  TArray<ComponentType> RequiredComponents;
  TArray<Tag> RequiredTags;
  TArray<ResourceName> Resources;
  TArray<EventType> Emits;
};

struct FResourceSpec {
  ResourceName Name;
  DomainPathKey Domain;
  FString Kind;
  FString Description;
};

struct FEventSpec {
  EventType Type;
  DomainPathKey Domain;
  TArray<FFieldSchema> Fields;
  FString Description;
};

struct FDomainNode {
  FDomainPath Path;
  EDomainKind Kind;
  TArray<DomainPathKey> Children;
  TArray<ComponentType> ComponentSchemas;
  TArray<FString> Capabilities;
  TArray<SystemName> Systems;
  TArray<ResourceName> Resources;
  TArray<EventType> Events;
};

struct FDomainGraph {
  TMap<DomainPathKey, FDomainNode> Nodes;
  TMap<ComponentType, FComponentSchema> ComponentSchemas;
  TMap<FString, FCapabilitySpec> Capabilities;
  TMap<SystemName, FSystemSpec> Systems;
  TMap<ResourceName, FResourceSpec> ResourceSpecs;
  TMap<EventType, FEventSpec> EventSpecs;
};

struct FCreateDomainNodeRequest {
  FDomainPath Path;
  EDomainKind Kind = EDomainKind::Unknown;
};

struct FCreateFieldSchemaRequest {
  FString Name;
  FString Kind;
  bool bRequired = false;
  FString Description;
};

struct FCreateComponentSchemaRequest {
  ComponentType Type;
  DomainPathKey Domain;
  TArray<FFieldSchema> Fields;
  TArray<Tag> Tags;
  FString Description;
};

struct FCreateCapabilitySpecRequest {
  FString Name;
  DomainPathKey Domain;
  TArray<ComponentType> Reads;
  TArray<ComponentType> Writes;
  TArray<ResourceName> Resources;
  TArray<EventType> Emits;
};

struct FCreateSystemSpecRequest {
  SystemName Name;
  DomainPathKey Domain;
  TArray<ComponentType> Components;
  TArray<Tag> Tags;
  TArray<ResourceName> Resources;
  TArray<EventType> Emits;
};

struct FCreateResourceSpecRequest {
  ResourceName Name;
  DomainPathKey Domain;
  FString Kind;
  FString Description;
};

struct FCreateEventSpecRequest {
  EventType Type;
  DomainPathKey Domain;
  TArray<FFieldSchema> Fields;
  FString Description;
};

struct FRegisterDomainRequest {
  FDomainGraph Registry;
  FDomainNode Node;
};

struct FRegisterDomainPathRequest {
  FDomainGraph Registry;
  TArray<FString> Segments;
  EDomainKind Kind = EDomainKind::Unknown;
};

struct FDomainPathRegistration {
  TArray<FString> Segments;
  EDomainKind Kind = EDomainKind::Unknown;

  FDomainPathRegistration() {}

  FDomainPathRegistration(std::initializer_list<const char *> InSegments,
                          EDomainKind InKind)
      : Kind(InKind) {
    const TArray<const char *> SegmentAtoms(InSegments);
    Segments = func::map_array<const char *, FString>(
        SegmentAtoms, [](const char *Segment) {
          return FString(UTF8_TO_TCHAR(Segment));
        });
  }
};

struct FRegisterDomainPathsRequest {
  FDomainGraph Registry;
  TArray<FDomainPathRegistration> Registrations;
};

struct FRegisterComponentSchemaRequest {
  FDomainGraph Registry;
  FComponentSchema Schema;
};

struct FRegisterCapabilitySpecRequest {
  FDomainGraph Registry;
  FCapabilitySpec Spec;
};

struct FRegisterSystemSpecRequest {
  FDomainGraph Registry;
  FSystemSpec Spec;
};

struct FRegisterResourceSpecRequest {
  FDomainGraph Registry;
  FResourceSpec Spec;
};

struct FRegisterEventSpecRequest {
  FDomainGraph Registry;
  FEventSpec Spec;
};

/**
 * @brief Creates a neutral ECS domain path from ordered path segments.
 * @signature inline FDomainPath createDomainPath(const TArray<FString> &Segments)
 *
 * User Story: As a feature author, I need stable domain paths so higher
 * gameplay domains can import downward into shared ECS primitives.
 */

} // namespace ecs
