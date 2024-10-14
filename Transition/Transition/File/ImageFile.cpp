#include "ImageFile.h"
#include<DxLib.h>

ImageFile::ImageFile(FileManager& manager):File(manager)
{
}

void ImageFile::Delete()
{
	DeleteGraph(handle_);
}
