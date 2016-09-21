#version 330 core

struct Light
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	Light Base;
	vec3 Direction;
};

struct Attenuation
{
	float constant;
	float linear;
	float exp;
};

struct PointLight
{
	Light Base;
	vec3 Position;
	Attenuation Attenuation;
};

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float Cutoff;
};

const int MAX_DIR_COUNT = 1;
const int MAX_POINT_COUNT = 8;
const int MAX_SPOT_COUNT = 8;


in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;
in vec3 TangentWorldspace;
in vec3 BitangentWorldspace;

out vec4 FragmentColor;


uniform DirectionalLight DirLights[MAX_DIR_COUNT];
uniform PointLight PointLights[MAX_POINT_COUNT];
uniform SpotLight SpotLights[MAX_SPOT_COUNT];

uniform int DirCount;
uniform int PointCount;
uniform int SpotCount;


uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform float Transparency;
uniform sampler2D Texture;
uniform sampler2D NormalmapTexture;
uniform float SpecularPower;

uniform vec3 CameraPosition;


vec4 CalculateLight(Light l, vec3 normal, vec3 dir)
{
	vec4 AmbientColor = vec4(l.Color, 1.0f) * l.AmbientIntensity;
	
	float DiffuseFactor = max(dot(normal, -dir), 0.0f);
	vec4 DiffuseColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * DiffuseFactor;
	
	vec3 FragmentToEye = normalize(CameraPosition - Position);
	vec3 LightReflect = normalize(reflect(dir, normal));
	float SpecularFactor = max(dot(FragmentToEye, LightReflect), 0.0f);
//	SpecularFactor = pow(SpecularFactor, SpecularPower);
	SpecularFactor = pow(SpecularFactor, 96);
	vec4 SpecularColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * SpecularFactor;
	
	return (AmbientColor + DiffuseColor + SpecularColor);
}


vec4 CalculatePointLight(int LightIndex, vec3 normal)
{
	vec3 Direction = Position - PointLights[LightIndex].Position;
	float Distance = length(Direction);
	float Attenuation = PointLights[LightIndex].Attenuation.constant + PointLights[LightIndex].Attenuation.linear * Distance +
						PointLights[LightIndex].Attenuation.exp * Distance * Distance;
						
	return CalculateLight(PointLights[LightIndex].Base, normal, normalize(Direction)) / Attenuation;
}


void main()
{
	//vec3 n = normalize(Normal);
	vec3 n = normalize(texture2D(NormalmapTexture, TexCoord).rgb * 2.0f - 1.0f);
	
	mat3 TBN = mat3(normalize(TangentWorldspace),
					normalize(BitangentWorldspace),
					normalize(Normal));
	
	vec3 normal = normalize(TBN * n);
	
	vec4 LightsColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < DirCount; ++i)
	{
		LightsColor += CalculateLight(DirLights[i].Base, normal, DirLights[i].Direction);
	}
	
	for (int i = 0; i < PointCount; ++i)
	{
		LightsColor += CalculatePointLight(i, normal);
	}

	//vec4 amb = AmbientColor * matAmbient;
	//vec4 diff = DiffuseColor * matDiffuse;
	//vec4 spec = SpecularColor * matSpecular;
	
	//FragmentColor = (AmbientColor + DiffuseColor + SpecularColor);
	//FragmentColor = texture2D(Texture, TexCoord) * (amb + diff + spec);
	FragmentColor = texture2D(Texture, TexCoord) * LightsColor;
	
	FragmentColor.a = 1 - Transparency;
}
