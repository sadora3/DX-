#pragma once

//タイトルクラス
class cTitle {
private:
	const int TITLE_NAME_POS_X = 500;	//タイトル名の座標X
	const int TITLE_NAME_POS_Y = 200;	//タイトル名の座標Y

	const int BUTTON_POS_X = 900;		//ボタンの座標X
	const int BUTTON_POS_Y = 500;		//ボタンの座標Y
	const int BUTTON_SIZE_X = 280;		//ボタンの大きさX
	const int BUTTON_SIZE_Y = 100;		//ボタンの大きさY
public:
	void Update();	//更新
	void Draw();	//描画
};


