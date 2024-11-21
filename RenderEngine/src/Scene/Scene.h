//
// Created by Abdulrahmen on 9/27/2024.
//
#pragma once
#include "Core.h"
#include <vector>
#include <memory>
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "MathCore.h"

class EntityNode;
class Camera;
//class aiNode;
//class aiScene;

class RENDERENGINE_API Scene
{
    friend class Editor_Scene;

public:
    static Scene& Get();

    void LoadScene(const std::string &filePath);

    void AddEntity(std::shared_ptr<EntityNode> node)
    {
        m_entities.push_back(std::move(node));
    }

    void AddCamera(std::shared_ptr<Camera> camera)
    {
        if (m_cameras.empty()) m_activeCamera = camera;
        m_cameras.push_back(std::move(camera));
    }

    void SetActiveCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> GetActiveCamera() { return m_activeCamera; }

    void Update(float DT);
    void Render();

    const Matrix4x4& GetViewMatrix() const;
    float GetViewFoV() const;

private:
    void RegisterNode(EntityNode* pParent, const aiScene* pScene, const aiNode* pNode);

    std::vector<std::shared_ptr<EntityNode>> m_entities;
    //std::vector<std::shared_ptr<Light>> m_lights;
    std::vector<std::shared_ptr<Camera>> m_cameras;
    std::shared_ptr<Camera> m_activeCamera;
};


