#version 460 core
#extension GL_ARB_shading_language_include : require
#include "/LightData"

out vec4 FragColor;

in vec3 position;
in vec3 colour;
in vec2 texCoord;
in vec3 normal;

layout (std430, binding = 0) buffer LightBuffer
{
   int LightCount;
   int[3] _padding;
   LightData[] Lights;
};

uniform sampler2D tex0;
uniform vec3 camPosition;

const float ambientLight = 0.2f;
const float specularAmount = 0.5f;

const float falloffA = 0.7;
const float falloffB = 0;

void main()
{
   vec3 properNormal = normalize(normal);
   vec4 texColour = texture(tex0, texCoord);

   vec3 reverseViewDirection = normalize(camPosition - position);

   FragColor += texColour * ambientLight;


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
            FragColor += texColour * Lights[i].Colour * max(dot(properNormal, inverseLightRayDirection), 0.0f) * intensity;
            break;
         case LIGHT_DIRECTIONAL:
            inverseLightRayDirection = -Lights[i].Direction.xyz;
            intensity = Lights[i].Colour.w;
            FragColor += texColour * Lights[i].Colour * max(dot(properNormal, inverseLightRayDirection), 0.0f) * intensity;
            break;
         case LIGHT_SPOT:
            inverseLightRayDirection = normalize(Lights[i].Position.xyz - position);
            // dot( lightDirection, lightRayDirection ) 
            float angle = dot(Lights[i].Direction.xyz, -inverseLightRayDirection);
            intensity = clamp((angle - Lights[i].FeatherAmount) / (Lights[i].Size - Lights[i].FeatherAmount), 0.0f, 1.0f);
            FragColor += texColour * Lights[i].Colour * intensity;
            break;
      }
      
      vec3 reflectionDirection = reflect(-inverseLightRayDirection, properNormal);
      float specularValue = pow(max(dot(reverseViewDirection, reflectionDirection), 0.0f), 8) * specularAmount * intensity;
      FragColor += specularValue;
   }
}