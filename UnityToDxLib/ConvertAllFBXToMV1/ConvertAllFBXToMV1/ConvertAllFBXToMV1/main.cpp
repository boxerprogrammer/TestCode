#include<DxLib.h>
#include<string>
#include<cassert>
#include<map>


std::wstring
GetWideStringFromString(const std::string& str)
{
	// MultiByteToWideChar関数は、マルチバイト文字列を
	// ワイド文字列に変換しますが、2回呼び出す必要がある関数です
	// １回目の呼び出しで、ワイド文字列のサイズを取得します
	// 1回目はサイズがわからないため、受け取るための引数にnullptrを
	// 指定して、文字列数を得る
	// 文字列数がわかったため、ワイド文字のサイズを確保しておく
	std::wstring wstr;//ワイド文字
	auto count = MultiByteToWideChar(CP_ACP, MB_COMPOSITE | MB_ERR_INVALID_CHARS,
		str.c_str(),
		str.length(),
		nullptr,// ここにnullptrを指定すると文字列数取得が目的で
		// あることを示す。
		0);     //nullptrなので0にしておく
	wstr.resize(count);//ワイド文字列に必要な文字列数を確保
	// 2回目の呼び出しでは実際に文字列を代入します
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE | MB_ERR_INVALID_CHARS,
		str.c_str(),
		str.length(),
		wstr.data(),//代入したい文字列のアドレス
		wstr.size());//代入したい文字列のサイズ
	return wstr;
}

std::string GetStringFromWideString(const std::wstring wstr)
{
	std::string str;
	int count = WideCharToMultiByte(CP_ACP, 0,
		wstr.data(), wstr.length(),
		nullptr, 0,
		nullptr, nullptr);
	assert(count > 0);
	str.resize(count);
	WideCharToMultiByte(CP_ACP, 0,
		wstr.data(), wstr.length(),
		str.data(), str.size(),
		nullptr, nullptr);

	return str;
}

int main() {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	FILEINFO info;
	int y = 0;

	int dataHandle = FileRead_open(L"texturedata.dat");
	if (dataHandle < 0) {
		return 0;
	}
	std::map<std::string, std::string> materialMap;
	int dataSize = 0;
	FileRead_read(&dataSize, sizeof(dataSize), dataHandle);
	for (int i = 0; i < dataSize; ++i) {
		byte namelen = 0;
		std::string name;
		FileRead_read(&namelen, sizeof(namelen), dataHandle);
		name.resize(namelen);
		FileRead_read(name.data(), name.size(), dataHandle);
		
		std::string texFileName;
		FileRead_read(&namelen, sizeof(namelen), dataHandle);
		texFileName.resize(namelen);
		FileRead_read(texFileName.data(), texFileName.size(), dataHandle);
		materialMap[name] = texFileName;
	}
	FileRead_close(dataHandle);
	std::wstring path = L"./";
	auto handle = FileRead_findFirst((path + L"*.mv1").c_str(), &info);
	if (handle != (DWORD_PTR)(-1)) {
		int y = 10;
		do {
			auto modelHandle = MV1LoadModel((path + info.Name).c_str());
			std::wstring modelWName = info.Name;
			auto modelName = GetStringFromWideString(modelWName);
			int n = modelName.rfind(".mv1");
			modelName = modelName.substr(0, n);
			if (materialMap.contains(modelName)) {
				auto texName = materialMap[modelName];
				auto texHandle = LoadGraph((path + GetWideStringFromString(texName)).c_str());
				
				assert(texHandle >= 0);
				auto wtexFilePath = path + GetWideStringFromString(texName);
				auto texFilePath = wtexFilePath.c_str();
				DrawFormatString(10, y, 0xffffff, L"%s", texFilePath);
				MV1AddTexture(modelHandle,GetWideStringFromString(texName).c_str(),texFilePath);
				MV1SetTextureGraphHandle(modelHandle,0,texHandle,false);
				MV1SetTextureColorFilePath(modelHandle, 0, texFilePath);
				MV1SetMaterialDifMapTexture(modelHandle, 0, texHandle);						// 指定のマテリアルでディフューズマップとして使用するテクスチャを指定する
				MV1SaveModelToMV1File(modelHandle, (path + info.Name).c_str());
				MV1DeleteModel(modelHandle);
				DeleteGraph(texHandle);
			}
			else {
				MV1DeleteModel(modelHandle);
			}
			y += 30;
		} while (FileRead_findNext(handle, &info)>=0);
	}
	WaitKey();
	DxLib_End();
}