#include "File.h"
#include"FileManager.h"

File::File(FileManager& manager) :manager_(manager) {

}
File::~File() {
	manager_.Delete(path_);
}
int 
File::GetHandle()const {
	return handle_;
}