
cbuffer TransformCBuffer
{
	matrix modelMatrix;
	matrix modelView;
	matrix modelViewProj;
};

cbuffer TimeCBuf : register(b1)
{
	float time;
	float3 padding;
}

struct Interpolator
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 worldPos : TEXCOORD1;
	float3 normal : NORMAL;
	float3 color : COLOR;
};

Texture2D testTexture;
SamplerState testTextureSampler;

Interpolator VSMain( float3 pos : POSITION, float2 UVs : TEXCOORD, float3 norm : NORMAL, float3 color : COLOR)
{
	Interpolator outd;

	//pos += testTexture.SampleLevel(testTextureSampler, UVs, 0) * 0.25f * norm;
	outd.pos = mul(modelViewProj, float4(pos, 1.f));
	outd.uv = UVs;
	outd.color = color;
	outd.normal = mul(norm, (float3x3)modelMatrix); // cast to 3x3 to remove translation from model matrix

	outd.worldPos = mul(float4(pos, 1.f), modelMatrix);
	return outd;
}


float4 PSMain(Interpolator interpolators) : SV_TARGET
{
	return testTexture.Sample(testTextureSampler, interpolators.uv);
	return float4(interpolators.uv, 0, 1);
	const float3 Ambient = float3(0, 0, 0.3);
	const float3 LightDir = normalize(float3(-1.f, 0.25f, -0.7f));
	float3 LightAtten = (dot(LightDir, interpolators.normal) + 1.f)/2.f * float3(1.f, 1.f, 1.f);
	return float4(normalize(LightAtten + Ambient) , 1.f);
}