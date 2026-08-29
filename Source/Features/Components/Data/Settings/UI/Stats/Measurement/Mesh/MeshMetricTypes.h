#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FMeshMetricSettings {
  int32 MeshLodIndex;
  int32 ForcedLodAutomaticModel;
  int32 LodModelIndexOffset;
  int32 ProcMeshFirstSectionIndex;
  int32 ProcMeshSectionStep;
  int32 TriangleIndexDivisor;
};

/** User Story: As a stats measurement consumer, I need to compare mesh metric settings so polygon counting remains deterministic. @fn inline bool operator==(const FMeshMetricSettings &Left, const FMeshMetricSettings &Right) */
inline bool operator==(const FMeshMetricSettings &Left,
                       const FMeshMetricSettings &Right) {
  return Left.MeshLodIndex == Right.MeshLodIndex &&
         Left.ForcedLodAutomaticModel == Right.ForcedLodAutomaticModel &&
         Left.LodModelIndexOffset == Right.LodModelIndexOffset &&
         Left.ProcMeshFirstSectionIndex == Right.ProcMeshFirstSectionIndex &&
         Left.ProcMeshSectionStep == Right.ProcMeshSectionStep &&
         Left.TriangleIndexDivisor == Right.TriangleIndexDivisor;
}

/** User Story: As a stats measurement consumer, I need to compare mesh metric settings for inequality so polygon counting remains deterministic. @fn inline bool operator!=(const FMeshMetricSettings &Left, const FMeshMetricSettings &Right) */
inline bool operator!=(const FMeshMetricSettings &Left,
                       const FMeshMetricSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
