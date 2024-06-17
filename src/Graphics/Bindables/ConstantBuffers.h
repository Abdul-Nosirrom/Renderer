#pragma once
#include "IBindable.h"
#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"

namespace RenderResource
{
    //////////////////////////////////////////////////////////////////////////
    // CONSTANT BUFFER GENERIC INTERFACE
    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    class ConstantBuffer : public IBindable
    {
    public:
        void Update(Graphics& gfx, const T& cbufData)
        {
            INFOMAN(gfx);

            D3D11_MAPPED_SUBRESOURCE cbufMap;
            GFX_THROW_INFO(GetContext(gfx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &cbufMap));
            memcpy(cbufMap.pData, &cbufData, sizeof(cbufData));
            GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
        }

        ConstantBuffer(Graphics& gfx, const T& cbufData, UINT slot = 0u) : m_Slot(slot)
        {
            INFOMAN(gfx);

            D3D11_BUFFER_DESC cbufDesc;
            cbufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbufDesc.Usage = D3D11_USAGE_DYNAMIC;
            cbufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbufDesc.MiscFlags = 0u;
            cbufDesc.ByteWidth = sizeof(cbufData);
            cbufDesc.StructureByteStride = 0u;

            D3D11_SUBRESOURCE_DATA cbufSRD = {};
            cbufSRD.pSysMem = &cbufData;

            GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbufDesc, &cbufSRD, &pConstantBuffer));
        }

        ConstantBuffer(Graphics& gfx, UINT slot = 0u) : m_Slot(slot)
        {
            INFOMAN(gfx);

            D3D11_BUFFER_DESC cbufDesc;
            cbufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbufDesc.Usage = D3D11_USAGE_DYNAMIC;
            cbufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbufDesc.MiscFlags = 0u;
            cbufDesc.ByteWidth = sizeof(T);
            cbufDesc.StructureByteStride = 0u;

            GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbufDesc, nullptr, &pConstantBuffer));
        }
        
    protected:
        Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
        UINT m_Slot;
    };

    //////////////////////////////////////////////////////////////////////////
    // VERTEX SHADER CONSTANT BUFFER
    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    class VertexConstantBuffer : public ConstantBuffer<T>
    {
        using ConstantBuffer<T>::pConstantBuffer;
        using ConstantBuffer<T>::m_Slot;
        using IBindable::GetContext;
    public:
        using ConstantBuffer<T>::ConstantBuffer;

        void Bind(Graphics& gfx) noexcept override
        {
            GetContext(gfx)->VSSetConstantBuffers(m_Slot, 1u, pConstantBuffer.GetAddressOf());
        }

        static std::shared_ptr<VertexConstantBuffer> Resolve(Graphics& gfx, UINT slot = 0)
        {
            return Pool::Resolve<VertexConstantBuffer>(gfx, slot);
        }
        
        static std::string GenerateUID(UINT slot)
        {
            using namespace std::string_literals;
            return typeid(VertexConstantBuffer).name() + "#"s + std::to_string( slot );
        }

        std::string GetUID() const noexcept override
        {
            return GenerateUID(m_Slot);
        }
    };
    
    //////////////////////////////////////////////////////////////////////////
    // PIXEL SHADER CONSTANT BUFFER
    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    class PixelConstantBuffer : public ConstantBuffer<T>
    {
        using ConstantBuffer<T>::pConstantBuffer;
        using ConstantBuffer<T>::m_Slot;
        using IBindable::GetContext;
    public:
        using ConstantBuffer<T>::ConstantBuffer;

        void Bind(Graphics& gfx) noexcept override
        {
            GetContext(gfx)->PSSetConstantBuffers(m_Slot, 1u, pConstantBuffer.GetAddressOf());
        }

        static std::shared_ptr<PixelConstantBuffer> Resolve(Graphics& gfx, UINT slot = 0)
        {
            return Pool::Resolve<PixelConstantBuffer>(gfx, slot);
        }
        
        static std::string GenerateUID(UINT slot)
        {
            using namespace std::string_literals;
            return typeid(PixelConstantBuffer).name() + "#"s + std::to_string( slot );
        }

        std::string GetUID() const noexcept override
        {
            return GenerateUID(m_Slot);
        }
    };
}
