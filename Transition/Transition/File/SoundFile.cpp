#include "SoundFile.h"
#include<DxLib.h>

SoundFile::SoundFile(FileManager& manager):File(manager)
{
}

void SoundFile::Delete()
{
	DeleteSoundMem(handle_);
}
