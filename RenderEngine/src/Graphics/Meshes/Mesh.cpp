#include "Mesh.h"

#include <format>
#include <iostream>
#include <assimp/scene.h>           // Output data structure

#include "Graphics/RenderResourcePool.h"

using namespace RenderResource;

Mesh::Mesh(const aiMesh* mesh)
{
	m_GeometryTag = mesh->mName.C_Str() ? mesh->mName.C_Str() : "ERROR";

	unsigned int numVerts = mesh->mNumVertices;

	for (unsigned int i = 0; i < numVerts; i++)
	{
		aiVector3D vertex = mesh->mVertices[i];
		//aiVector3D uv = mesh->mTextureCoords[i];

		Vertex vert;
		vert.SetPosition(Vector3(vertex.x, vertex.y, vertex.z) * 0.05f);// * 0.001f);
		if (mesh->HasNormals())
		{
			aiVector3D normal = mesh->mNormals[i];
			vert.SetNormal(Vector3(normal.x, normal.y, normal.z));
		}
		if (mesh->HasTangentsAndBitangents())
		{
			aiVector3D tangent = mesh->mTangents[i];
			vert.SetTangent(Vector3(tangent.x, tangent.y, tangent.z));
		}
		if (mesh->HasTextureCoords(0))
		{
			aiVector3D* texCoord = mesh->mTextureCoords[0];
			vert.SetUV(Vector2(texCoord->x, texCoord->y));
		}
		m_VertexFactory.AddVertex(vert);
	}

	unsigned int numFaces = mesh->mNumFaces;
	for (unsigned int i = 0; i < numFaces; i++)
	{
		aiFace* tri = mesh->mFaces;
		if (tri->mNumIndices == 3)
		{
			// Face
			m_VertexFactory.AddTri({{tri[i].mIndices[0], tri[i].mIndices[1], tri[i].mIndices[2]}});

			// Can use vertex indices to generate half-edges [edge 1 = i0 -> i1], can make a helper (GenerateHalfEdgeFromFace)
			// Only thing is an edge needs to hold a ref to another edge its shared with so its adjacent faces
		}
		//aiVector3D uv = mesh->mTextureCoords[i];
	}

	m_VertexFactory.GenerateMatrices();
}

Mesh::Mesh(GeometryFactory::EType primitiveType)
{
	switch (primitiveType)
	{
		case GeometryFactory::EType::Cube:
			m_GeometryTag = GeometryFactory::MakeCube(m_VertexFactory);
			break;
		case GeometryFactory::EType::Plane:
			m_GeometryTag = GeometryFactory::MakePlane(m_VertexFactory);
			break;
		case GeometryFactory::EType::Sphere:
			m_GeometryTag = GeometryFactory::MakeSphere(m_VertexFactory);
			break;
		case GeometryFactory::EType::Cylinder:
			m_GeometryTag = GeometryFactory::MakeCylinder(m_VertexFactory);
			break;
		default: ;
	}
}