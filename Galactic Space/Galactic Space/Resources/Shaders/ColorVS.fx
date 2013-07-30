//Color.vs/////////////////////////////////////

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
};

struct VertexInput
{
	float4 pos		:	POSITION;
	float4 color 	        :	COLOR;
};

struct PixelInput
{
	float4 pos		:	SV_POSITION;
	float4 color	        :	COLOR;	
};



PixelInput ColorVertexShader(VertexInput input)
{
        PixelInput output;

        input.pos.w = 1.0f;

        output.pos = mul(input.pos, worldMatrix);
        output.pos = mul(output.pos, viewMatrix);
        output.pos = mul(output.pos, projMatrix);

        output.color = input.color;
        
        return output;
}

