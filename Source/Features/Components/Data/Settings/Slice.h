#pragma once

#include "Features/Components/Data/Json/Read/Adapters.h"

// Runtime settings reducers: pure state transitions from JSON to typed values.
// "Given JSON object, compute typed color/value." No IO, no side effects.

namespace ForbocAI {
namespace Game {
namespace Data {
namespace SettingsReducers {

namespace Json = JsonAdapters;

/**
 * @brief Reads an FColor from a JSON object with r/g/b/a int fields.
 * @signature FColor ReadColorSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a settings adapter author, authored color values should
 * reduce through one neutral reader without per-field boilerplate.
 */
inline FColor ReadColorSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  return FColor(static_cast<uint8>(Int(TEXT("r"))),
                static_cast<uint8>(Int(TEXT("g"))),
                static_cast<uint8>(Int(TEXT("b"))),
                static_cast<uint8>(Int(TEXT("a"))));
}

/**
 * @brief Reads an FLinearColor from a JSON object with r/g/b/a float fields.
 * @signature FLinearColor ReadLinearColorSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a rendering settings adapter, linear color values should
 * reduce through one neutral reader for consistency with FColor.
 */
inline FLinearColor ReadLinearColorSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  return FLinearColor(Float(TEXT("r")), Float(TEXT("g")), Float(TEXT("b")),
                      Float(TEXT("a")));
}

} // namespace SettingsReducers
} // namespace Data
} // namespace Game
} // namespace ForbocAI
