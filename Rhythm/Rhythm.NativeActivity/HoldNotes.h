#pragma once

//ホールドノーツクラス
class cHoldNotes {
private:
	float TapTime;	//タップタイム(0になった瞬間にタップするとPEFECT)
	int PosX;		//X座標
	float PosY;		//Y座標

	int Time;		//ホールド時間(1～。1の場合8分1個。10の場合8分10個)
	int PosY;		//ホールドノーツの長さ
	bool FirstTap;	//ホールドノーツの最初のタップがされたかどうか
	int Count;		//ホールドの判定カウント
	float SafeTime;	//ホールド後、指を離しても大丈夫な時間
public:
	void Init(const float _Beat, const float _Scale, const int _Lane, const int _HoldTime);	//初期化
	bool Update();	//更新
	void Draw();	//描画
};

