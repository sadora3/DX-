#pragma once

//FPSクラス
class cFps {
private:
	int StartTime;      //測定開始時刻
	int Count;          //カウンタ
	float Fps;          //fps
	const int N = 60;	//平均を取るサンプル数
	const int FPS = 60;	//設定したFPS
public:
	void Update();	//更新
	void Draw();	//描画
	void Wait();	//制御
};
