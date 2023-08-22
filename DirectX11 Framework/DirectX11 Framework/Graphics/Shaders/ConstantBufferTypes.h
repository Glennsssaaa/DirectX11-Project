#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader {
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
};

struct CB_VS_vertexshader_2d {
	DirectX::XMMATRIX wvpMatrix;
};

struct CB_PS_light {
	DirectX::XMFLOAT3 ambientLightColour;
	float ambientLightStrength;
	
	DirectX::XMFLOAT3 dynamicLightColour;
	float dynamicLightStrength;
	DirectX::XMFLOAT3 dynamicLightPosition;
	
	float dynamicLightAttenuation_a;
	float dynamicLightAttenuation_b;
	float dynamicLightAttenuation_c;
};

struct CB_PS_colourshader {
	DirectX::XMFLOAT4 colour;
};

struct CB_PS_boxblur {
	int grayScaleToggle;
	DirectX::XMFLOAT3 padding;
};

struct CB_VS_boxblur {
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};