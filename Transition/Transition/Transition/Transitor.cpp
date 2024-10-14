#include "Transitor.h"
#include"../Application.h"
#include<DxLib.h>



void Transitor::Start()
{
	const auto& size = Application::GetInstance().GetWindowSize();

	oldRT_ = MakeGraph(size.w, size.h);
	newRT_ = MakeScreen(size.w, size.h);

	int result = GetDrawScreenGraph(0, 0, size.w, size.h, oldRT_, true);
	frame_ = 0;
}

Transitor::~Transitor()
{
	DeleteGraph(oldRT_);
	DeleteGraph(newRT_);
}


bool 
Transitor::IsEnd() const
{
	return frame_ >= interval_;
}