#pragma once

//曲再生ボタンクラス
class cMusicStartButton {
private:
	bool PlayFlag;	//再生フラグ
	int StartTime;	//開始時間

	const int POS_X = 100;	//座標X
	const int POS_Y = 500;	//座標Y
	const int SIZE_X = 190;	//大きさX
	const int SIZE_Y = 100;	//大きさY

	const int POS_X2 = 500;		//オートプレイの座標X
	const int SIZE_X2 = 280;	//オートプレイの大きさX

	const int FD_X = 300;
	const int FD_Y = 100;

public:
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画

	bool GetPlayFlag() const;	//再生フラグのゲッター
};
