
cbuffer TransformCBuffer
{
	matrix modelMatrix;
	matrix modelView;
	matrix modelViewProj;
};

cbuffer TimeCBuf
{
	float time;
	float3 padding;
}

struct Interpolator
{
	float4 pos : SV_POSITION;
	float4 worldPos : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 color : COLOR;
};

Interpolator VSMain( float3 pos : POSITION, float3 norm : NORMAL, float3 color : COLOR)
{
	Interpolator outd;

	outd.pos = mul(float4(pos, 1.f), modelViewProj);
	outd.color = color;
	outd.normal = mul(norm, (float3x3)modelMatrix); // cast to 3x3 to remove translation from model matrix
	outd.worldPos = mul(float4(pos, 1.f), modelMatrix);
	return outd;
}

float4 PSMain(Interpolator interpolators) : SV_TARGET
{
	const float3 Ambient = float3(0, 0, 0.3);
	const float3 LightDir = normalize(float3(-1.f, 0.25f, -0.7f));
	float3 LightAtten = (dot(LightDir, interpolators.normal) + 1.f)/2.f * float3(1.f, 1.f, 1.f);
	return float4(normalize(LightAtten + Ambient) , 1.f);
}