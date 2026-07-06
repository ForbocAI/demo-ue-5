#include "Features/Systems/Terrain/TerrainReducers.h"

#include "Core/ecs.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainReducers {
namespace {

struct FTerrainMeshBuildContext {
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  int32 SourceGridSize;
  int32 SourceGridLastIndex;
  int32 LodGridSize;
  int32 LodGridLastIndex;
  int32 LodStep;
  int32 QuadColumnStep;
  int32 QuadRowStep;
  float HalfSize;
  float Step;
  float VertexHeightOffset;
};

struct FTerrainMeshReserveRequest {
  int32 LodGridSize;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FTerrainLodGridSizeRequest {
  int32 SourceGridSize;
  int32 LodStep;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

FTerrainMeshPayload
ReserveTerrainMeshPayload(const FTerrainMeshReserveRequest &Request) {
  return (func::pipe(FTerrainMeshPayload{}) |
          [&Request](FTerrainMeshPayload Payload) {
            Payload.bLoaded = true;
            Payload.MaterialSlotIndex =
                Request.Geometry.TerrainMaterialSlotIndex;
            Payload.MeshSectionIndex = Request.Geometry.TerrainMeshSectionIndex;
            Payload.bCreateCollision = Request.Geometry.bTerrainCreateCollision;
            Payload.Vertices.Reserve(Request.LodGridSize *
                                     Request.LodGridSize);
            Payload.Normals.Reserve(Request.LodGridSize *
                                    Request.LodGridSize);
            Payload.UVs.Reserve(Request.LodGridSize * Request.LodGridSize);
            Payload.VertexColors.Reserve(Request.LodGridSize *
                                         Request.LodGridSize);
            Payload.Triangles.Reserve(
                (Request.LodGridSize -
                 Request.Geometry.TerrainGridTerminalOffset) *
                (Request.LodGridSize -
                 Request.Geometry.TerrainGridTerminalOffset) *
                Request.Geometry.TerrainQuadIndexReserveCount);
            return Payload;
          })
      .val;
}

int32 SelectTerrainLodStep(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return FMath::Max(Geometry.TerrainLodStep, Geometry.TerrainMinimumLodStep);
}

int32 SelectTerrainLodGridSize(const FTerrainLodGridSizeRequest &Request) {
  return ((Request.SourceGridSize -
           Request.Geometry.TerrainGridTerminalOffset) /
          Request.LodStep) +
         Request.Geometry.TerrainLodGridPadding;
}

int32 SelectTerrainSourceGridIndex(const FTerrainMeshBuildContext &Context,
                                   int32 LodIndex) {
  return LodIndex >= Context.LodGridLastIndex
             ? Context.SourceGridLastIndex
             : LodIndex * Context.LodStep;
}

FColor TerrainVertexColor(const FTerrainMeshBuildContext &Context, int32 Row,
                          int32 Column) {
  return Context.OrthoData.GetColorAt(Row, Column);
}

FTerrainMeshPayload AddTerrainVertex(const FTerrainMeshBuildContext &Context,
                                     int32 Row, int32 Column,
                                     FTerrainMeshPayload Payload) {
  const int32 SourceRow = SelectTerrainSourceGridIndex(Context, Row);
  const int32 SourceColumn = SelectTerrainSourceGridIndex(Context, Column);
  const float NorthSouth = Context.HalfSize - Context.Step * SourceRow;
  const float EastWest = -Context.HalfSize + Context.Step * SourceColumn;
  Payload.Vertices.Add(Context.TerrainData.ToWorld(
      EastWest, NorthSouth, Context.VertexHeightOffset));
  Payload.Normals.Add(FVector::UpVector);
  Payload.UVs.Add(FVector2D(
      static_cast<float>(SourceColumn) / Context.SourceGridLastIndex,
      static_cast<float>(SourceRow) / Context.SourceGridLastIndex));
  Payload.VertexColors.Add(TerrainVertexColor(Context, SourceRow,
                                              SourceColumn));
  return Payload;
}

FTerrainMeshPayload AddTerrainQuadTriangles(
    const FTerrainMeshBuildContext &Context, int32 Row, int32 Column,
    FTerrainMeshPayload Payload) {
  const int32 A = Row * Context.LodGridSize + Column;
  const int32 B = A + Context.QuadColumnStep;
  const int32 C = A + Context.LodGridSize * Context.QuadRowStep;
  const int32 D = C + Context.QuadColumnStep;
  Payload.Triangles.Add(A);
  Payload.Triangles.Add(C);
  Payload.Triangles.Add(B);
  Payload.Triangles.Add(B);
  Payload.Triangles.Add(C);
  Payload.Triangles.Add(D);
  return Payload;
}

FTerrainMeshPayload BuildLoadedTerrainMeshPayload(
    const FLevelTerrainData &TerrainData, const FLevelOrthoData &OrthoData,
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  const int32 SourceGridSize = TerrainData.GetGridSize();
  const int32 LodStep = SelectTerrainLodStep(Geometry);
  const int32 LodGridSize =
      SelectTerrainLodGridSize({SourceGridSize, LodStep, Geometry});
  const FTerrainMeshBuildContext Context{
      TerrainData, OrthoData, SourceGridSize,
      SourceGridSize - Geometry.TerrainGridTerminalOffset, LodGridSize,
      LodGridSize - Geometry.TerrainGridTerminalOffset, LodStep,
      Geometry.TerrainQuadColumnStep, Geometry.TerrainQuadRowStep,
      TerrainData.GetTerrainWorldSize() * Geometry.TerrainHalfWorldSizeScale,
      TerrainData.GetTerrainWorldSize() /
          static_cast<float>(SourceGridSize -
                             Geometry.TerrainGridTerminalOffset),
      Geometry.TerrainVertexHeightOffset};
  const FTerrainMeshPayload WithVertices =
      func::fold_grid_range<FTerrainMeshPayload>(
          LodGridSize, LodGridSize,
          ReserveTerrainMeshPayload({LodGridSize, Geometry}),
          [&Context](const FTerrainMeshPayload &Payload,
                     const func::GridIndex &Index) {
            return AddTerrainVertex(Context, static_cast<int32>(Index.Row),
                                    static_cast<int32>(Index.Column), Payload);
          });
  return func::fold_grid_range<FTerrainMeshPayload>(
      Context.LodGridLastIndex, Context.LodGridLastIndex, WithVertices,
      [&Context](const FTerrainMeshPayload &Payload,
                 const func::GridIndex &Index) {
        return AddTerrainQuadTriangles(Context, static_cast<int32>(Index.Row),
                                       static_cast<int32>(Index.Column),
                                       Payload);
      });
}

} // namespace

FTerrainState
ReduceTerrainLoaded(const FTerrainState &State,
                    const rtk::PayloadAction<FTerrainLoadedPayload> &Action) {
  return (func::pipe(State) | [&](FTerrainState Next) -> FTerrainState {
  Next.bTerrainLoaded = true;
  Next.TerrainSource = Action.PayloadValue.TerrainSource;
  Next.OrthoSource = Action.PayloadValue.OrthoSource;
  Next.GridSize = Action.PayloadValue.GridSize;
  Next.MinElevationMeters = Action.PayloadValue.MinElevationMeters;
  Next.MaxElevationMeters = Action.PayloadValue.MaxElevationMeters;
  return Next;
  }).val;
}

FTerrainMeshPayload
BuildTerrainMeshPayload(const FLevelTerrainData &TerrainData,
                        const FLevelOrthoData &OrthoData,
                        const ForbocAI::Game::Data::FLevelGeometrySettings
                            &Geometry) {
  return TerrainData.IsLoaded()
             ? (OrthoData.IsLoaded()
                    ? BuildLoadedTerrainMeshPayload(TerrainData, OrthoData,
                                                    Geometry)
                    : FTerrainMeshPayload{})
             : FTerrainMeshPayload{};
}

} // namespace TerrainReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
