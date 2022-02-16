#version 460 core
#extension GL_ARB_shading_language_include : require


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColour;
layout (location = 3) in vec2 aUV;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec3 Colour;
	vec2 TexCoord;
} vso;

uniform mat4 renderMatrix;
uniform mat4 modelMatrix;

void main()
{
	vso.Colour = aColour;
	vso.TexCoord = aUV;
	vso.Normal = mat3(modelMatrix) * aNormal;
	vso.FragPos = (modelMatrix * vec4(aPos, 1)).xyz;
	gl_Position = renderMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}