cbuffer lightBuffer : register(b0)
{
    float3 ambientColour;
    float ambientLightStrength;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    //float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    
    float3 sampleColour = input.inNormal;
    float3 ambientLight = ambientColour * ambientLightStrength;
    float3 finalColour = sampleColour * ambientLight;
    return float4(finalColour, 1.0f);
}