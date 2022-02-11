#version 460 core
#extension GL_ARB_shading_language_include : require

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

out vec3 position;
out vec3 colour;
out vec2 texCoord;
out vec3 normal;

uniform mat4 renderMatrix;
uniform mat4 modelMatrix;

void main()
{
	colour = aColour;
	texCoord = aUV;
	normal = mat3(modelMatrix) * aNormal;
	position = (modelMatrix * vec4(aPos, 1)).xyz;
	gl_Position = renderMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}