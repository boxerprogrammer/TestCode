#pragma once
#include<tchar.h>
#include<vector>
#include<memory>
#include"../Geometry.h"
struct StageHeader {
	char id[4];
	unsigned int size;
	unsigned int mapW;
	unsigned int mapH;
	unsigned char chipW;
	unsigned char chipH;
	unsigned char layerCount;
	unsigned char bitCount;//8固定
};
class Camera;
/// <summary>
/// ステージデータ読み込み
/// およびデータの管理を行う
/// </summary>
class Stage
{
private:
	using StageLayerData_t = std::vector<unsigned char>;
	std::vector<StageLayerData_t> stagedata_;
	std::vector<Segment> terrainSegment_;
	StageHeader header_;
	std::shared_ptr<Camera> camera_;
	void CreateSegment(Position2f& lastPos, const Position2f& pos);
public:
	Stage(std::shared_ptr<Camera> c);
	void Load(const TCHAR* path);
	
	void Update();
	void BackDraw();
	void DrawChips(size_t layerNo);
	void FrontDraw();
	void DebugDraw();
	Size GetStageSize()const;
	/// <summary>
	/// 現在の座標に対応する地面のY座標を返す
	/// </summary>
	/// <param name="pos">現在の座標</param>
	/// <returns>地面のY座標</returns>
	float ComputeGroundY(const Position2f& pos)const;

	//「壁」へのめりこみベクトルを返す
	Vector2f ComputeOverlapWall(const Position2f& pos,float r)const;

	std::array<Segment,3> GetThreeSegment(const Position2f& pos)const;
	/// <summary>
	/// ボスモードか否か(スクロールが禁止される)
	/// </summary>
	/// <returns>true:ボスモード</returns>
	bool IsBossMode()const;

};

