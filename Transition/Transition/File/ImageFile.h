#pragma once
#include "File.h"
class ImageFile :
    public File
{
public:
    ImageFile(FileManager& manager);
    void Delete();
};

