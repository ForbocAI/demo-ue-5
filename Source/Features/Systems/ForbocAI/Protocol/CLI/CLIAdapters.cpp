#include "Features/Systems/ForbocAI/Protocol/CLI/CLIAdapters.h"

#include "Core/frmt.hpp"
#include "Systems/CLI/CLIThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ProtocolCLI {
namespace {

TArray<FString> CommandTokens(const FCommandRequest &Request) {
  return func::append_values<FString>(Request.Command.Tokens,
                                      Request.Arguments);
}

FString RouteMismatch(const FCommandRequest &Request,
                      const FString &ResolvedKey) {
  return frmt::RuntimeString(
      Request.Errors.RouteFamilyMismatchFormat,
      frmt::Args(
          {frmt::Arg(Request.Command.Family), frmt::Arg(ResolvedKey)}));
}

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
