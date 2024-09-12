
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

	outd.pos = float4((UVs-0.5f) * 2, 0.f, 1.f);// mul(float4(pos, 1.f), modelViewProj);
	outd.uv = UVs;
	outd.color = color;
	outd.normal = mul(norm, (float3x3)modelMatrix); // cast to 3x3 to remove translation from model matrix

	outd.worldPos = mul(float4(pos, 1.f), modelMatrix);

	return outd;
}


// quartic polynomial
float smin( float a, float b, float k )
{
    k *= 16.0/3.0;
    float h = max( k-abs(a-b), 0.0 )/k;
    return min(a,b) - h*h*h*(4.0-h)*k*(1.0/16.0);
}

float Sphere(const float3 SpherePos, const float3 Pos, float Radius)
{
	return length(Pos - SpherePos) - Radius;
}

float DistanceToNearestSurface(const float3 CurrentPosition)
{
	float d1 = Sphere(float3(0.f, 2.f * sin(time), 0.f), CurrentPosition, 0.5f);
	float d2 = Sphere(float3(0.f, 0.f, 0.f), CurrentPosition, 0.65f);
	float d3 = Sphere(float3(3.f * cos(time), 0.f, 0.f), CurrentPosition, 1.f);
	float d4 = Sphere(float3(-2.f * cos(time), 3.f * sin(2.f * time), 2.f * sin(2.f * time)), CurrentPosition, 1.f);

	return smin(d4, smin(d3, smin(d1, d2, 0.2f), 0.1f), 0.4f);
}

float4 PSMain(Interpolator interpolators) : SV_TARGET
{
	//return float4(interpolators.uv, 0.f, 1.f);
	const int NumSteps = 128;
	const float3 Rayorigin = float3(0,0,-5.f);
	const float3 RayDir = float3(interpolators.uv * 2.f - 1.f, 1.f);
	const float StepSize = 0.1f;
	float T = 999990.f;
	[UNROLL]
	for (int i = 0; i < NumSteps; i++)
	{
		float3 Pos = Rayorigin + RayDir * (i * StepSize);
		float SDF = DistanceToNearestSurface(Pos);
		T = min(T, SDF);
		if (SDF < 0.f)
		{
			return float4(1.f, 1.f, 1.f, 1.f);// * sin(time);
		}
	}

	//clip(-1);
	return float4(0.f, 0.f, 0.f, 0.f);
	float tF = frac(T * 2.f);
	return float4(1.f, 1.f, 1.f, 1.f) * smoothstep(0.2, 0.21, tF) * saturate(T/2.f);
	return float4(1.f, 1.f, 1.f, 1.f) * saturate(T/2.f);
}