#pragma once
#include<string>
#include<vector>
class FileManager;
/// <summary>
/// ファイルそのものを表すクラス
/// </summary>
/// <note>
/// 画像とサウンドファイルに関しては実体はここにはない
/// しかし、データ系の場合は実体をFileがもつこととする
/// </note>
class File
{
	friend FileManager;
public:
	/// <summary>
	/// ファイルの種別
	/// </summary>
	enum class Type {
		image,//画像ファイル
		mask,//マスク画像ファイル
		sound,//サウンドファイル(BGMを除く)
		data//それ以外のデータ系
	};
private:
	
	std::string tag_;//タグ
	Type type_;//ファイル種別
	int handle_;//ファイルハンドル(画像、サウンドの時に使用)
	std::vector<uint8_t>::iterator cursor_;//データ読み込みの際の疑似カーソル
	std::vector<uint8_t> data_;//データ型の時にのみ使用
	File(int h,Type type,const char* tag="");
	void FCopyRead(void* data, size_t size);
public:
	~File();
	int Handle()const;
	std::vector<uint8_t> Data();
	bool IsLoaded()const;
	const std::string& GetTag()const;
	template<typename T>
	void CopyRead(T* data,size_t num) {
		FCopyRead(data,sizeof(T) * num);
	}
	template<typename T>
	void CopyRead(T& data) {
		FCopyRead(&data, sizeof(T));
	}
};

