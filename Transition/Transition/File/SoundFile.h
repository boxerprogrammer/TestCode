#pragma once
#include "File.h"
class SoundFile :
    public File
{
public:
    SoundFile(FileManager& manager);
    void Delete();
};

