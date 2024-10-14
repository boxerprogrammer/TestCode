#pragma once
#include "Scene.h"
#include"../Geometry.h"
#include<list>
#include<memory>

enum class ProgressState {
	left,
	right,
	up,
	down
};

struct Bomb {
	Position2 pos = {};
	int frame=0;
	bool isDead = false;
	Bomb(const Position2& p):pos(p){}
};
class File;
class Background;
class TitleScene :
    public Scene
{
private:
	std::shared_ptr<File> bombImg_;
	std::shared_ptr<File> bigExpImg_;
	std::shared_ptr<File> seBomb_;
	std::shared_ptr<File> titleImg_;
	std::shared_ptr<File> pressAnyImg_;
	std::shared_ptr<Background> bg_;
	

	int frame_;
	bool isBigExploding_ = false;
	int bigExplodingFrame_ = 0;
	Position2 bigExpPos_ = {};
	bool isExploding_ = false;
	Position2 upperPos_;
	Vector2 upperVec_;
	Position2 lowerPos_;
	Vector2 lowerVec_;
	std::list<Bomb> boms_;
	using UpdateFunc_t = void (TitleScene::*)(Input&);
	using DrawFunc_t = void (TitleScene::*)();

	UpdateFunc_t updateFunc_;
	DrawFunc_t drawFunc_;


	int enterFrame_ = 0;

	void FadeInUpdate(Input& );
	void FadeDraw();

	void TitleCallUpdate(Input&);
	void TitleCallDraw();

	void NormalUpdate(Input&);
	void NormalDraw();

	void DrawBg();
	void DrawBombs();
	void DrawTitle(float rate);

public:
	TitleScene(SceneManager& scene);
	virtual void Update(Input& input)override;
	virtual void Draw()override;
};

