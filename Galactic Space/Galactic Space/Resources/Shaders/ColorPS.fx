struct PixelInput
{
        float4 pos              :       SV_POSITION;
        float4 color            :       COLOR;
};

float4 ColorPixelShader(PixelInput input)       : SV_TARGET
{
        return input.color;
}