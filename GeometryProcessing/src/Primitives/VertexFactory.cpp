//
// Created by Abdulrahmen on 10/22/2024.
//

#include "VertexFactory.h"

void VertexFactory::GenerateMatrices()
{
    // Initialize sizes
    int numVerts = m_Vertices.size();
    int numIndices = m_Faces.size();

    V.resize(numVerts, 3);
    C.resize(numVerts, 3);
    N.resize(numVerts, 3);
    F.resize(numIndices, 3);

    // Fill up vertex matrix
    for (int i = 0; i < numVerts; i++)
    {
        V.row(i) = m_Vertices[i].GetPosition();
        C.row(i) = m_Vertices[i].GetColor();
        N.row(i) = m_Vertices[i].GetNormal();
    }

    // Fill up indices
    for (int i = 0; i < numIndices; i++)
    {
        //F.row(i) = m_Faces[i].vIndices;
    }
}

void VertexFactory::SetPositions(MeshMatrix& positionMatrix)
{
    // Ensure proper size
    int numVerts = m_Vertices.size();
    assert(positionMatrix.rows() == numVerts && positionMatrix.cols() == 3);

    for (int i = 0; i < numVerts; i++)
    {
        m_Vertices[i].SetPosition(positionMatrix.row(i));
    }
}

void VertexFactory::SetColors(MeshMatrix& colorMatrix)
{
    // Ensure proper size
    int numVerts = m_Vertices.size();
    assert(colorMatrix.rows() == numVerts && colorMatrix.cols() == 3);

    for (int i = 0; i < numVerts; i++)
    {
        m_Vertices[i].SetColor(colorMatrix.row(i));
    }
}

void VertexFactory::SetNormals(MeshMatrix& normalMatrix)
{
    // Ensure proper size
    int numVerts = m_Vertices.size();
    assert(normalMatrix.rows() == numVerts && normalMatrix.cols() == 3);

    for (int i = 0; i < numVerts; i++)
    {
        m_Vertices[i].SetNormal(normalMatrix.row(i));
    }
}

void VertexFactory::SetFaces(MeshMatrix& triMatrix)
{
    // Ensure proper size
    int numTris = m_Faces.size();
    assert(triMatrix.rows() == numTris && triMatrix.cols() == 3);

    for (int i = 0; i < numTris; i++)
    {
        //m_Faces[i].vIndices = triMatrix.row(i);
    }
}
