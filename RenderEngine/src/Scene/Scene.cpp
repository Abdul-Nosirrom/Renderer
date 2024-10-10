//
// Created by Abdulrahmen on 9/27/2024.
//

#include "Scene.h"

#include "Camera.h"
#include "MeshEntity.h"

Matrix4x4 ConvertAssimpMatrix(const aiMatrix4x4& m)
{
	Matrix4x4 result;
	result <<	m.a1, m.a2, m.a3, m.a4,
				m.b1, m.b2, m.b3, m.b4,
				m.c1, m.c2, m.c3, m.c4,
				m.d1, m.d2, m.d3, m.d4;

	return result;
}

Scene& Scene::Get()
{
	static Scene instance;
	return instance;
}

const Matrix4x4 & Scene::GetViewMatrix() const
{
	return m_activeCamera->GetViewMatrix();
}

float Scene::GetViewFoV() const
{
	return m_activeCamera->GetFoV();
}

void Scene::LoadScene(Graphics& gfx, const std::string& filePath)
{
	Assimp::Importer modelLoader;
	const aiScene* pScene = modelLoader.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);
	if (!pScene)
	{
		throw std::exception(modelLoader.GetErrorString());
	}
	if (!pScene->HasMeshes()) throw std::exception("File contains no meshes...");

	auto pLocalRoot = pScene->mRootNode;

	// root has no parents
	RegisterNode(gfx, nullptr, pScene, pLocalRoot);

	// Add 'children' nodes
	//for (unsigned int i = 0; i < pLocalRoot->)
}

void Scene::RegisterNode(Graphics& gfx, EntityNode* pParent, const aiScene* pScene, const aiNode* pNode)
{
	//if (pNode->mNumMeshes == 0) return;

	// Add 'root' nodes (for now just adding 1 mesh since thats all we support)

	//for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
	if (pNode->mNumMeshes != 0)
	{
		auto meshIdx = pNode->mMeshes[0];//i];
		aiMesh* pMesh = pScene->mMeshes[meshIdx];
		auto entity = std::make_shared<MeshEntity>(gfx, pMesh);
		entity->m_Transform.SetMatrix(ConvertAssimpMatrix(pNode->mTransformation));
		if (pParent)
		{
			entity->SetParent(pParent);
			pParent->AddChild(std::move(entity));
		}
		else
		{
			entity->SetParent(nullptr);
			m_entities.push_back(std::move(entity));
		}
	}

	// Parse children
	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		aiNode* childNode = pNode->mChildren[i];
		RegisterNode(gfx, pParent, pScene, childNode);
	}

}


void Scene::SetActiveCamera(std::shared_ptr<Camera> camera)
{
	for (auto& cam : m_cameras)
	{
		cam->SetActive(false);
	}

	m_activeCamera = std::move(camera);
	m_activeCamera->SetActive(true);
}

void Scene::Update(float DT)
{
	m_activeCamera->Update(DT);
}

void Scene::Render(Graphics& gfx)
{
	for (auto& node : m_entities)
	{
		if (node.get())
		{
			node->OnRender(gfx); // Will then proceed to call draw on children
		}
	}
}
