
struct TimeData
{
	float time;
	float3 padding;
};

cbuffer Transforms : register(b0)
{
	matrix viewproj;
};

cbuffer TimeBuffer : register(b1)
{
	TimeData tData;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 cData : COLOR;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 cData : COLOR;
};

VS_OUT VSMain( VS_INPUT vsin )
{
	VS_OUT outd;
	outd.pos = mul(float4(vsin.pos, 1.f), viewproj);
	outd.cData = vsin.cData;

	return outd;
}

float4 PSMain(VS_OUT inData) : SV_TARGET
{
	return float4(pow(sin(tData.time), 2) * inData.cData, 1.f);
	//return float4(0.f, 1.f, 0.f, 1.f);
}