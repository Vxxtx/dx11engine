#include "Element.h"

Element::Element()
{
}

Element::~Element()
{
}

bool Element::Init()
{
    return true;
}

void Element::Shutdown()
{
}

bool Element::Frame()
{
    return true;
}

DirectX::XMFLOAT3 Element::GetForward()
{
    DirectX::XMFLOAT3 Forward
    { 
        cosf(Rotation.x * (DirectX::XM_PI / 180)) * sinf(Rotation.y * (DirectX::XM_PI / 180)),
        -sinf(Rotation.x * (DirectX::XM_PI / 180)),
        cosf(Rotation.x * (DirectX::XM_PI / 180)) * cosf(Rotation.y * (DirectX::XM_PI / 180))
    };

    return Forward;
}

DirectX::XMFLOAT3 Element::GetRight()
{
    DirectX::XMFLOAT3 Right
    { 
        cosf(Rotation.y * (DirectX::XM_PI / 180)),
        0.f, 
        -sinf(Rotation.y * (DirectX::XM_PI / 180))
    };

    return Right;
}

DirectX::XMFLOAT3 Element::GetUp()
{
    DirectX::XMFLOAT3 ForwardFloat = GetForward();
    DirectX::XMFLOAT3 RightFloat = GetRight();

    DirectX::XMVECTOR Forward, Right;
    
    Forward = XMLoadFloat3(&ForwardFloat);
    Right = XMLoadFloat3(&RightFloat);

    DirectX::XMVECTOR UpVec = DirectX::XMVector3Cross(Forward, Right);
    DirectX::XMFLOAT3 Up;

    DirectX::XMStoreFloat3(&Up, UpVec);

    return Up;
}
