cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
};

struct VertexInput
{
	float4 pos		:	POSITION;
	float2 tex              :       TEXCOORD0;
};

struct PixelInput
{
	float4 pos		:	SV_POSITION;
        float2 tex              :       TEXCOORD0;
};



PixelInput TextureVertexShader(VertexInput input)
{
        PixelInput output;

        input.pos.w = 1.0f;

        output.pos = mul(input.pos, worldMatrix);
        output.pos = mul(output.pos, viewMatrix);
        output.pos = mul(output.pos, projMatrix);

        output.tex = input.tex;
        
        return output;
}