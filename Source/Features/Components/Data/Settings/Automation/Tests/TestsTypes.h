#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Tests {

struct FBddSettings {
  FString Spec;
  FString Group;
  FString Case;
  FString Assertion;
};

struct FContractParitySettings {
  FString Test;
  FString HeadersAvailableAssertion;
  FString SkipWarning;
  FString RunEnvironmentVariable;
  int32 MinimumScenarioSteps;
};

struct FIntegrationVerificationSettings {
  FString Test;
  FString QuarantineWarning;
};

struct FSettings {
  FBddSettings Bdd;
  FContractParitySettings ContractParity;
  FIntegrationVerificationSettings IntegrationVerification;
};

inline bool operator==(const FBddSettings &Left,
                       const FBddSettings &Right) {
  return Left.Spec == Right.Spec && Left.Group == Right.Group &&
         Left.Case == Right.Case && Left.Assertion == Right.Assertion;
}

inline bool operator!=(const FBddSettings &Left,
                       const FBddSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FContractParitySettings &Left,
                       const FContractParitySettings &Right) {
  return Left.Test == Right.Test &&
         Left.HeadersAvailableAssertion == Right.HeadersAvailableAssertion &&
         Left.SkipWarning == Right.SkipWarning &&
         Left.RunEnvironmentVariable == Right.RunEnvironmentVariable &&
         Left.MinimumScenarioSteps == Right.MinimumScenarioSteps;
}

inline bool operator!=(const FContractParitySettings &Left,
                       const FContractParitySettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FIntegrationVerificationSettings &Left,
                       const FIntegrationVerificationSettings &Right) {
  return Left.Test == Right.Test &&
         Left.QuarantineWarning == Right.QuarantineWarning;
}

inline bool operator!=(const FIntegrationVerificationSettings &Left,
                       const FIntegrationVerificationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Bdd == Right.Bdd &&
         Left.ContractParity == Right.ContractParity &&
         Left.IntegrationVerification == Right.IntegrationVerification;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Tests
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
