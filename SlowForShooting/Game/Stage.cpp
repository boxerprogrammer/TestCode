#include "Stage.h"
#include<DxLib.h>
#include<string>
#include<cassert>

void 
Stage::TransposeMapData(int layerId)
{
	auto temp = mapData_[layerId];//いったんコピーしておく
	//理由としてはコピーせずに転置しようとすると元のデータが消える
	for (int Y = 0; Y < mapHeight_; ++Y) {
		for (int X = 0; X < mapWidth_; ++X) {
			//通常のXY指定の場合
			//Y*mapWidth_+Xという風になる。
			//さらに、横200縦15は維持する必要がある
			//縦に並べていきたい
			//0の隣は1段下にしたい
			int idxSrc = Y * mapWidth_ + X;//Source Index
			int idxDst = Y + mapHeight_ * X;//Destination Index
			mapData_[layerId][idxDst] = temp[idxSrc];
		}
	}

}

void Stage::Load(const wchar_t* filePath)
{
	//FMFヘッダー(Platinumのドキュメントに書いてある)
	struct Header {
		int8_t id[4];//識別子(FMF_) 1*4バイト
		uint32_t size;//データサイズ 4バイト
		uint32_t mapWidth;//マップの幅 4バイト
		uint32_t mapHeight;//マップの高さ 4バイト
		uint8_t chipWidth;//チップ(セル１個)の幅 1バイト
		uint8_t chipHeight;//チップ(セル１個)の高さ １バイト
		uint8_t layerCount;//レイヤー数 1バイト
		uint8_t bitCount;//１セルあたりのビット数(÷8でバイト数) 1バイト
	};//20バイト

	Header header;
	int handle = FileRead_open(filePath);
	FileRead_read(&header, sizeof(header), handle);
	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());
	if (strId != "FMF_") {
		//ロード失敗
		assert(0);
	}

	mapWidth_ = header.mapWidth;
	mapHeight_ = header.mapHeight;
	int layerCount = header.layerCount;
	//レイヤー１個あたりのサイズを計算する
	//マップの幅*マップの高さ*(チップ１個あたりのバイト数
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	mapData_.resize(layerCount);
	for (auto& layer : mapData_) {
		layer.resize(layerDataSize);
		FileRead_read(layer.data(), layerDataSize, handle);
	}
	FileRead_close(handle);

	//実行時メモリ効率化のために、データを加工
	for (int layerIdx = 0; layerIdx < layerCount; ++layerIdx) {
		TransposeMapData(layerIdx);
	}

}

const MapData_t& Stage::GetMapData() const
{
	return mapData_;
}

const int Stage::GetChipId(int layerId, int chipX, int chipY) const
{
	assert(chipX < mapWidth_);
	assert(chipY < mapHeight_);
	auto index = chipY + chipX*mapHeight_;
	return mapData_[layerId][index];
}

void Stage::GetMapSize(int& width, int& height)
{
	width = mapWidth_;
	height = mapHeight_;
}
