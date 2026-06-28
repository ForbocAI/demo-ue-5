#include "Features/Systems/Terrain/TerrainReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainReducers {
namespace {

struct FTerrainMeshBuildContext {
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  int32 GridSize;
  float HalfSize;
  float Step;
};

FTerrainMeshPayload ReserveTerrainMeshPayload(int32 GridSize) {
  return (func::pipe(FTerrainMeshPayload{}) |
          [GridSize](FTerrainMeshPayload Payload) {
            Payload.bLoaded = true;
            Payload.Vertices.Reserve(GridSize * GridSize);
            Payload.Normals.Reserve(GridSize * GridSize);
            Payload.UVs.Reserve(GridSize * GridSize);
            Payload.VertexColors.Reserve(GridSize * GridSize);
            Payload.Triangles.Reserve((GridSize - 1) * (GridSize - 1) * 6);
            return Payload;
          })
      .val;
}

FColor TerrainVertexColor(const FTerrainMeshBuildContext &Context, int32 Row,
                          int32 Column) {
  return Context.OrthoData.GetColorAt(Row, Column);
}

FTerrainMeshPayload AddTerrainVertex(const FTerrainMeshBuildContext &Context,
                                     int32 Row, int32 Column,
                                     FTerrainMeshPayload Payload) {
  const float NorthSouth = Context.HalfSize - Context.Step * Row;
  const float EastWest = -Context.HalfSize + Context.Step * Column;
  Payload.Vertices.Add(Context.TerrainData.ToWorld(EastWest, NorthSouth, 0.0f));
  Payload.Normals.Add(FVector::UpVector);
  Payload.UVs.Add(FVector2D(static_cast<float>(Column) / (Context.GridSize - 1),
                            static_cast<float>(Row) / (Context.GridSize - 1)));
  Payload.VertexColors.Add(TerrainVertexColor(Context, Row, Column));
  return Payload;
}

FTerrainMeshPayload BuildTerrainVertexColumns(
    const FTerrainMeshBuildContext &Context, int32 Row, int32 Column,
    FTerrainMeshPayload Payload) {
  return Column >= Context.GridSize
             ? Payload
             : BuildTerrainVertexColumns(
                   Context, Row, Column + 1,
                   AddTerrainVertex(Context, Row, Column, MoveTemp(Payload)));
}

FTerrainMeshPayload BuildTerrainVertexRows(
    const FTerrainMeshBuildContext &Context, int32 Row,
    FTerrainMeshPayload Payload) {
  return Row >= Context.GridSize
             ? Payload
             : BuildTerrainVertexRows(
                   Context, Row + 1,
                   BuildTerrainVertexColumns(Context, Row, 0,
                                             MoveTemp(Payload)));
}

FTerrainMeshPayload AddTerrainQuadTriangles(
    const FTerrainMeshBuildContext &Context, int32 Row, int32 Column,
    FTerrainMeshPayload Payload) {
  const int32 A = Row * Context.GridSize + Column;
  const int32 B = A + 1;
  const int32 C = A + Context.GridSize;
  const int32 D = C + 1;
  Payload.Triangles.Add(A);
  Payload.Triangles.Add(C);
  Payload.Triangles.Add(B);
  Payload.Triangles.Add(B);
  Payload.Triangles.Add(C);
  Payload.Triangles.Add(D);
  return Payload;
}

FTerrainMeshPayload BuildTerrainTriangleColumns(
    const FTerrainMeshBuildContext &Context, int32 Row, int32 Column,
    FTerrainMeshPayload Payload) {
  return Column >= Context.GridSize - 1
             ? Payload
             : BuildTerrainTriangleColumns(
                   Context, Row, Column + 1,
                   AddTerrainQuadTriangles(Context, Row, Column,
                                           MoveTemp(Payload)));
}

FTerrainMeshPayload BuildTerrainTriangleRows(
    const FTerrainMeshBuildContext &Context, int32 Row,
    FTerrainMeshPayload Payload) {
  return Row >= Context.GridSize - 1
             ? Payload
             : BuildTerrainTriangleRows(
                   Context, Row + 1,
                   BuildTerrainTriangleColumns(Context, Row, 0,
                                               MoveTemp(Payload)));
}

FTerrainMeshPayload BuildLoadedTerrainMeshPayload(
    const FLevelTerrainData &TerrainData, const FLevelOrthoData &OrthoData) {
  const int32 GridSize = TerrainData.GetGridSize();
  const FTerrainMeshBuildContext Context{
      TerrainData, OrthoData, GridSize,
      TerrainData.GetTerrainWorldSize() * 0.5f,
      TerrainData.GetTerrainWorldSize() /
          static_cast<float>(GridSize - 1)};
  return BuildTerrainTriangleRows(
      Context, 0,
      BuildTerrainVertexRows(Context, 0, ReserveTerrainMeshPayload(GridSize)));
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

FTerrainState ReduceTerrainMeshPayloadObserved(
    const FTerrainState &State,
    const rtk::PayloadAction<FTerrainMeshPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FTerrainState Next) -> FTerrainState {
            Next.LastMeshSection =
                ReduceTerrainMeshSectionViewModel(Action.PayloadValue);
            return Next;
          })
      .val;
}

FTerrainMeshPayload
BuildTerrainMeshPayload(const FLevelTerrainData &TerrainData,
                        const FLevelOrthoData &OrthoData) {
  return TerrainData.IsLoaded()
             ? (OrthoData.IsLoaded()
                    ? BuildLoadedTerrainMeshPayload(TerrainData, OrthoData)
                    : FTerrainMeshPayload{})
             : FTerrainMeshPayload{};
}

FTerrainMeshSectionViewModel
ReduceTerrainMeshSectionViewModel(const FTerrainMeshPayload &Payload) {
  return (func::pipe(FTerrainMeshSectionViewModel{}) |
          [&Payload](FTerrainMeshSectionViewModel Model) {
            Model.bLoaded = Payload.bLoaded;
            Model.Vertices = Payload.Vertices;
            Model.Triangles = Payload.Triangles;
            Model.Normals = Payload.Normals;
            Model.UVs = Payload.UVs;
            Model.VertexColors = Payload.VertexColors;
            return Model;
          })
      .val;
}

} // namespace TerrainReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
