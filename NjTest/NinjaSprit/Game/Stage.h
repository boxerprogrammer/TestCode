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
class GameplayingScene;
/// <summary>
/// �X�e�[�W�f�[�^�ǂݍ���
/// ����уf�[�^�̊Ǘ����s��
/// </summary>
class Stage
{
private:
	int stageAtlasH_ = -1;
	using StageLayerData_t = std::vector<unsigned char>;
	std::vector<StageLayerData_t> stagedata_;
	std::vector<Segment> terrainSegment_;
	StageHeader header_;
	std::shared_ptr<Camera> camera_;
	GameplayingScene* gameScene_;
	void CreateSegment(Position2f& lastPos, const Position2f& pos);

	using Updater_t = void (Stage::*)();
	Updater_t updater_;
	void NormalUpdate();
	void BossUpdate();
	bool isBossMode_=false;
	void CheckBossMode();

public:
	/// <param name="camera">�J�����|�C���^</param>
	Stage(std::shared_ptr<Camera> camera, GameplayingScene* gs);

	/// <summary>
	/// �X�e�[�W�̃��[�h
	/// </summary>
	/// <param name="path">���x��(fmf)�t�@�C���p�X</param>
	void Load(const TCHAR* path);
	
	/// <summary>
	/// �X�e�[�W�X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �w�i�\��
	/// </summary>
	void BackDraw();

	/// <summary>
	/// �O�i�\��
	/// </summary>
	void FrontDraw();

	/// <summary>
	/// �����蔻��Ȃǂ̃f�o�b�O�\��
	/// </summary>
	void DebugDraw();


	/// <summary>
	/// �w�背�C���[�ԍ��̉�ʔ͈͓��̃}�b�v�`�b�v�����ׂĕ\��
	/// </summary>
	/// <param name="layerNo">���C���[�ԍ�</param>
	void DrawChips(size_t layerNo);

	/// <summary>
	/// ���݂̃X�e�[�W�̕��ƍ�����Ԃ�
	/// </summary>
	/// <returns>���݂̃X�e�[�W�T�C�Y</returns>
	Size GetStageSize()const;

	/// <summary>
	/// ���݂̍��W�ɑΉ�����n�ʂ�Y���W��Ԃ�
	/// </summary>
	/// <param name="pos">���݂̍��W</param>
	/// <returns>�n�ʂ�Y���W</returns>
	float ComputeGroundY(const Position2f& pos)const;

	//�u�ǁv�ւ̂߂肱�݃x�N�g����Ԃ�
	Vector2f ComputeOverlapWall(const Position2f& pos,float r)const;

	/// <summary>
	/// ���݂̍��W�ɑΉ���������ƑO��̐�����Ԃ�
	/// </summary>
	/// <param name="pos">�T�����߂̑Ώۍ��W</param>
	/// <returns></returns>
	std::array<Segment,3> GetThreeSegment(const Position2f& pos)const;
	
	/// <summary>
	/// �{�X���[�h���ۂ�(�X�N���[�����֎~�����)
	/// </summary>
	/// <returns>true:�{�X���[�h</returns>
	bool IsBossMode()const;

};

