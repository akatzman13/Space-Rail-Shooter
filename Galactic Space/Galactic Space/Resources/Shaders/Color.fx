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