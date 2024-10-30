#pragma once
#include "IBindable.h"
#include <memory>

namespace RenderResource
{
    namespace InputElem
    {
        inline constexpr D3D11_INPUT_ELEMENT_DESC POSITION =
        {
            "Position",
            0u,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0u,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0u
        };

        inline constexpr D3D11_INPUT_ELEMENT_DESC UV =
        {
            "Texcoord",
            0u,
            DXGI_FORMAT_R32G32_FLOAT,
            0u,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0u
        };


        inline constexpr D3D11_INPUT_ELEMENT_DESC NORMAL =
        {
            "Normal",
            0u,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0u,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0u
        };

        inline constexpr D3D11_INPUT_ELEMENT_DESC COLOR =
        {
            "Color",
            0u,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0u,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0u
        };
    }
    
    // NOTE: atm we don't have a really robust input layout/vertex factory so should just keep the input layout constant for now till we get to that point
    class InputLayout : public IBindable
    {
    public:
        InputLayout(Graphics& gfx, ID3DBlob* pVertexShaderBytecode);
        void Bind(Graphics& gfx) noexcept override;

        static std::shared_ptr<InputLayout> Resolve(Graphics& gfx, ID3DBlob* pVertexShaderBytecode);
        static std::string GenerateUID(ID3DBlob* pVertexShaderBytecode = nullptr);

        std::string GetUID() const noexcept override;

    protected:
        //std::vector<D3D11_INPUT_ELEMENT_DESC> m_LayoutDesc = { InputElem::POSITION, InputElem::UV, InputElem::NORMAL, InputElem::COLOR };
        Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    };
    
}
