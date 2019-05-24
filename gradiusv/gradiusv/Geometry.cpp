#include "Geometry.h"
#include<DxLib.h>


Geometry::Geometry()
{

}


Geometry::~Geometry()
{
}

Size::Size() :w(0),h(0){}
Size::Size(int inw, int inh) :w(inw),h(inh){}

Rect::Rect() : center(0,0),size(0,0){}

Rect::Rect(int x, int y, int w, int h): center(x,y),size(w,h){}

Rect::Rect(Position2& pos, Size& sz):center(pos),size(sz) {}

const int 
Rect::Left()const { 
	return center.x - size.w / 2; 
}
const int 
Rect::Top()const { 
	return center.y - size.h / 2; 
}
const int 
Rect::Right()const { 
	return center.x + size.w / 2; 
}
const int 
Rect::Bottom()const { 
	return center.y + size.h / 2; 
}

void 
Rect::Draw(unsigned int color ) {
	//DrawLine(Left(), Top(), Left(), Bottom(), color, 3);
	//DrawLine(Right(), Top(), Right(), Bottom(), color, 3);
	//DrawLine(Left(), Top(), Right(), Top(), color, 3);
	//DrawLine(Left(), Bottom(), Right(), Bottom(), color, 3);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DxLib::DrawBox(Left(), Top(), Right(), Bottom(), color, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DxLib::DrawBox(Left(), Top(), Right(), Bottom(), color, false);
}

void
Rect::Draw(const Vector2f& offset, unsigned int color) {
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DxLib::DrawBox(Left() + offset.x,
		Top() + offset.y,
		Right() + offset.x,
		Bottom() + offset.y,
		color, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DxLib::DrawBox(Left() + offset.x,
		Top() + offset.y,
		Right() + offset.x,
		Bottom() + offset.y,
		color, false);
}

void
Rect::Draw(const Vector2& offset,unsigned int color ) {
	DxLib::DrawBox(Left()+offset.x, 
		Top()+offset.y, 
		Right()+offset.x,
		Bottom()+offset.y, 
		color, false);
}


Rect 
Rect::CreateRectFromLRTB(int left, int right, int top, int bottom) {
	return Rect((left + right) / 2, (top + bottom) / 2, right - left, bottom - top);
}

Rect
Rect::CreateOverlappedRangeRect(const Rect& rcA, const Rect& rcB) {
	return CreateRectFromLRTB(max(rcA.Left(), rcB.Left()), min(rcA.Right(), rcB.Right()), max(rcA.Top(), rcB.Top()), min(rcA.Bottom(), rcB.Bottom()));
}