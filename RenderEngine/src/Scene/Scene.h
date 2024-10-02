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

class EntityNode;
class Graphics;
class Camera;
//class aiNode;
//class aiScene;

class RENDERENGINE_API Scene
{
public:
    static Scene* Get()
  	{
      if (!s_instance) s_instance = new Scene();
      return s_instance;
    }

    static void Destroy() { delete s_instance; }

    void LoadScene(Graphics& gfx, const std::string &filePath);

    void AddEntity(std::shared_ptr<EntityNode> node)
    {
        m_entities.push_back(std::move(node));
    }

    void Render(Graphics& gfx);

private:
    void RegisterNode(Graphics& gfx, EntityNode* pParent, const aiScene* pScene, const aiNode* pNode);

    inline static Scene* s_instance = nullptr;

    std::vector<std::shared_ptr<EntityNode>> m_entities;
    //std::vector<std::shared_ptr<Light>> m_lights;
    std::shared_ptr<Camera> m_camera;
};


