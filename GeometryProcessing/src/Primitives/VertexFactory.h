#pragma once
#include <vector>
#include "GeometryProcessing.h"
#include "HalfEdge.h"

using MeshMatrix = Eigen::Matrix<float, Eigen::Dynamic, 3>;

class GEO_API VertexFactory
{
public:
    VertexFactory() noexcept = default;
    VertexFactory(const std::vector<Vertex>& inVertices, const std::vector<Face>& inTris) noexcept
        : m_Vertices(inVertices), m_Faces(inTris)
    {
        m_HalfEdges = HalfEdge::GenerateHalfEdges(m_Vertices, m_Faces);
        GenerateMatrices();
    }

    void AddTri(const Face& InFace) noexcept { m_Faces.push_back(InFace); }
    void AddVertex(const Vertex& InVertex) noexcept { m_Vertices.push_back(InVertex); }

    void AddVertex(const Vector3& Pos) noexcept
    { Vertex Vert; Vert.SetPosition(Pos); AddVertex(Vert); };
    void AddVertex(const Vector3& Pos, const Vector2& UV) noexcept
    { Vertex Vert; Vert.SetPosition(Pos); Vert.SetUV(UV), AddVertex(Vert); };
    void AddVertex(const Vector3& Pos, const Vector3& Color) noexcept
    { Vertex Vert; Vert.SetPosition(Pos); Vert.SetColor(Color), AddVertex(Vert); };
    void AddVertex(const Vector3& Pos, const Vector3& Normal, const Vector2& UV) noexcept
    { Vertex Vert; Vert.SetPosition(Pos); Vert.SetNormal(Normal); Vert.SetUV(UV); AddVertex(Vert); };
    void AddVertex(const Vector3& Pos, const Vector3& Normal, const Vector2& UV, const Vector3& Color) noexcept
    { Vertex Vert; Vert.SetPosition(Pos); Vert.SetNormal(Normal); Vert.SetUV(UV); Vert.SetColor(Color); AddVertex(Vert); };

    void Init(const std::vector<Vertex>& vertices, const std::vector<Face>& indices)
    {
        m_Vertices = vertices;
        m_Faces = indices;
        m_HalfEdges = HalfEdge::GenerateHalfEdges(m_Vertices, m_Faces);
        GenerateMatrices();
    }

    void SetIndices(const std::vector<Face>& indices) { m_Faces = indices; }
    void SetVertices(const std::vector<Vertex>& vertices) { m_Vertices = vertices; }

    const void* VertexData() const  { return m_Vertices.data(); }
    const void* IndexData() const   { return m_Faces.data(); }

    size_t IndexCount() const       { return m_Faces.size() * 3; }
    size_t GetByteSize() const      { return m_Vertices.size() * sizeof(Vertex); }

    void GenerateMatrices();

    void SetPositions(MeshMatrix& positionMatrix);
    void SetColors(MeshMatrix& colorMatrix);
    void SetNormals(MeshMatrix& normalMatrix);
    void SetFaces(MeshMatrix& triMatrix);

public:

    std::vector<Vertex> m_Vertices;
    std::vector<Face> m_Faces;
    std::vector<HalfEdge*> m_HalfEdges;

    // Vertex position matrix [NumVerts, 3]
    MeshMatrix V;
    // Vertex color matrix [NumVerts, 3]
    MeshMatrix C;
    // Vertex normal matrix [NumVerts, 3]
    MeshMatrix N;
    // Triangle indices matrix [NumFaces, 3]
    MeshMatrix F;
};
