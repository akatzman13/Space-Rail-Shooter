Texture2D shaderTex;
SamplerState SampleType;

struct PixelInput
{
        float4 pos              :       SV_POSITION;
        float2 tex              :       TEXCOORD0;
};

float4 TexturePixelShader(PixelInput input)       : SV_TARGET
{
        float4 texColor;

        texColor = shaderTex.Sample(SampleType, input.tex);
        return texColor;
}