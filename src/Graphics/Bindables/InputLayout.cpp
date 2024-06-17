#include "InputLayout.h"

#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"

using namespace RenderResource;

InputLayout::InputLayout(Graphics& gfx, ID3DBlob* pVertexShaderBytecode)
{
    INFOMAN(gfx);
    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
        m_LayoutDesc.data(),
        UINT(m_LayoutDesc.size()),
        pVertexShaderBytecode->GetBufferPointer(),
        pVertexShaderBytecode->GetBufferSize(),
        &pInputLayout));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}

std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& gfx, ID3DBlob* pVertexShaderBytecode)
{
    return Pool::Resolve<InputLayout>(gfx, pVertexShaderBytecode);
}

std::string InputLayout::GenerateUID(ID3DBlob* pVertexShaderBytecode)
{
    const std::vector<D3D11_INPUT_ELEMENT_DESC> elemDesc = { InputElem::POSITION, InputElem::UV, InputElem::NORMAL, InputElem::COLOR };

    using namespace std::string_literals;
    std::string layoutID;
    // layout description for all our needs rn would be semantic name + format, then combine all of them
    for (auto elem : elemDesc)
    {
        const char* semanticName = elem.SemanticName;
        const DXGI_FORMAT inputFormat = elem.Format;

        layoutID += (semanticName + inputFormat);
        layoutID += "_";
    }

    return typeid(InputLayout).name() + "#"s + layoutID; 
}

std::string InputLayout::GetUID() const noexcept
{
    return GenerateUID();
}
