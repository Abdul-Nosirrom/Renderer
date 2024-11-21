
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

	outd.worldPos = mul(modelMatrix, float4(pos, 1.f));
	return outd;
}


struct PSOutput
{
	float4 color : SV_TARGET0;
	float4 worldPos : SV_TARGET1;
	float4 worldNormal : SV_TARGET2;
};

PSOutput PSMain(Interpolator interpolators)
{
	//return testTexture.Sample(testTextureSampler, interpolators.uv);
	//return float4(interpolators.uv, 0, 1);

	PSOutput targets;

	targets.color = float4(0.3,0,0.6,1);//float4(normalize(LightAtten + Ambient) , 1.f);
	targets.worldNormal = float4((interpolators.normal + 1)/2, 1.f);
	targets.worldPos = interpolators.worldPos;

	return targets;
}