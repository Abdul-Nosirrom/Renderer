
struct TimeData
{
	float time;
	float3 padding;
};

cbuffer TimeBuffer : register(b0)
{
	TimeData tData;
};

struct VS_INPUT
{
	float2 pos : POSITION;
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
	outd.pos = float4(vsin.pos.x, vsin.pos.y, 0.f, 1.f);
	outd.cData = vsin.cData;
	return outd;
}

float4 PSMain(VS_OUT inData) : SV_TARGET
{
	return float4(pow(sin(tData.time), 2) * inData.cData, 1.f);
	//return float4(0.f, 1.f, 0.f, 1.f);
}

// Geometry shader that mirrors the triangle
[maxvertexcount(6)]
void GSMain(triangle VS_OUT gShaderInput[3], inout TriangleStream<VS_OUT> outStream)
{

	// Append our current input tri	
	[unroll(3)]
    for (int i = 0; i < 3; i++)
    {
        outStream.Append(gShaderInput[i]);
    }


	// Create new triangle
    VS_OUT outputElem;

	outputElem.pos = float4(0.5f, 0.5f, 0.f, 1.f); // Top Right
	outputElem.cData = gShaderInput[0].cData;
	outStream.Append(outputElem);
	outputElem.pos = float4(-0.5f, 0.5f, 0.f, 1.f); // Top Left
	outputElem.cData = gShaderInput[1].cData;
	outStream.Append(outputElem);
	outputElem.pos = float4(0.f, -0.5f, 0.f, 1.f); // Bottom Center
	outputElem.cData = gShaderInput[2].cData;
	outStream.Append(outputElem);
}