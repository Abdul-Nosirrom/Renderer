﻿#pragma once
#include <memory>
#include <vector>
#include "MathCore.h"

// Forward declarations
namespace RenderResource
{
    class IBindable;
    class IndexBuffer;
}


/// @brief  Generic interface for anything that will have a drawcall
class IDrawable
{
public:
    IDrawable() = default;
    IDrawable(const IDrawable&) = delete;
    virtual ~IDrawable() = default;
    
    void Draw() const;

    virtual void Update(float DeltaTime) {};

    virtual const Matrix4x4& GetTransformMatrix() const noexcept = 0;

protected:
    template<class T>
    T* GetBindable() noexcept
    {
        for (auto& pb : m_Bindables)
        {
            if (auto pt = dynamic_cast<T*>(pb.get()))
            {
                return pt;
            }
        }
        return nullptr;
    }

    void AddBindable(std::shared_ptr<RenderResource::IBindable> bindable);
    
private:
    const RenderResource::IndexBuffer* pIndexBuffer = nullptr;
    std::vector<std::shared_ptr<RenderResource::IBindable>> m_Bindables;
};
