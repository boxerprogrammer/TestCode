#pragma once
#include"BGPart.h"

#include<vector>
class File;
class Background
{
private:
	std::vector<BGPart> bgparts_;
	float bgScale_ = 1.0f;
public:
	Background(float scale=1.0f);
	~Background();
	void AddPart(std::shared_ptr<File> file, float scale,const Vector2f& rate = {1.0f,1.0f}, int priority = -1);
	void AddPart(std::shared_ptr<File> file, const Vector2f& rate = { 1.0f,1.0f }, int priority = -1);
	void Ready();
	void Update();
	void Clear();
	void Move(const Vector2f& v);
	void Move(float x, float y);
	void Draw();
};

