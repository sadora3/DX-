#include "Tap.h"
#include"DxLib.h"

//初期化
void cTap::Init() {
	for (int i = 0; i < MAX_TAP_NUM; i++) {
		Count[i] = 0;	//タップカウントの初期化
		Id[i] = -1;		//IDの初期化
	}
}

//更新
void cTap::Update() {
	//IDのリセット
	for (int i = 0; i < MAX_TAP_NUM; i++) {
		Id[i] = -1;
	}

	//タップの数だけループ
	TouchNum = GetTouchInputNum();
	int TempPosX, TempId;
	for (int TapNum = 0; TapNum < TouchNum; TapNum++) {
		// タッチされている箇所の座標を取得
		GetTouchInput(TapNum, &TempPosX, NULL, &TempId, NULL);

		//座標を代入
		PosX[TempId] = TempPosX;
		//IDを代入
		Id[TempId] = TempId;
		//タッチされているフレーム数をカウント
		Count[TempId]++;
	}
	//タップされていないタップカウントは初期化
	for (int TapNum = 0; TapNum < MAX_TAP_NUM; TapNum++) {
		//タップされていたら、次へ
		if (Id[TapNum] != -1) { continue; }

		Count[TapNum] = 0;
		PosX[TapNum] = 0;
	}
}

//最大タップ数のゲッター
const char cTap::GetMaxTapNum() const {
	return MAX_TAP_NUM;
}

//タッチされた数のゲッター
int cTap::GetTouchNum() const {
	return TouchNum;
}

//カウントのゲッター
short cTap::GetCount(const int _i) const {
	return Count[_i];
}

//座標Xのゲッター
int cTap::GetPosX(const int _i) const {
	return PosX[_i];
}

//IDのゲッター
int cTap::GetId(const int _i) const {
	return Id[_i];
}
