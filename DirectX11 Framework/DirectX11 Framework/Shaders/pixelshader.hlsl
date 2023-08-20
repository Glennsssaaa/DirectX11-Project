cbuffer lightBuffer : register(b0)
{
    float3 ambientColour;
    float ambientLightStrength;
    float3 dynamicLightColour;
    float dynamicLightStrength;
    float3 dynamicLightPosition;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColour = objTexture.Sample(objSamplerState, input.inTexCoord);
    
    float3 ambientLight = ambientColour * ambientLightStrength;
    
    float3 appliedLight = ambientLight;
    
    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
    
    float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal),0);
    
    float3 diffuseLight = dynamicLightColour * dynamicLightStrength * diffuseLightIntensity;
    
    appliedLight += diffuseLight;
    
    float3 finalColour = sampleColour * saturate(appliedLight);
    
    return float4(finalColour, 1.0f);
}