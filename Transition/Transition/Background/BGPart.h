#pragma once

#include"../Geometry.h"
#include"../File/File.h"
#include<memory>
class Background;

class BGPart
{
	friend Background;
private:
	bool isRepeatable_ = true;
	Position2f pos_;
	Vector2f rate_ = { 1.0f,1.0f };
	float scale_;
	int priority_ = -1;
	int screenW_;
	Size imgSize_ = {};
	std::shared_ptr<File> file_;
public:
	BGPart(const std::shared_ptr<File> file,
									float scale,
									Position2f pos,
									Vector2f rate,
									int priority);
	~BGPart();
	bool operator<(const BGPart& val);
	void Update();
	void Move(const Vector2f& v);
	void Draw();
};

