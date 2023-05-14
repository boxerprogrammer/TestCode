#pragma once

#include<unordered_map>
#include<string>

/// <summary>
/// SEの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
private:
	//変更したサウンド情報をファイルに書き込む
	struct SoundConfigInfo {
		char signature[4];//"SND_"
		float version;//1.0
		unsigned short volumeSE;//0～255
		unsigned short volumeBGM;//0～255
	};

	int m_volumeSE=255;
	int m_volumeBGM=255;

	std::unordered_map<std::wstring, int> nameAndHandleTable_;
	int LoadSoundFile(const wchar_t* fileName);
	//シングルトンパターンなのでコンストラクタはprivateに置く
	SoundManager();
	//コピーも代入も禁止する
	SoundManager(const SoundManager&)=delete;//コピーコンストラクタ禁止
	void operator=(const SoundManager&) = delete;//代入も禁止


	void LoadSoundConfig();
	

public:
	~SoundManager();
	/// <summary>
	/// SoundManager使用者はGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance() {
		static SoundManager instance;//ここが唯一の実体
		return instance;//それの参照を返す
	}

	/// <summary>
	/// 指定のサウンドを鳴らす
	/// </summary>
	/// <param name="name">サウンド名</param>
	void Play(const wchar_t* name);
	void PlayMusic(const wchar_t* path);

	/// <summary>
	/// SEのボリュームを設定する
	/// </summary>
	/// <param name="volume"></param>
	void SetSEVolume(int volume);
	int GetSEVolume()const;

	void SetBGMVolume(int volume);
	
	/// <summary>
	/// BGMの現在の鳴らす音量の割合
	/// </summary>
	/// <param name="rate>音量の割合(一時的なもの0.0～1.0)</param>
	void SetBGMRate(float rate);
	int GetBGMVolume()const;

	///サウンドを全部一時停止する
	void StopBGMAndSE();

	//サウンド情報のセーブ
	void SaveSoundConfig();

};

