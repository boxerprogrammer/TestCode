#pragma once
#include<vector>
class Background
{
private:
	std::vector<int> bgH_;
	int frame_=0;
public:
	Background();
	~Background();
	void Update();
	void Draw();
};

