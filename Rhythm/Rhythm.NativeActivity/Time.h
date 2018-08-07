#pragma once

//タイムクラス
class cTime {
private:
	int OldTime;	//1ループ前の時間
	int NowTime;	//現在の時間
	int DeltaTime;	//前のループから、何秒経ったか
public:
	void Init();				//初期化
	void Update();				//更新
	float GetDeltaTime() const;	//デルタタイムのゲッター
};
