#pragma once

//曲選択クラス
class cSelectMusic {
private:
	const int CONFIG_BUTTON_POS_X = 800;	//設定ボタンの座標X
	const int CONFIG_BUTTON_POS_Y = 600;	//設定ボタンの座標Y
	const int CONFIG_BUTTON_SIZE_X = 400;	//設定ボタンの大きさX
	const int CONFIG_BUTTON_SIZE_Y = 100;	//設定ボタンの大きさY

	const int X = 150;		//曲選択ボタンの座標X
	const int Y = 100;		//曲選択ボタンの座標Y
	const int SX = 930;		//曲選択ボタンの大きさX
	const int SY = 100;		//曲選択ボタンの大きさY

	char SelectMusicName[256];	//選択した曲名
public:
	void Update();	//更新
	void Draw();	//描画

	char* GetSelectMusicName();	//選択した曲名のゲッター
};

