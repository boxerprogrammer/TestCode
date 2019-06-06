#include "Geometry.h"
#include<DxLib.h>
#include<algorithm>
void
Rect::Draw(unsigned int color) {
	DrawBox(Left(), Top(), Right(), Bottom(), color, true);
}
Vector2 operator*(const Vector2& lval, const Vector2& rval) {
	return Vector2(lval.x*rval.x, lval.y*rval.y);
}
bool 
IsCollided(const Rect& lval, const Rect& rval ,Size& overlappedSize) {
	bool ret= abs(lval.pos.x - rval.pos.x) < (lval.size.w / 2 + rval.size.w / 2) &&
		abs(lval.pos.y - rval.pos.y) < (lval.size.h / 2 + rval.size.h / 2);
	if (ret) {
		overlappedSize = Size(min(lval.Right(), rval.Right()) - max(lval.Left(), rval.Left()),
			min(lval.Bottom(), rval.Bottom()) - max(lval.Top(), rval.Top()));
	}
	else {
		overlappedSize = Size(0, 0);
	}
	return ret;
}
