#include"Fps.h"
#include"DxLib.h"

//更新
void cFps::Update() {
	//1フレーム目なら時刻を記憶
	if (Count == 0) {
		StartTime = GetNowCount();
	}
	//60フレーム目なら平均を計算する
	if (Count == N) {
		int t = GetNowCount();
		Fps = 1000.f / ((t - StartTime) / (float)N);
		Count = 0;
		StartTime = t;
	}
	Count++;
}

//描画
void cFps::Draw() {
	//FPSの描画
	DrawFormatString(0, 0, GetColor(0, 0, 0), "%.0f", Fps);
}

//制御
void cFps::Wait() {
	int tookTime = GetNowCount() - StartTime;		//かかった時間
	int waitTime = Count * 1000 / FPS - tookTime;	//待つべき時間
	if (waitTime > 0) {
		WaitTimer(waitTime);	//待機
	}
}

