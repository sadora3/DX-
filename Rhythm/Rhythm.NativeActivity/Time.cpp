#include"Time.h"
#include"DxLib.h"

//初期化
void cTime::Init() {
	//現在時間の取得
	NowTime = GetNowCount();
}

//更新
void cTime::Update() {
	OldTime = NowTime;				//前のループの時間を記憶
	NowTime = GetNowCount();		//現在時間の更新
	DeltaTime = NowTime - OldTime;	//前のループから、何秒経ったかの更新
}

//デルタタイムのゲッター
float cTime::GetDeltaTime() const {
	return DeltaTime / 1000.0f;
}

