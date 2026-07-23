#include "Features/Components/Data/Json/Value/JsonValueAdapters.h"

#include "Features/Components/Data/Json/Read/ReadAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

/** User Story: As a data json value consumer, I need to invoke log invalid field through a stable signature so the data json value workflow remains explicit and composable. @fn void LogInvalidField(const FFieldRequest &Request) */
void LogInvalidField(const FFieldRequest &Request) {
  UE_LOG(LogTemp, Warning, TEXT("Invalid JSON field: %s"),
         *Request.FieldName);
}

/** User Story: As a data json value consumer, I need to invoke read required string through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<FString> ReadRequiredString(const FFieldRequest &Request) */
func::Maybe<FString> ReadRequiredString(const FFieldRequest &Request) {
  FString Value;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetStringField(Request.FieldName, Value)
             ? func::just(Value)
             : func::nothing<FString>();
}

/** User Story: As a data json value consumer, I need to invoke read required float through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<float> ReadRequiredFloat(const FFieldRequest &Request) */
func::Maybe<float> ReadRequiredFloat(const FFieldRequest &Request) {
  double Value = 0.0;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetNumberField(Request.FieldName, Value)
             ? func::just(static_cast<float>(Value))
             : func::nothing<float>();
}

/** User Story: As a data json value consumer, I need to invoke read required object through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> ReadRequiredObject(const FFieldRequest &Request) */
func::Maybe<TSharedPtr<FJsonObject>>
ReadRequiredObject(const FFieldRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Value =
      JsonAdapters::ReadObject(Request);
  return Value.hasValue ? Value : func::nothing<TSharedPtr<FJsonObject>>();
}

/** User Story: As a data json value consumer, I need to invoke read required array through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<TArray<TSharedPtr<FJsonValue>>> ReadRequiredArray(const FFieldRequest &Request) */
func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ReadRequiredArray(const FFieldRequest &Request) {
  const TArray<TSharedPtr<FJsonValue>> *Values = nullptr;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetArrayField(Request.FieldName, Values) &&
                 Values != nullptr
             ? func::just(*Values)
             : func::nothing<TArray<TSharedPtr<FJsonValue>>>();
}

/** User Story: As a data json value consumer, I need to invoke read array object through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> ReadArrayObject(const FArrayValueObjectRequest &Request) */
func::Maybe<TSharedPtr<FJsonObject>>
ReadArrayObject(const FArrayValueObjectRequest &Request) {
  const TSharedPtr<FJsonObject> Object =
      Request.Value.IsValid() ? Request.Value->AsObject()
                              : TSharedPtr<FJsonObject>();
  return Object.IsValid()
             ? func::just(Object)
             : func::nothing<TSharedPtr<FJsonObject>>();
}

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
