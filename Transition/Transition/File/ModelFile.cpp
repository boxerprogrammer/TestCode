#include "ModelFile.h"
#include<DxLib.h>

ModelFile::ModelFile(FileManager& manager):File(manager)
{
}

void ModelFile::Delete()
{
	MV1DeleteModel(handle_);
}
