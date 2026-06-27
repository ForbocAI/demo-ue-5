#include "Store.h"

#include "Features/Systems/Runtime/RuntimeFactories.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace Store {

rtk::EnhancedStore<FRuntimeState> ConfigureStore() {
  return rtk::configureStore<FRuntimeState>(
      RuntimeSlice::GetSlice().Reducer, RuntimeFactories::CreateInitialState());
}

rtk::EnhancedStore<FRuntimeState> &GetStore() {
  static rtk::EnhancedStore<FRuntimeState> SingletonStore = ConfigureStore();
  return SingletonStore;
}

} // namespace Store
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
