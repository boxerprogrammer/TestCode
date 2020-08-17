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
class GameplayingScene;
/// <summary>
/// ステージデータ読み込み
/// およびデータの管理を行う
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
	/// <param name="camera">カメラポインタ</param>
	Stage(std::shared_ptr<Camera> camera, GameplayingScene* gs);

	/// <summary>
	/// ステージのロード
	/// </summary>
	/// <param name="path">レベル(fmf)ファイルパス</param>
	void Load(const TCHAR* path);
	
	/// <summary>
	/// ステージ更新
	/// </summary>
	void Update();

	/// <summary>
	/// 背景表示
	/// </summary>
	void BackDraw();

	/// <summary>
	/// 前景表示
	/// </summary>
	void FrontDraw();

	/// <summary>
	/// 当たり判定などのデバッグ表示
	/// </summary>
	void DebugDraw();


	/// <summary>
	/// 指定レイヤー番号の画面範囲内のマップチップをすべて表示
	/// </summary>
	/// <param name="layerNo">レイヤー番号</param>
	void DrawChips(size_t layerNo);

	/// <summary>
	/// 現在のステージの幅と高さを返す
	/// </summary>
	/// <returns>現在のステージサイズ</returns>
	Size GetStageSize()const;

	/// <summary>
	/// 現在の座標に対応する地面のY座標を返す
	/// </summary>
	/// <param name="pos">現在の座標</param>
	/// <returns>地面のY座標</returns>
	float ComputeGroundY(const Position2f& pos)const;

	//「壁」へのめりこみベクトルを返す
	Vector2f ComputeOverlapWall(const Position2f& pos,float r)const;

	/// <summary>
	/// 現在の座標に対応する線分と前後の線分を返す
	/// </summary>
	/// <param name="pos">探すための対象座標</param>
	/// <returns></returns>
	std::array<Segment,3> GetThreeSegment(const Position2f& pos)const;
	
	/// <summary>
	/// ボスモードか否か(スクロールが禁止される)
	/// </summary>
	/// <returns>true:ボスモード</returns>
	bool IsBossMode()const;

};

