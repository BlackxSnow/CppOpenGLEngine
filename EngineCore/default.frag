#version 460 core
#extension GL_ARB_shading_language_include : require
#include "/LightData"

out vec4 FragColor;

in vec3 position;
in vec3 normal;
in vec3 colour;
in vec2 texCoord;

layout (std430, binding = 0) buffer LightBuffer
{
   int LightCount;
   int[3] _padding;
   LightData[] Lights;
};

uniform sampler2D diffuse0;
uniform sampler2D specular0;

// If 0, use vert colours instead.
uniform uint useTextures;

uniform vec3 camPosition;

const float ambientLight = 0.2f;
const float specularAmount = 0.5f;

const float falloffA = 1.0;
const float falloffB = 0.05;

void main()
{
   vec3 properNormal = normalize(normal);

   // Set colour depending on useTextures value.
   vec4 sampleColour = texture(diffuse0, texCoord) * useTextures + vec4(colour, 1) * (1-useTextures);

   vec3 reverseViewDirection = normalize(camPosition - position);

   FragColor += sampleColour * ambientLight;


   for (int i = 0; i < LightCount; i++)
   {
      vec3 inverseLightRayDirection;
      float intensity;
      switch (Lights[i].Type)
      {
         case LIGHT_POINT:
            vec3 lightVector = Lights[i].Position.xyz - position;
            inverseLightRayDirection = normalize(lightVector);
            float dist = length(lightVector);
            intensity = Lights[i].Colour.w / (falloffA * dist * dist + falloffB * dist + 1.0f);
            FragColor += sampleColour * Lights[i].Colour * max(dot(properNormal, inverseLightRayDirection), 0.0f) * intensity;
            break;
         case LIGHT_DIRECTIONAL:
            inverseLightRayDirection = -Lights[i].Direction.xyz;
            intensity = Lights[i].Colour.w;
            FragColor += sampleColour * Lights[i].Colour * max(dot(properNormal, inverseLightRayDirection), 0.0f) * intensity;
            break;
         case LIGHT_SPOT:
            inverseLightRayDirection = normalize(Lights[i].Position.xyz - position);
            // Angle is: dot( lightDirection, lightRayDirection ) 
            float angle = dot(Lights[i].Direction.xyz, -inverseLightRayDirection);
            intensity = clamp((angle - Lights[i].FeatherAmount) / (Lights[i].Size - Lights[i].FeatherAmount), 0.0f, 1.0f);
            FragColor += sampleColour * Lights[i].Colour * intensity * max(dot(properNormal, inverseLightRayDirection), 0.0f);
            break;
      }
      
      vec3 reflectionDirection = reflect(-inverseLightRayDirection, properNormal);
      float specularValue = pow(max(dot(reverseViewDirection, reflectionDirection), 0.0f), 8) * specularAmount * intensity;
      FragColor += specularValue;
   }
}