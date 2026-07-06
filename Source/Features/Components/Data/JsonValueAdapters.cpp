#include "Features/Components/Data/JsonValueAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

void LogInvalidField(const FJsonFieldRequest &Request) {
  UE_LOG(LogTemp, Warning, TEXT("Invalid JSON field: %s"),
         *Request.FieldName);
}

func::Maybe<FString> ReadRequiredString(const FJsonFieldRequest &Request) {
  FString Value;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetStringField(Request.FieldName, Value)
             ? func::just(Value)
             : func::nothing<FString>();
}

func::Maybe<float> ReadRequiredFloat(const FJsonFieldRequest &Request) {
  double Value = 0.0;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetNumberField(Request.FieldName, Value)
             ? func::just(static_cast<float>(Value))
             : func::nothing<float>();
}

func::Maybe<TSharedPtr<FJsonObject>>
ReadRequiredObject(const FJsonFieldRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Value =
      JsonAdapters::ReadObject(Request);
  return Value.hasValue ? Value : func::nothing<TSharedPtr<FJsonObject>>();
}

func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ReadRequiredArray(const FJsonFieldRequest &Request) {
  const TArray<TSharedPtr<FJsonValue>> *Values = nullptr;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetArrayField(Request.FieldName, Values) &&
                 Values != nullptr
             ? func::just(*Values)
             : func::nothing<TArray<TSharedPtr<FJsonValue>>>();
}

func::Maybe<TSharedPtr<FJsonObject>>
ReadArrayObject(const FJsonArrayValueObjectRequest &Request) {
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
