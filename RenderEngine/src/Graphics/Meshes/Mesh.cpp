#include "Mesh.h"

#include <iostream>
#include <assimp/scene.h>           // Output data structure

#include "Graphics/RenderResourcePool.h"

using namespace RenderResource;

Mesh::Mesh(const aiMesh* mesh)
{
	//m_GeometryTag; // we still end up regenerating the vertices...
	m_GeometryTag = mesh->mName.C_Str();

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
		if (mesh->HasTextureCoords(0))
		{
			aiVector3D* texCoord = mesh->mTextureCoords[0];
			vert.SetUV(Vector2(texCoord->x, texCoord->y));

		}
		m_Vertices.AddVertex(vert);
	}

	unsigned int numFaces = mesh->mNumFaces;
	for (unsigned int i = 0; i < numFaces; i++)
	{
		aiFace* tri = mesh->mFaces;
		if (tri->mNumIndices == 3)
		{
			// Face
			m_Indices.push_back(tri[i].mIndices[0]);
			m_Indices.push_back(tri[i].mIndices[1]);
			m_Indices.push_back(tri[i].mIndices[2]);

			// Can use vertex indices to generate half-edges [edge 1 = i0 -> i1], can make a helper (GenerateHalfEdgeFromFace)
			// Only thing is an edge needs to hold a ref to another edge its shared with so its adjacent faces
		}
		//aiVector3D uv = mesh->mTextureCoords[i];
	}
}

Mesh::Mesh(GeometryFactory::EType primitiveType)
{
	switch (primitiveType)
	{
		case GeometryFactory::EType::Cube:
			m_GeometryTag = GeometryFactory::MakeCube(m_Vertices, m_Indices);
			break;
		case GeometryFactory::EType::Plane:
			m_GeometryTag = GeometryFactory::MakePlane(m_Vertices, m_Indices);
			break;
		case GeometryFactory::EType::Sphere:
			m_GeometryTag = GeometryFactory::MakeSphere(m_Vertices, m_Indices);
			break;
		case GeometryFactory::EType::Cylinder:
			m_GeometryTag = GeometryFactory::MakeCylinder(m_Vertices, m_Indices);
			break;
		default: ;
	}
}