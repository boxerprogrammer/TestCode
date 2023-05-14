#pragma once
#include<vector>

enum class LayerType {
	Background,//0:背景
	Enemies,//1:敵
	Event//2:イベント
};

//マップの広さ＊レイヤー数なので、２つのベクターを入れ子にしてる
using MapData_t = std::vector<std::vector<unsigned char>>;
//ステージ情報を管理するクラス
class Stage
{

	 MapData_t mapData_;
	 int mapWidth_;
	 int mapHeight_;
private:
	/// <summary>
	/// データの内容をXY転置する
	/// </summary>
	/// <param name="layerId">レイヤーID</param>
	void TransposeMapData(int layerId);
public:
	void Load(const wchar_t* filePath);
	const MapData_t& GetMapData()const;
	const int GetChipId(int layerId,int chipX,int chipY)const;
	void GetMapSize(int& width, int& height);
};

