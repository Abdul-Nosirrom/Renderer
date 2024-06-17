
cbuffer TransformCBuffer
{
	matrix modelMatrix;
	matrix modelView;
	matrix modelViewProj;
};

struct Interpolator
{
	float4 pos : SV_POSITION;
	float4 worldPos : TEXCOORD0;
	float3 color : COLOR;
};

Interpolator VSMain( float3 pos : POSITION)
{
	Interpolator outd;
	outd.pos = mul(float4(pos, 1.f), modelViewProj);
	outd.color = outd.pos / 1.f;
	outd.worldPos = mul(float4(pos, 1.f), modelMatrix);
	return outd;
}

float4 PSMain(Interpolator interpolators) : SV_TARGET
{
	const float distToOrigin = distance(interpolators.worldPos.xyz, float3(0,0,0));
	if (distToOrigin >= 10.f) discard;

	return float4(1.f, 1.f, 1.f, 1.f) * distToOrigin / 50.f;
	//return float4(interpolators.color, 1.f);
	//return float4(1,1,1,1);
}