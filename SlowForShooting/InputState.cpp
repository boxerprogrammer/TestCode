#include"InputState.h"
#include<DxLib.h>
#include<cassert>

InputState::InputState()
{
	//次へ
	defaultMapTable_[InputType::next] = { {InputCategory::keybd,KEY_INPUT_RETURN},
										{InputCategory::pad,PAD_INPUT_R },//スタートボタン
										{InputCategory::mouse,MOUSE_INPUT_LEFT } };
	//前へ
	defaultMapTable_[InputType::prev] = { { InputCategory::keybd,KEY_INPUT_SPACE },
										{ InputCategory::pad,PAD_INPUT_B } };
	//ポーズ
	defaultMapTable_[InputType::pause] = { { InputCategory::keybd,KEY_INPUT_P },
										{ InputCategory::pad,PAD_INPUT_L } };//セレクトボタン
	//キーコンフィグ
	defaultMapTable_[InputType::keyconf] = { { InputCategory::keybd,KEY_INPUT_K },
										{ InputCategory::pad,PAD_INPUT_Y } };//左ショルダー
	//変更
	defaultMapTable_[InputType::change] = { { InputCategory::keybd,KEY_INPUT_C },
										{ InputCategory::pad,PAD_INPUT_Z} };//右ショルダー
	//上
	defaultMapTable_[InputType::up] = { { InputCategory::keybd,KEY_INPUT_UP },
											{ InputCategory::pad,PAD_INPUT_UP} };//上
	//下
	defaultMapTable_[InputType::down] = { { InputCategory::keybd,KEY_INPUT_DOWN },
											{ InputCategory::pad,PAD_INPUT_DOWN} };//下
	//左
	defaultMapTable_[InputType::left] = { { InputCategory::keybd,KEY_INPUT_LEFT },
											{ InputCategory::pad,PAD_INPUT_LEFT} };
	//右
	defaultMapTable_[InputType::right] = { { InputCategory::keybd,KEY_INPUT_RIGHT },
											{ InputCategory::pad,PAD_INPUT_RIGHT} };

	//ショット
	defaultMapTable_[InputType::shot] = { { InputCategory::keybd,KEY_INPUT_Z },
											{ InputCategory::pad,PAD_INPUT_C} };//Xキー

	//フォースの切り離し or 吸い込み切り替え
	defaultMapTable_[InputType::switching] = { { InputCategory::keybd,KEY_INPUT_X },
											{ InputCategory::pad,PAD_INPUT_X} };//Yキー

	//スロウ
	defaultMapTable_[InputType::slow] = { { InputCategory::keybd,KEY_INPUT_X },
											{ InputCategory::pad,PAD_INPUT_A} };



	inputMapTable_ = defaultMapTable_;

	LoadKeyInfo();

	//一時マップテーブルにコピー
	tempMapTable_ = inputMapTable_;

	//入力タイプの名前のテーブルを作る
	inputNameTable_[InputType::next] = L"next";
	inputNameTable_[InputType::prev] = L"prev";
	inputNameTable_[InputType::pause] = L"pause";
	inputNameTable_[InputType::keyconf] = L"keyconf";
	inputNameTable_[InputType::change] = L"change";
	inputNameTable_[InputType::shot] = L"shot";
	inputNameTable_[InputType::switching] = L"switching";
	inputNameTable_[InputType::slow] = L"slow";
	

	currentInput_.resize(static_cast<int>(InputType::max));
	lastInput_.resize(static_cast<int>(InputType::max));
}

