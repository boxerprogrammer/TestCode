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
	unsigned char bitCount;//8�Œ�
};
class Camera;
/// <summary>
/// �X�e�[�W�f�[�^�ǂݍ���
/// ����уf�[�^�̊Ǘ����s��
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
	float GetGroundY(const Position2f& pos)const;
};

