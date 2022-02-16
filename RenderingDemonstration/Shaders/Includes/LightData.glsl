// Light types
const uint LIGHT_POINT = 0;
const uint LIGHT_DIRECTIONAL = 1;
const uint LIGHT_SPOT = 2;

struct LightData 
{
    uint Type;
    float Size;
    float FeatherAmount;
    int ShadowMapIndex;
    vec4 Position;
    vec4 Direction;
    vec4 Colour;
};