void
InputState::Update() {
	
	lastInput_ = currentInput_;//直前の入力情報を記憶しておく
	
	char keystate[256];
	GetHitKeyStateAll(keystate);//全キー情報取得
	int padState = GetJoypadInputState(DX_INPUT_PAD1);//パッド1コンの情報を取得する
	int mouseState = GetMouseInput();

	for (const auto& keymap : inputMapTable_) {//マップの全情報をループする
		for (const auto& input : keymap.second) {//入力情報配列をループする
			//このinputの中身は、keybd,KEY_INPUT_RETURNなどのセット(InputInfo)が入ってる
			//keymap.secondには、この入力情報セットInputInfoが入っている。
			//keymap.firstには、対応するゲーム入力名の"InputType::next"などが入っている。
			if (input.cat == InputCategory::keybd) {
				currentInput_[static_cast<int>(keymap.first)] = 
															keystate[input.id];
			}
			else if (input.cat == InputCategory::pad) {
				currentInput_[static_cast<int>(keymap.first)] = 
															padState & input.id;
			}
			else if (input.cat == InputCategory::mouse) {
				currentInput_[static_cast<int>(keymap.first)] = 
															mouseState & input.id;			
			}
			//3つの入力のうちどれかがtrueだったらもう「入力されてる」
			//とみなして、breakする。
			if (currentInput_[static_cast<int>(keymap.first)]) {
				break;
			}
		}
	}
}

void 
InputState::RewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//入力種別(割り当て先)がなければ、無効なので無視します。
	if (tempMapTable_.count(type) == 0) {
		return;
	}
	bool isRewrite = false;
	for (auto& inputInfo : tempMapTable_[type]) {
		if (inputInfo.cat == cat) {//カテゴリがヒットしたら
			inputInfo.id = id;//IDを上書きする
			isRewrite = true;
			break;
		}
	}
	if (!isRewrite) {//もしカテゴリが存在しなかったら、ここで追加しておく
		tempMapTable_[type].push_back({cat,id});
	}
}

void InputState::CommitChangedInputInfo()
{
	inputMapTable_ = tempMapTable_;
}

void InputState::RollbackChangedInputInfo()
{
	tempMapTable_ = inputMapTable_;
}

void InputState::ResetInputInfo()
{
	inputMapTable_ = defaultMapTable_;
	tempMapTable_ = defaultMapTable_;
}

void 
InputState::SaveKeyInfo()const
{
	FILE* fp = nullptr;
	
	auto err=fopen_s(&fp, "key.info","wb");
	if (fp == nullptr) {
		assert(0);
		return;
	}
	//仮想キータイプの数を書き込む
	int keytypeNum = static_cast<int>(inputMapTable_.size());
	fwrite(&keytypeNum, sizeof(keytypeNum), 1, fp);
	//仮想キータイプ(next,prevなど)のループ
	for (const auto& key : inputMapTable_) {
		int keytype = static_cast<int>(key.first);
		//仮想キー番号
		fwrite(&keytype, sizeof(keytype), 1, fp);
		int dataSize = static_cast<int>(key.second.size());
		//いくつ実入力データがあるのか
		fwrite(&dataSize, sizeof(dataSize), 1, fp);
		
		//vector型のdata()は先頭のアドレスを返す
		//一気にその入力のデータを書きこむ
		fwrite(key.second.data(), dataSize * sizeof(InputInfo), 1, fp);
	}
	fclose(fp);

}

void InputState::LoadKeyInfo() 
{
	int handle=FileRead_open(L"key.info");
	if (handle == 0) {
		return;
	}
	int keyTypeNum = 0;
	FileRead_read(&keyTypeNum, sizeof(keyTypeNum), handle);
	inputMapTable_.clear();
	for (int i = 0; i < keyTypeNum; ++i) {
		int inputType;
		FileRead_read(&inputType, sizeof(inputType), handle);

		int dataSize = 0;
		FileRead_read(&dataSize, sizeof(dataSize), handle);
		std::vector<InputInfo> inputInfoes(dataSize);
		FileRead_read(inputInfoes.data(), sizeof(InputInfo)*dataSize, handle);
		inputMapTable_[static_cast<InputType>(inputType)] = inputInfoes;
	}
	tempMapTable_ = inputMapTable_;
	FileRead_close(handle);
}

bool 
InputState::IsPressed(InputType type) const {
	return currentInput_[static_cast<int>(type)];
}

bool
InputState::IsTriggered(InputType type) const {
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}
