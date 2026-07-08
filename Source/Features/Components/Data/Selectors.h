#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataSelectors {

func::Maybe<FString> SelectLastPath(const FDataState &State);
bool SelectLastLoadSucceeded(const FDataState &State);
int32 SelectLoadedObjectCount(const FDataState &State);

} // namespace DataSelectors
} // namespace Data
} // namespace Game
} // namespace ForbocAI
