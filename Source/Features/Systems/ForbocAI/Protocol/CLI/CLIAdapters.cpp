#include "Features/Systems/ForbocAI/Protocol/CLI/CLIAdapters.h"

#include "Core/frmt.hpp"
#include "Systems/CLI/CLIThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ProtocolCLI {
namespace {

/** User Story: As a forboc ai protocol cli consumer, I need to invoke command tokens through a stable signature so the forboc ai protocol cli workflow remains explicit and composable. @fn TArray<FString> CommandTokens(const FCommandRequest &Request) */
TArray<FString> CommandTokens(const FCommandRequest &Request) {
  return func::append_values<FString>(Request.Command.Tokens,
                                      Request.Arguments);
}

/** User Story: As a forboc ai protocol cli consumer, I need to invoke route mismatch through a stable signature so the forboc ai protocol cli workflow remains explicit and composable. @fn FString RouteMismatch(const FCommandRequest &Request, const FString &ResolvedKey) */
FString RouteMismatch(const FCommandRequest &Request,
                      const FString &ResolvedKey) {
  return frmt::RuntimeString(
      Request.Errors.RouteFamilyMismatchFormat,
      frmt::Args(
          {frmt::Arg(Request.Command.Family), frmt::Arg(ResolvedKey)}));
}

/** User Story: As a forboc ai protocol cli consumer, I need to invoke dispatch resolved through a stable signature so the forboc ai protocol cli workflow remains explicit and composable. @fn FCommandOutcome DispatchResolved( const FCommandRequest &Request, const ForbocAI::CLI::FCommandParseResult &Parsed) */
FCommandOutcome DispatchResolved(
    const FCommandRequest &Request,
    const ForbocAI::CLI::FCommandParseResult &Parsed) {
  const func::TestResult<void> Result =
      CLIOps::DispatchCommand(Parsed.CommandKey, Parsed.Args);
  const FString Message = UTF8_TO_TCHAR(Result.message.c_str());
  return Result.isSuccessful()
             ? func::right<FString, FCommandExecution>(
                   {Request.Command.Family, Parsed.CommandKey, Message})
             : func::left<FString, FCommandExecution>(Message);
}

} // namespace

/** User Story: As a forboc ai protocol cli consumer, I need to invoke execute command through a stable signature so the forboc ai protocol cli workflow remains explicit and composable. @fn FCommandOutcome ExecuteCommand(const FCommandRequest &Request) */
FCommandOutcome ExecuteCommand(const FCommandRequest &Request) {
  const ForbocAI::CLI::FCommandParseResult Parsed =
      CLIOps::ResolveCommandTokens(CommandTokens(Request));
  return !Parsed.bMatched
             ? func::left<FString, FCommandExecution>(
                   Request.Errors.CommandUnmatched)
             : !Parsed.CommandKey.StartsWith(Request.Command.KeyPrefix)
                   ? func::left<FString, FCommandExecution>(
                         RouteMismatch(Request, Parsed.CommandKey))
                   : DispatchResolved(Request, Parsed);
}

} // namespace ProtocolCLI
} // namespace Level
} // namespace Game
} // namespace ForbocAI
