#include "Geometry.h"
#include<cmath>
#include<DxLib.h>

void Vector2::operator+=(const Vector2& rval)
{
	x += rval.x;
	y += rval.y;
}

void Vector2::operator-=(const Vector2& rval)
{
	x -= rval.x;
	y -= rval.y;
}

Vector2 
Vector2::operator*(float scale) const
{
	return {x*scale,y*scale};
}

Vector2 Vector2::operator/(float div) const
{
	return {x/div,y/div};
}

void 
Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

void 
Vector2::operator/=(float div)
{
	x /= div;
	y /= div;
}

Vector2 
Vector2::operator-()const
{
	return { -x,-y };
}

float Vector2::Length() const
{
	return hypotf(x, y);
	//return sqrtf(x * x + y * y);
}

float Vector2::SQLength() const
{
	return x * x + y * y;
}

void Vector2::Normalize()
{
	auto len = Length();
	x /= len;
	y /= len;
}

Vector2 Vector2::GetNormalized() const
{
	auto len = Length();
	return { x / len,y / len };
}

Rect::Rect() :Rect({}, {})
{
}

Rect::Rect(const Position2& c, const Size& sz) :center(c),size(sz)
{
}

void Rect::SetLTRB(int left, int top, int right, int bottom)
{
	center.x = static_cast<float>(left + right) / 2.0f;
	center.y = static_cast<float>(top + bottom) * 0.5f;
}

int Rect::Left() const
{
	return static_cast<int>(center.x)-size.w/2;
}

int Rect::Top() const
{
	return static_cast<int>(center.y) - size.h / 2;
}

int Rect::Right() const
{
	return static_cast<int>(center.x) + size.w / 2;
}

int Rect::Bottom() const
{
	return static_cast<int>(center.y) + size.h / 2;
}

const Position2& Rect::GetCenter() const
{
	return center;
}

const Size& Rect::GetSize() const
{
	return size;
}

void Rect::Draw(unsigned int color) const
{
	//’e‚Ì‹éŒ`‚ð•\Ž¦‚·‚é
	DrawBox(
		Left(),//¶
		Top(),//ã
		Right(),//‰E
		Bottom(),//‰º
		color, false);

}

bool 
Rect::IsHit(const Rect& rc) const
{
	////“–‚½‚Á‚Ä‚È‚¢ðŒ(‰¡)
	//if (fabsf(center.x - rc.center.x) > static_cast<float>(size.w + rc.size.w) / 2) {
	//	return false;
	//}
	////“–‚½‚Á‚Ä‚È‚¢ðŒ(c)
	//if (fabsf(center.y - rc.center.y) > static_cast<float>(size.h + rc.size.h) / 2) {
	//	return false;
	//}
	//return true;

	return !(
		(fabsf(center.x - rc.center.x) > static_cast<float>(size.w + rc.size.w) / 2) ||
		(fabsf(center.y - rc.center.y) > static_cast<float>(size.h + rc.size.h) / 2)
		);
}
