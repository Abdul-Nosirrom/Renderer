#pragma once
#include "Math.h"

#include "IBindable.h"
#include <memory>

// TODO: Figure out VertexType
namespace RenderResource
{
    struct Vertex
    {
	public:
		Vertex() noexcept = default;

		void SetPosition(Vector3 InPos) noexcept
		{
			Position = InPos;
		}
		void SetUV(Vector2 InUV) noexcept
		{
			UV = InUV;
		}
		void SetNormal(Vector3 InNormal) noexcept
		{
			Normal = InNormal;
		}
		void SetColor(Vector3 InColor) noexcept
		{
			Color = InColor;
		}
	protected:
        Vector3 Position = { 0,0,0 };
        Vector2 UV = { 0,0 };
        Vector3 Normal = { 0,0,0 };
        Vector3 Color = { 0,0,0 };
    };

    struct VertexFactory
    {
    public:
        VertexFactory() noexcept = default;
        
        void AddVertex(const Vertex& InVertex) noexcept { m_VertexData.push_back(InVertex); }

        void AddVertex(Vector3 Pos) noexcept
        { Vertex Vert; Vert.SetPosition(Pos); AddVertex(Vert); };
        void AddVertex(Vector3 Pos, Vector2 UV) noexcept
        { Vertex Vert; Vert.SetPosition(Pos); Vert.SetUV(UV), AddVertex(Vert); };
        void AddVertex(Vector3 Pos, Vector3 Color) noexcept
        { Vertex Vert; Vert.SetPosition(Pos); Vert.SetColor(Color), AddVertex(Vert); };
        void AddVertex(Vector3 Pos, Vector3 Normal, Vector2 UV) noexcept
        { Vertex Vert; Vert.SetPosition(Pos); Vert.SetNormal(Normal); Vert.SetUV(UV); AddVertex(Vert); };
        void AddVertex(Vector3 Pos, Vector3 Normal, Vector2 UV, Vector3 Color) noexcept
        { Vertex Vert; Vert.SetPosition(Pos); Vert.SetNormal(Normal); Vert.SetUV(UV); Vert.SetColor(Color); AddVertex(Vert); };
       
        const void* Data() const { return m_VertexData.data(); }
        size_t GetByteSize() const { return m_VertexData.size() * sizeof(Vertex); }

    private:
        std::vector<Vertex> m_VertexData;
    };
    
    class VertexBuffer : public IBindable
    {
    public:
        VertexBuffer(Graphics& gfx, const std::string& poolTag, const VertexFactory& vertexData);

        void Bind(Graphics& gfx) noexcept override;

        static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& poolTag, const VertexFactory& vertexData);
        // NOTE: this is needed so the signature matches with Resolve but we don't wanna pass the rest into it
        template<typename...Ignore>
        static std::string GenerateUID(const std::string& poolTag, Ignore&&...ignore)
        {
            return GenerateUID_Internal(poolTag);
        }
        
        std::string GetUID() const noexcept override { return GenerateUID_Internal(m_PoolTag); }

    private:
        static std::string GenerateUID_Internal(const std::string& poolTag);

    protected:
        std::string m_PoolTag;
        UINT m_Stride;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    };
}
