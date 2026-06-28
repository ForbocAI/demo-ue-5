#include "Features/Components/Data/JsonValueAdapters.h"

#include "Features/Components/Data/DataAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonValueAdapters {

void LogInvalidField(const FJsonFieldRequest &Request) {
  UE_LOG(LogTemp, Error, TEXT("Runtime JSON invalid required field: %s"),
         *Request.FieldName);
}

void LogInvalidArrayValue(const FJsonArrayValueObjectRequest &Request) {
  UE_LOG(LogTemp, Error,
         TEXT("Runtime JSON array item invalid: %s[%d]"),
         *Request.FieldName, Request.Index);
}

func::Maybe<FString> ReadRequiredString(const FJsonFieldRequest &Request) {
  FString Value;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetStringField(Request.FieldName, Value)
             ? func::just(Value)
             : (LogInvalidField(Request), func::nothing<FString>());
}

func::Maybe<float> ReadRequiredFloat(const FJsonFieldRequest &Request) {
  double Value = 0.0;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetNumberField(Request.FieldName, Value)
             ? func::just(static_cast<float>(Value))
             : (LogInvalidField(Request), func::nothing<float>());
}

func::Maybe<TSharedPtr<FJsonObject>>
ReadRequiredObject(const FJsonFieldRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Value =
      DataAdapters::ReadObject(Request);
  return Value.hasValue
             ? Value
             : (LogInvalidField(Request),
                func::nothing<TSharedPtr<FJsonObject>>());
}

func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ReadRequiredArray(const FJsonFieldRequest &Request) {
  const TArray<TSharedPtr<FJsonValue>> *Values = nullptr;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetArrayField(Request.FieldName, Values) &&
                 Values != nullptr
             ? func::just(*Values)
             : (LogInvalidField(Request),
                func::nothing<TArray<TSharedPtr<FJsonValue>>>());
}

func::Maybe<TSharedPtr<FJsonObject>>
ReadArrayObject(const FJsonArrayValueObjectRequest &Request) {
  const TSharedPtr<FJsonObject> Object =
      Request.Value.IsValid() ? Request.Value->AsObject()
                              : TSharedPtr<FJsonObject>();
  return Object.IsValid()
             ? func::just(Object)
             : (LogInvalidArrayValue(Request),
                func::nothing<TSharedPtr<FJsonObject>>());
}

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
