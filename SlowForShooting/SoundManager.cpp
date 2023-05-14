#include "SoundManager.h"
#include<DxLib.h>
#include<cassert>

namespace {
	constexpr char sound_file_signature[] = "SND_";
	constexpr char sound_config_file_path[] = "./sound.conf";
	constexpr float sound_config_version = 1.0f;
}

int SoundManager::LoadSoundFile(const wchar_t* fileName)
{
	std::wstring path = L"Data/Sound/SE/";
	path += fileName;
	path += L".wav";
	int handle= LoadSoundMem(path.c_str());
	assert(handle >= 0);
	nameAndHandleTable_[fileName] = handle;
	return handle;
}

SoundManager::SoundManager() {
	LoadSoundConfig();
	LoadSoundFile(L"decide");
	LoadSoundFile(L"shot");
	LoadSoundFile(L"chargeshot");
	LoadSoundFile(L"burst");
	LoadSoundFile(L"dead");
	LoadSoundFile(L"powerup");
	SetSEVolume(m_volumeSE);
	SetBGMVolume(m_volumeBGM);
	
}

void SoundManager::LoadSoundConfig()
{
	SoundConfigInfo conf = {};
	FILE* fp = nullptr;
	fopen_s(&fp, sound_config_file_path, "rb");
	if (fp){
		fread(&conf, sizeof(conf), 1, fp);
		fclose(fp);
		m_volumeBGM = conf.volumeBGM;
		m_volumeSE = conf.volumeSE;
	}
	
}

void 
SoundManager::SetBGMRate(float rate) {
	assert(0.0f <= rate && rate <= 1.0f);
	SetVolumeMusic(static_cast<int>(static_cast<float>(m_volumeBGM)*rate));
}

void SoundManager::SaveSoundConfig()
{
	
	SoundConfigInfo conf = {};
	std::copy_n(std::begin(sound_file_signature), 
		sizeof(char)*4,
		std::begin(conf.signature));
	conf.version = sound_config_version;
	conf.volumeSE = m_volumeSE;
	conf.volumeBGM = m_volumeBGM;

	FILE* fp = nullptr;
	fopen_s(&fp, sound_config_file_path, "wb");
	if (fp) {
		fwrite(&conf, sizeof(conf), 1, fp);
		fclose(fp);
	}
}

SoundManager::~SoundManager()
{
	//SaveSoundConfig();
}

void SoundManager::Play(const wchar_t* name)
{
	PlaySoundMem(nameAndHandleTable_[name],DX_PLAYTYPE_BACK);
}

void SoundManager::PlayMusic(const wchar_t* path)
{

	DxLib::PlayMusic(path,DX_PLAYTYPE_LOOP);
	auto result = SetVolumeMusic(m_volumeBGM);
}

void SoundManager::SetSEVolume(int volume)
{
	for (auto& record : nameAndHandleTable_) {
		int result = ChangeVolumeSoundMem(volume, record.second);
		DxLib::DrawFormatString(100,100,0xffffaa,L"result=%d", result);
	}
	m_volumeSE = volume;
}

int SoundManager::GetSEVolume() const
{
	return m_volumeSE;
}

void SoundManager::SetBGMVolume(int volume)
{
	SetVolumeMusic(volume);
	m_volumeBGM = volume;
}

int SoundManager::GetBGMVolume() const
{
	return m_volumeBGM;
}

void SoundManager::StopBGMAndSE()
{
	StopMusic();

}
