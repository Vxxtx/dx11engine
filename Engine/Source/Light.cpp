#include "Light.h"

Light::Light()
{
}

Light::Light(const Light&)
{
}

Light::~Light()
{
}

void Light::SetDiffuseColor(float R, float G, float B, float A)
{
    DiffuseColor = XMFLOAT4(R, G, B, A);
}

void Light::SetDirection(float x, float y, float z)
{
    Direction = XMFLOAT3(x, y, z);
}

XMFLOAT4 Light::GetDiffuseColor()
{
    return DiffuseColor;
}

XMFLOAT3 Light::GetDirection()
{
    return Direction;
}
