#pragma once
#include "GeometryProcessing.h"
#include "Vertex.h"
#include "Math/Vector.h"

struct Edge;
struct Face;

struct GEO_API HalfEdge
{
    HalfEdge* next = nullptr;
    HalfEdge* twin = nullptr;

    Vertex* vertex = nullptr;
    Edge* edge     = nullptr;
    Face* face     = nullptr;

    UINT vID;

    static std::vector<HalfEdge*> GenerateHalfEdges(std::vector<Vertex>& vertices, std::vector<Face>& faces);
};

struct GEO_API Edge
{
    HalfEdge* halfEdge;
};

struct GEO_API Face
{
    Face() noexcept = default;
    Face(const Eigen::Vector<UINT, 3>& inIndices)
        : vIndices(inIndices) {}

    Face& operator=(const Eigen::Vector<UINT, 3>& indices)
    {
        vIndices = indices;
        return *this;
    }

    //HalfEdge* halfEdge = nullptr;
    Eigen::Vector<UINT, 3> vIndices;
};