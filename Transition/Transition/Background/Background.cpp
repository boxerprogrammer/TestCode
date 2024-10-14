#include "Background.h"
#include<DxLib.h>
#include<algorithm>
#include"../Application.h"
Background::Background(float scale):bgScale_(scale)
{

}
Background::~Background()
{
}
void Background::AddPart(std::shared_ptr<File> file, float scale, const Vector2f& rate, int priority)
{
	const auto& size = Application::GetInstance().GetWindowSize();
	bgparts_.emplace_back(file,scale, Position2f(0, 0), rate, priority);
	bgparts_.back().screenW_ = size.w;
}

void Background::AddPart(std::shared_ptr<File> file, const Vector2f& rate, int priority)
{
	const auto& size = Application::GetInstance().GetWindowSize();
	bgparts_.emplace_back(file, bgScale_, Position2f(0, 0), rate, priority);
	bgparts_.back().screenW_ = size.w;
}

void Background::Ready()
{
	std::sort(bgparts_.begin(), bgparts_.end());
}

void Background::Update()
{
	for (BGPart& part : bgparts_) {
		part.Update();
	}
}

void Background::Clear()
{
	bgparts_.clear();
}

void Background::Move(const Vector2f& v)
{
	for (BGPart& part : bgparts_) {
		part.Move(v);
	}
}

void Background::Move(float x, float y)
{
	Move({ x,y });
}

void Background::Draw()
{
	for (BGPart& part : bgparts_) {
		part.Draw();
	}
}
