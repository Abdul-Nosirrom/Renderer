#include "InputLayout.h"

#include "Shaders.h"
#include "Primitives/Vertex.h"
#include "Errors/GraphicsExceptions.h"
#include "Graphics/RenderResourcePool.h"

using namespace RenderResource;

InputLayout::InputLayout()
{
    INFOMAN();

    GFX_THROW_INFO(GetDevice()->CreateInputLayout(
        VertexAttributes::InputLayout,
        VertexAttributes::NumElements,
        VertexShader::GetLastByteCode()->GetBufferPointer(),
        VertexShader::GetLastByteCode()->GetBufferSize(),
        &pInputLayout));
}

void InputLayout::Bind() noexcept
{
    GetContext()->IASetInputLayout(pInputLayout.Get());
}

std::shared_ptr<InputLayout> InputLayout::Resolve()
{
    return BindablePool::Resolve<InputLayout>();
}

std::string InputLayout::GenerateUID()
{
    //const std::vector<D3D11_INPUT_ELEMENT_DESC> elemDesc = { InputElem::POSITION, InputElem::UV, InputElem::NORMAL, InputElem::COLOR };

    using namespace std::string_literals;
    std::string layoutID;
    // layout description for all our needs rn would be semantic name + format, then combine all of them
    for (const auto& elem : VertexAttributes::InputLayout)
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
