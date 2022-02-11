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

// uniform vec3 lightPos;

void main()
{
   vec3 properNormal = normalize(normal);
   vec4 texColour = texture(tex0, texCoord);

   vec3 reverseViewDirection = normalize(camPosition - position);

   FragColor += texColour * ambientLight;

   for (int i = 0; i < LightCount; i++)
   {
      vec3 inverseLightDirection;
      switch (Lights[i].Type)
      {
         case LIGHT_POINT:
            inverseLightDirection = normalize(Lights[i].Position.xyz - position);
            float dist = distance(position, vec3(Lights[i].Position));
            // FragColor += texColour * Lights[i].Colour * max(dot(properNormal, direction), 0.0f) * (1/ (1 + dist));
            FragColor += texColour * Lights[i].Colour * max(dot(properNormal, inverseLightDirection), 0.0f);
            break;
         case LIGHT_DIRECTIONAL:
            FragColor += texColour * Lights[i].Colour * max(dot(properNormal, Lights[i].Direction.xyz), 0.0f);
            break;
      }
      
      vec3 reflectionDirection = reflect(-inverseLightDirection, properNormal);
      float specularValue = pow(max(dot(reverseViewDirection, reflectionDirection), 0.0f), 8) * specularAmount;
      FragColor += specularValue;
   }
}