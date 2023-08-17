struct PS_INPUT
{
    float2 inPosition : POSITIONT;
    float3 inColor : COLOR;
};

float4 main(PS_INPUT INPUT) : SV_TARGET
{
    return float4(INPUT.inColor, 1.0f);
}