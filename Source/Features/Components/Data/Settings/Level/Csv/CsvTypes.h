#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FSyntaxSettings {
  FString CommentPrefix;
  FString CellDelimiter;
  FString ColorChannelDelimiter;
};

struct FColorSettings {
  int32 ChannelCount;
  int32 RedChannelIndex;
  int32 GreenChannelIndex;
  int32 BlueChannelIndex;
  int32 Alpha;
};

struct FGridSettings {
  int32 EmptyCount;
  int32 MinimumGridSize;
  int32 TerminalOffset;
};

struct FMessageSettings {
  FString OrthoRowWidthMismatchFormat;
  FString OrthoBadColorCellFormat;
  FString OrthoInvalidFormat;
  FString OrthoLoadedFormat;
  FString TerrainLoadedFormat;
};

struct FCsvSettings {
  FSyntaxSettings Syntax;
  FColorSettings Color;
  FGridSettings Grid;
  FMessageSettings Messages;
};

/** User Story: As a level CSV consumer, I need to compare syntax settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FSyntaxSettings &Left, const FSyntaxSettings &Right) */
inline bool operator==(const FSyntaxSettings &Left,
                       const FSyntaxSettings &Right) {
  return Left.CommentPrefix == Right.CommentPrefix &&
         Left.CellDelimiter == Right.CellDelimiter &&
         Left.ColorChannelDelimiter == Right.ColorChannelDelimiter;
}

/** User Story: As a level CSV consumer, I need to compare syntax settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FSyntaxSettings &Left, const FSyntaxSettings &Right) */
inline bool operator!=(const FSyntaxSettings &Left,
                       const FSyntaxSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a level CSV consumer, I need to compare color settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FColorSettings &Left, const FColorSettings &Right) */
inline bool operator==(const FColorSettings &Left,
                       const FColorSettings &Right) {
  return Left.ChannelCount == Right.ChannelCount &&
         Left.RedChannelIndex == Right.RedChannelIndex &&
         Left.GreenChannelIndex == Right.GreenChannelIndex &&
         Left.BlueChannelIndex == Right.BlueChannelIndex &&
         Left.Alpha == Right.Alpha;
}

/** User Story: As a level CSV consumer, I need to compare color settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FColorSettings &Left, const FColorSettings &Right) */
inline bool operator!=(const FColorSettings &Left,
                       const FColorSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a level CSV consumer, I need to compare grid settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FGridSettings &Left, const FGridSettings &Right) */
inline bool operator==(const FGridSettings &Left,
                       const FGridSettings &Right) {
  return Left.EmptyCount == Right.EmptyCount &&
         Left.MinimumGridSize == Right.MinimumGridSize &&
         Left.TerminalOffset == Right.TerminalOffset;
}

/** User Story: As a level CSV consumer, I need to compare grid settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FGridSettings &Left, const FGridSettings &Right) */
inline bool operator!=(const FGridSettings &Left,
                       const FGridSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a level CSV consumer, I need to compare message settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FMessageSettings &Left, const FMessageSettings &Right) */
inline bool operator==(const FMessageSettings &Left,
                       const FMessageSettings &Right) {
  return Left.OrthoRowWidthMismatchFormat ==
             Right.OrthoRowWidthMismatchFormat &&
         Left.OrthoBadColorCellFormat == Right.OrthoBadColorCellFormat &&
         Left.OrthoInvalidFormat == Right.OrthoInvalidFormat &&
         Left.OrthoLoadedFormat == Right.OrthoLoadedFormat &&
         Left.TerrainLoadedFormat == Right.TerrainLoadedFormat;
}

/** User Story: As a level CSV consumer, I need to compare message settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FMessageSettings &Left, const FMessageSettings &Right) */
inline bool operator!=(const FMessageSettings &Left,
                       const FMessageSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a level CSV consumer, I need to compare composed settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FCsvSettings &Left, const FCsvSettings &Right) */
inline bool operator==(const FCsvSettings &Left,
                       const FCsvSettings &Right) {
  return Left.Syntax == Right.Syntax && Left.Color == Right.Color &&
         Left.Grid == Right.Grid && Left.Messages == Right.Messages;
}

/** User Story: As a level CSV consumer, I need to compare composed settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FCsvSettings &Left, const FCsvSettings &Right) */
inline bool operator!=(const FCsvSettings &Left,
                       const FCsvSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
