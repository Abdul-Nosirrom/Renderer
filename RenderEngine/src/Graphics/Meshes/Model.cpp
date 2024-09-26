#include "Model.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace RenderResource;

Model::Model(const std::string& inPath)
{
	m_GeometryTag = inPath.c_str();

	// Reset
	m_Vertices = VertexFactory();
	m_Indices.clear();

	Assimp::Importer modelLoader;

	const aiScene* loadedScene = modelLoader.ReadFile(inPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);
	if (!loadedScene) throw std::exception("Failed loading file...");
	if (!loadedScene->HasMeshes()) throw std::exception("File contains no meshes...");

	auto mesh = loadedScene->mMeshes[0];

	unsigned int numVerts = mesh->mNumVertices;

	for (unsigned int i = 0; i < numVerts; i++)
	{
		aiVector3D vertex = mesh->mVertices[i];
		//aiVector3D uv = mesh->mTextureCoords[i];

		Vertex vert;
		vert.SetPosition(Vector3(vertex.x, vertex.y, vertex.z) * 0.05f);
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
};
