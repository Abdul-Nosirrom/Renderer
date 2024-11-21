// Time related data
cbuffer Time 
{
    float time;
}

// Camera info


struct Interpolator
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 color : COLOR;
};

Interpolator VSMain( float3 pos : POSITION, float2 UVs : TEXCOORD, float3 norm : NORMAL, float3 color : COLOR)
{
	Interpolator outd;

	//pos += testTexture.SampleLevel(testTextureSampler, UVs, 0) * 0.25f * norm;
	outd.pos = float4((UVs - 0.5f) * 2, 0.f, 1.f);//mul(modelViewProj, float4(pos, 1.f));
	outd.uv = UVs;
	outd.color = color;
	return outd;
}

Texture2D colorTex : register(t0);
Texture2D posTex : register(t1);
Texture2D normTex : register(t2);

SamplerState anisoSampler;

float4 PSMain(Interpolator interpolators) : SV_TARGET
{
	const float N = normTex.Sample(anisoSampler, interpolators.uv);

	const float3 Ambient = float3(0, 0, 0.3);
	const float3 LightDir = normalize(float3(-1.f, 0.25f, -0.7f));
	float3 LightAtten = (dot(LightDir, N) + 1.f)/2.f * float3(1.f, 1.f, 1.f) + Ambient;

	return float4(LightAtten, 1.f) * colorTex.Sample(anisoSampler, interpolators.uv);
	//return float4(uv, 0, 1.f);
}