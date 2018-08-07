#pragma once

//TAPノーツクラス
class cTapNote {
private:
	float TapTime;	//タップタイム(0になった瞬間にタップするとPEFECT)
	int PosX;		//X座標
	float PosY;		//Y座標

	bool TapProc();		//タップ
	bool AutoTapProc();	//オートタップ
public:
	void Init(const float _Beat, const float _Scale, const int _Lane);	//初期化
	bool Update();	//更新
	void Draw();	//描画
};

