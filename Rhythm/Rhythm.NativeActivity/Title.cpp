#include"DxLib.h"
#include "Title.h"
#include"Main.h"

//更新
void cTitle::Update() {
	//未クリックの場合、返る
	if (MouseCount != 1) { return; }

	//マウス座標を取得
	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);

	//ボタンが押された場合
	const bool TEMP_X = MouseX > BUTTON_POS_X && MouseX < BUTTON_POS_X + BUTTON_SIZE_X;
	const bool TEMP_Y = MouseY > BUTTON_POS_Y && MouseY < BUTTON_POS_Y + BUTTON_SIZE_Y;
	if (TEMP_X && TEMP_Y) {
		//曲選択画面へ遷移
		ScreenState = SCREEN_SELECT_MUSIC;
	}
}

//描画
void cTitle::Draw() {
	//タイトルネームの描画
	DrawFormatString(TITLE_NAME_POS_X, TITLE_NAME_POS_Y, 0x000000, "たいとる");

	//スタートボタンの描画
	DrawBox(BUTTON_POS_X, BUTTON_POS_Y, BUTTON_POS_X + BUTTON_SIZE_X, BUTTON_POS_Y + BUTTON_SIZE_Y, 0x000000, FALSE);
	//TAP HEREの描画
	DrawFormatString(BUTTON_POS_X + 10, BUTTON_POS_Y + (int)(BUTTON_SIZE_Y * 0.2), 0x000000, "TAP HERE");
}
