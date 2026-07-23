#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCsvSyntaxSettings {
  FString CommentPrefix;
  FString CellDelimiter;
  FString ColorChannelDelimiter;
};

struct FCsvColorSettings {
  int32 ChannelCount;
  int32 RedChannelIndex;
  int32 GreenChannelIndex;
  int32 BlueChannelIndex;
  int32 Alpha;
};

struct FCsvGridSettings {
  int32 EmptyCount;
  int32 MinimumGridSize;
  int32 TerminalOffset;
};

struct FCsvMessageSettings {
  FString OrthoRowWidthMismatchFormat;
  FString OrthoBadColorCellFormat;
  FString OrthoInvalidFormat;
  FString OrthoLoadedFormat;
  FString TerrainLoadedFormat;
};

struct FCsvSettings {
  FCsvSyntaxSettings Syntax;
  FCsvColorSettings Color;
  FCsvGridSettings Grid;
  FCsvMessageSettings Messages;
};

/** User Story: As a level CSV consumer, I need to compare syntax settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FCsvSyntaxSettings &Left, const FCsvSyntaxSettings &Right) */
inline bool operator==(const FCsvSyntaxSettings &Left,
                       const FCsvSyntaxSettings &Right) {
  return Left.CommentPrefix == Right.CommentPrefix &&
         Left.CellDelimiter == Right.CellDelimiter &&
         Left.ColorChannelDelimiter == Right.ColorChannelDelimiter;
}

/** User Story: As a level CSV consumer, I need to compare syntax settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FCsvSyntaxSettings &Left, const FCsvSyntaxSettings &Right) */
inline bool operator!=(const FCsvSyntaxSettings &Left,
                       const FCsvSyntaxSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a level CSV consumer, I need to compare color settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FCsvColorSettings &Left, const FCsvColorSettings &Right) */
inline bool operator==(const FCsvColorSettings &Left,
                       const FCsvColorSettings &Right) {
  return Left.ChannelCount == Right.ChannelCount &&
         Left.RedChannelIndex == Right.RedChannelIndex &&
         Left.GreenChannelIndex == Right.GreenChannelIndex &&
         Left.BlueChannelIndex == Right.BlueChannelIndex &&
         Left.Alpha == Right.Alpha;
}

/** User Story: As a level CSV consumer, I need to compare color settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FCsvColorSettings &Left, const FCsvColorSettings &Right) */
inline bool operator!=(const FCsvColorSettings &Left,
                       const FCsvColorSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a level CSV consumer, I need to compare grid settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FCsvGridSettings &Left, const FCsvGridSettings &Right) */
inline bool operator==(const FCsvGridSettings &Left,
                       const FCsvGridSettings &Right) {
  return Left.EmptyCount == Right.EmptyCount &&
         Left.MinimumGridSize == Right.MinimumGridSize &&
         Left.TerminalOffset == Right.TerminalOffset;
}

/** User Story: As a level CSV consumer, I need to compare grid settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FCsvGridSettings &Left, const FCsvGridSettings &Right) */
inline bool operator!=(const FCsvGridSettings &Left,
                       const FCsvGridSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a level CSV consumer, I need to compare message settings for equality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator==(const FCsvMessageSettings &Left, const FCsvMessageSettings &Right) */
inline bool operator==(const FCsvMessageSettings &Left,
                       const FCsvMessageSettings &Right) {
  return Left.OrthoRowWidthMismatchFormat ==
             Right.OrthoRowWidthMismatchFormat &&
         Left.OrthoBadColorCellFormat == Right.OrthoBadColorCellFormat &&
         Left.OrthoInvalidFormat == Right.OrthoInvalidFormat &&
         Left.OrthoLoadedFormat == Right.OrthoLoadedFormat &&
         Left.TerrainLoadedFormat == Right.TerrainLoadedFormat;
}

/** User Story: As a level CSV consumer, I need to compare message settings for inequality through a stable signature so parsing state remains explicit and composable. @fn inline bool operator!=(const FCsvMessageSettings &Left, const FCsvMessageSettings &Right) */
inline bool operator!=(const FCsvMessageSettings &Left,
                       const FCsvMessageSettings &Right) {
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
