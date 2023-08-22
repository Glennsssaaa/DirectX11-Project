cbuffer lightBuffer : register(b0)
{
    float4 colour;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
    float4 inColour : COLOUR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return colour;
}