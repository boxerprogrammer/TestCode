#pragma once
#include "File.h"
class ModelFile :
    public File
{
public:
    ModelFile(FileManager& manager);
    void Delete();
};

