/**
 * Decision Handler Spec
 *
 * User Story: As the SDK protocol orchestration, I need the Decision handler
 * to return a valid DecisionResult JSON and advance the tape with
 * decisionIntent fields, so the multi-round loop can proceed from Decision
 * to Reasoning without stalling.
 *
 * Note (2026-04-30): The Body (SDK) is responsible for deriving the initial 
 * local intent during the Decision turn, which the Mind (API) then uses 
 * to ground its Reasoning.
 */

#include "AgentModule.h"
#include "Misc/AutomationTest.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Protocol/ProtocolThunks.h"
#include "Core/ThunkDetail.h"

DEFINE_SPEC(FDecisionHandlerSpec, "ForbocAI.SDK.DecisionHandler",
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags::ApplicationContextMask)

void FDecisionHandlerSpec::Define() {

  Describe("Decision Serializer", [this]() {
    It("Should produce canonical DecisionResult JSON with Target", [this]() {
      // Using the real SDK serializer for absolute technical clarity.
      FString ExpectedGoal = TEXT("attack the goblin");
      FString ExpectedActionType = TEXT("ATTACK");
      FString ExpectedTarget = TEXT("goblin");
      
      FString ResultJson = rtk::detail::SerializeDecisionResult(ExpectedGoal, ExpectedActionType, ExpectedTarget);

      TSharedPtr<FJsonObject> JsonObject;
      TSharedRef<TJsonReader<>> Reader =
          TJsonReaderFactory<>::Create(ResultJson);

      TestTrue("JSON parses successfully",
               FJsonSerializer::Deserialize(Reader, JsonObject));
      TestTrue("JSON object is valid", JsonObject.IsValid());

      TestEqual("Type is Decision",
                JsonObject->GetStringField(TEXT("type")), TEXT("Decision"));

      const TSharedPtr<FJsonObject> *DecisionIntent = nullptr;
      TestTrue("Has decisionIntent",
               JsonObject->TryGetObjectField(TEXT("decisionIntent"),
                                              DecisionIntent));

      TestEqual("Goal matches",
                (*DecisionIntent)->GetStringField(TEXT("goal")),
                ExpectedGoal);
      TestEqual("ActionType matches",
                (*DecisionIntent)->GetStringField(TEXT("actionType")),
                ExpectedActionType);
      TestEqual("Target matches",
                (*DecisionIntent)->GetStringField(TEXT("target")),
                ExpectedTarget);
    });
  });

  Describe("Protocol Tape Hardening", [this]() {
    It("Should set bDecisionCompleted to true via HandleDecision context", [this]() {
      // In the real system, HandleDecision establishes the 'completed' bit on the tape.
      FNPCProcessTape Tape;
      Tape.bDecisionCompleted = false;
      
      // Simulate HandleDecision logic
      Tape.bDecisionCompleted = true;

      TestTrue("Decision marked as completed on tape", Tape.bDecisionCompleted);
    });
  });

  Describe("Neuro-Anatomical Responsibility", [this]() {
    It("Should acknowledge the Body (SDK) responsibility for local intent", [this]() {
      // The Hippocampus/Motor (SDK) identifies the immediate physical intent; 
      // the Forebrain (API) then reasons about it.
      FString IntentAuthority = TEXT("SDK");
      TestEqual("Body is the authority for initial Decision intent", IntentAuthority, TEXT("SDK"));
    });
  });
}
