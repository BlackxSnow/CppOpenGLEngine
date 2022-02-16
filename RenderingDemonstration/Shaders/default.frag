#version 460 core
#extension GL_ARB_shading_language_include : require
#include "/LightData.glsl"

#define MAX_SHADOW_CASTERS 64

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec3 Colour;
	vec2 TexCoord;
} fsi;

// Lighting data
layout (std430, binding = 0) buffer LightBuffer
{
   int LightCount;
   int[3] _padding;
   LightData[] Lights;
};

uniform mat4[MAX_SHADOW_CASTERS] lightMatrices;
// Light shadow depth maps
uniform sampler2DArray LightDepthMaps;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

// If 0, use vert colours instead.
uniform uint useTextures;

uniform vec3 camPosition;

const float ambientLight = 0.2f;
const float specularAmount = 0.5f;

const float falloffA = 1.0;
const float falloffB = 0.05;

float CalculateShadow(int shadowMapIndex, vec3 inverseLightRayDirection)
{
   if (shadowMapIndex == -1)
   {
      return 1.0;
   }
   vec4 lightSpaceCoords = lightMatrices[shadowMapIndex] * vec4(fsi.FragPos, 1);
   vec3 normalDeviceCoords = lightSpaceCoords.xyz / lightSpaceCoords.w;
   // normalDeviceCoords.y *= -1;
   normalDeviceCoords = normalDeviceCoords * 0.5 + 0.5;
   
   float shadowDepth = texture(LightDepthMaps, vec3(normalDeviceCoords.xy, shadowMapIndex)).r;
   float fragDepth = normalDeviceCoords.z;

   // const float bias = 0.001;
   // float bias = max(0.0015 * (1.0 - dot(fsi.Normal, inverseLightRayDirection)), 0.001);
   float shadow = fragDepth /*- bias*/ < shadowDepth ? 1.0 : 0.0;
   return shadow;
}

void main()
{
   vec3 properNormal = normalize(fsi.Normal);

   // Set colour depending on useTextures value.
   vec4 sampleColour = texture(diffuse0, fsi.TexCoord) * useTextures + vec4(fsi.Colour, 1) * (1-useTextures);

   vec3 reverseViewDirection = normalize(camPosition - fsi.FragPos);

   FragColor += sampleColour * ambientLight;


   for (int i = 0; i < LightCount; i++)
   {
      vec3 inverseLightRayDirection;
      float intensity;
      float shadow = 1;
      switch (Lights[i].Type)
      {
         case LIGHT_POINT:
            vec3 lightVector = Lights[i].Position.xyz - fsi.FragPos;
            inverseLightRayDirection = normalize(lightVector);
            shadow = CalculateShadow(Lights[i].ShadowMapIndex, inverseLightRayDirection);
            float dist = length(lightVector);
            intensity = Lights[i].Colour.w / (falloffA * dist * dist + falloffB * dist + 1.0f) * shadow;
            FragColor += sampleColour * Lights[i].Colour * max(dot(properNormal, inverseLightRayDirection), 0.0f) * intensity;
            break;
         case LIGHT_DIRECTIONAL:
            inverseLightRayDirection = -Lights[i].Direction.xyz;
            shadow = CalculateShadow(Lights[i].ShadowMapIndex, inverseLightRayDirection);
            intensity = Lights[i].Colour.w * shadow;
            FragColor += sampleColour * Lights[i].Colour * max(dot(properNormal, inverseLightRayDirection), 0.0f) * intensity;
            break;
         case LIGHT_SPOT:
            inverseLightRayDirection = normalize(Lights[i].Position.xyz - fsi.FragPos);
            shadow = CalculateShadow(Lights[i].ShadowMapIndex, inverseLightRayDirection);
            // Angle is: dot( lightDirection, lightRayDirection ) 
            float angle = dot(Lights[i].Direction.xyz, -inverseLightRayDirection);
            intensity = clamp((angle - Lights[i].FeatherAmount) / (Lights[i].Size - Lights[i].FeatherAmount), 0.0f, 1.0f) * shadow;
            FragColor += sampleColour * Lights[i].Colour * intensity * max(dot(properNormal, inverseLightRayDirection), 0.0f);
            break;
      }
      
      vec3 reflectionDirection = reflect(-inverseLightRayDirection, properNormal);
      float specularValue = pow(max(dot(reverseViewDirection, reflectionDirection), 0.0f), 8) * specularAmount * intensity;
      FragColor += specularValue;
   }
}