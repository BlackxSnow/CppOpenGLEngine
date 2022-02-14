#version 460 core
out vec4 FragColour;

in vec2 uv;

uniform sampler2DArray screenTexture;

void main()
{
    float depth = texture(screenTexture, vec3(uv, 0)).r;
    FragColour = vec4(vec3(depth), 1);
}