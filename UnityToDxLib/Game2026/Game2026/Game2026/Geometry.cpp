#include "Geometry.h"
#include<cmath>
#include<cassert>
#include<DxLib.h>

float Vector2::Length() const
{
    return std::hypot(x,y);
}

void Vector2::Normalize()
{
    auto len = Length();
    if (len == 0.0f) {
        return;
    }
    x /= len;
    y /= len;
}

Vector2 Vector2::Normalied() const
{
    auto len = Length();
    if (len == 0.0f) {
        return {};
    }
    return { x / len,y / len };
}

Vector2 Vector2::operator*(float scale) const
{
    return { x * scale,y * scale };
}

Vector2 Vector2::operator/(float div) const
{
    assert(div != 0.0f);
    return {x/div,y/div};
}

void Vector2::operator*=(float scale) 
{
    x *= scale;
    y *= scale;
}

Vector2 Vector2::operator-() const
{
    return { -x,-y };
}

Vector2 Vector2::operator+(const Vector2& val) const
{
    return { x + val.x,y + val.y };
}

Vector2 Vector2::operator-(const Vector2& val) const
{
    return { x - val.x,y - val.y };
}

void Vector2::operator+=(const Vector2& val)
{
    x += val.x;
    y += val.y;
}

void Vector2::operator-=(const Vector2& val)
{
    x -= val.x;
    y -= val.y;
}

Vector3 Vector3::operator+(const Vector3& v)const
{
    return { x + v.x,y + v.y,z + v.z };
}

Vector3 Vector3::operator-(const Vector3& v)const
{
    return { x - v.x,y - v.y,z - v.z };
}

VECTOR Vector3::GetVECTOR() const
{
    return VGet(x, y, z);
}

Cube::Cube(const Position3& center, float w):center_(center)
{
    //左手前上
    positions[0].x = center.x - w;
    positions[0].y = center.y + w;
    positions[0].z = center.z - w;

    //右手前上
    positions[1].x = center.x + w;
    positions[1].y = center.y + w;
    positions[1].z = center.z - w;

    //左手前下
    positions[2].x = center.x - w;
    positions[2].y = center.y - w;
    positions[2].z = center.z - w;

    //右手前下
    positions[3].x = center.x + w;
    positions[3].y = center.y - w;
    positions[3].z = center.z - w;

    //左奥上
    positions[4].x = center.x - w;
    positions[4].y = center.y + w;
    positions[4].z = center.z + w;

    //右奥
    positions[5].x = center.x + w;
    positions[5].y = center.y + w;
    positions[5].z = center.z + w;

    //左奥下
    positions[6].x = center.x - w;
    positions[6].y = center.y - w;
    positions[6].z = center.z + w;

    //右奥下
    positions[7].x = center.x + w;
    positions[7].y = center.y - w;
    positions[7].z = center.z + w;
}

void Cube::Draw(MATRIX mat,uint32_t color) {
    std::array<VERTEX3D, 8> vertices;
    for (int i = 0; i < vertices.size(); ++i) {
        auto norm = (positions[i] - center_).GetVECTOR();
        norm = VTransformSR(norm, mat);
        vertices[i].pos = VTransform(positions[i].GetVECTOR(),mat);
        vertices[i].dif = GetColorU8(255, 255, 255, 255);
        vertices[i].spc = GetColorU8(255, 255, 255, 255);
        vertices[i].u = vertices[i].v = 0.0f;
        vertices[i].su = vertices[i].sv = 0.0f;
        vertices[i].norm = norm;
    }
    //ワイヤーフレーム用
    //std::array<uint16_t, 24> indices = {
    //    0,1, 0,2, 1,3, 2,3, //手前
    //    4,5, 4,6, 5,7, 6,7, //奥
    //    0,4, 2,6, 1,5, 3,7 //左
    //};
    std::array<uint16_t, 36> indices = {
        0,1,3, 0,3,2, 1,5,7, 1,7,3, 
        5,4,6, 5,6,7, 0,4,2, 4,2,6,
        4,5,1, 4,1,0, 2,3,7, 2,7,6
    };
    DrawPrimitiveIndexed3D(vertices.data(), vertices.size(),
        indices.data(),indices.size(),
        DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, true);
}

float DegreeToRadian(float degree)
{
    return (degree * DX_PI_F) / 180.0f;
}
