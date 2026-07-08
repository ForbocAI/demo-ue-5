#pragma once

#include "Features/Components/Data/Json/Read/ReadAdapters.h"
#include "Features/Components/Data/Json/Settings/Field/JsonSettingsFieldAdapters.h"

#include <initializer_list>

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

#define JSON_SETTING_FIELD(Type, Field) {#Field, &Type::Field}
#define JSON_SETTING_FIELD_LIST_INDIRECT() JSON_SETTING_FIELD_LIST
#define JSON_SETTING_FIELD_LIST(Type, Field, ...)                            \
  JSON_SETTING_FIELD(Type, Field)                                            \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_SETTING_FIELD_LIST_INDIRECT)()(            \
                    Type, __VA_ARGS__))
#define JSON_SETTING_FIELDS(Type, ...)                                       \
  JSON_EXPAND(JSON_SETTING_FIELD_LIST(Type, __VA_ARGS__))
#define JSON_SETTINGS_FIELDS(Type, ...)                                      \
  {JSON_SETTING_FIELDS(Type, __VA_ARGS__)}
#define JSON_OBJECT_SETTING_FIELD(Type, Reader, Field)                       \
  {#Field, &Type::Field, Reader}
#define JSON_OBJECT_SETTING_FIELD_LIST_INDIRECT()                            \
  JSON_OBJECT_SETTING_FIELD_LIST
#define JSON_OBJECT_SETTING_FIELD_LIST(Type, Reader, Field, ...)             \
  JSON_OBJECT_SETTING_FIELD(Type, Reader, Field)                             \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_OBJECT_SETTING_FIELD_LIST_INDIRECT)()(      \
                    Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_SETTING_FIELDS(Type, Reader, ...)                        \
  JSON_EXPAND(JSON_OBJECT_SETTING_FIELD_LIST(Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_SETTINGS_FIELDS(Type, Reader, ...)                       \
  {JSON_OBJECT_SETTING_FIELDS(Type, Reader, __VA_ARGS__)}
#define JSON_OBJECT_ARRAY_SETTING_FIELD(Type, Reader, Field)                 \
  {#Field, &Type::Field, Reader}
#define JSON_OBJECT_ARRAY_SETTING_FIELD_LIST_INDIRECT()                      \
  JSON_OBJECT_ARRAY_SETTING_FIELD_LIST
#define JSON_OBJECT_ARRAY_SETTING_FIELD_LIST(Type, Reader, Field, ...)       \
  JSON_OBJECT_ARRAY_SETTING_FIELD(Type, Reader, Field)                       \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_OBJECT_ARRAY_SETTING_FIELD_LIST_INDIRECT)()(  \
                    Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_ARRAY_SETTING_FIELDS(Type, Reader, ...)                  \
  JSON_EXPAND(JSON_OBJECT_ARRAY_SETTING_FIELD_LIST(Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_ARRAY_SETTINGS_FIELDS(Type, Reader, ...)                 \
  {JSON_OBJECT_ARRAY_SETTING_FIELDS(Type, Reader, __VA_ARGS__)}

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
