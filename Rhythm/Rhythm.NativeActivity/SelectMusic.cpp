#include "SelectMusic.h"
#include"DxLib.h"
#include"Main.h"

//更新
void cSelectMusic::Update() {
	//未クリックの場合、返る
	if (MouseCount != 1) { return; }

	//マウス座標を取得
	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);

	//コンフィグボタンが押された
	const bool TEMP_X = MouseX > CONFIG_BUTTON_POS_X && MouseX < CONFIG_BUTTON_POS_X + CONFIG_BUTTON_SIZE_X;
	const bool TEMP_Y = MouseY > CONFIG_BUTTON_POS_Y && MouseY < CONFIG_BUTTON_POS_Y + CONFIG_BUTTON_SIZE_Y;
	if (TEMP_X && TEMP_Y) {
		ScreenState = SCREEN_CONFIG;
	}

	//曲ボタンが押された場合
	for (int i = 0; i < 4; i++) {
		const bool TEMP_X2 = MouseX > X && MouseX < X + SX;
		const bool TEMP_Y2 = MouseY > Y + SY * (i + 0) + (i * 20) && MouseY < Y + SY * (i + 1) + (i * 20);
		if (TEMP_X2 && TEMP_Y2) {
			//ゲーム画面へ遷移
			ScreenState = SCREEN_GAME;

			//エンドマークだった場合
			if (i == 0) {
				//BPMのセット
				cTapNote::SetBpm(257.0f);

				//FarstDelayの設定（これはバグを生むかもしれないの）
				FarstDelay = 3.04f;

				//選択した曲名の記憶
				strcpy(SelectMusicName, "data/End.txt");

				//譜面読み込み
				LoadChart(SelectMusicName);

				//曲の読み込み
				Sound.SetBgm("data/End.wav");
			}

			//La Baguette Magiqueだった場合
			if (i == 1) {
				//BPMのセット
				cTapNote::SetBpm(230.0f);

				//FarstDelayの設定（これはバグを生むかもしれないの）
				FarstDelay = 2.72f;

				//選択した曲名の記憶
				strcpy(SelectMusicName, "data/La Baguette Magique.txt");

				//譜面読み込み
				LoadChart(SelectMusicName);

				//曲の読み込み
				Sound.SetBgm("data/La Baguette Magique.wav");
			}
			break;
		}
	}
}

//描画
void cSelectMusic::Draw() {
	//設定ボタンの描画
	DrawBox(CONFIG_BUTTON_POS_X, CONFIG_BUTTON_POS_Y, CONFIG_BUTTON_POS_X + CONFIG_BUTTON_SIZE_X, CONFIG_BUTTON_POS_Y + CONFIG_BUTTON_SIZE_Y, 0x000000, FALSE);
	//TAP HEREの描画
	DrawFormatString(CONFIG_BUTTON_POS_X + 10, CONFIG_BUTTON_POS_Y + (int)(CONFIG_BUTTON_SIZE_Y * 0.2), 0x000000, "GO TO CONFIG");

	//曲ボタンの描画
	char Temp[4][256] = {
		"エンドマークに希望と涙を添えて", "La Baguette Magique", "まだない", "まだ無い"
	};

	for (int i = 0; i < 4; i++) {
		DrawBox(X, Y + SY * (i + 0) + (i * 20), X + SX, Y + SY * (i + 1) + (i * 20), 0x000000, FALSE);

		const int POS_Y = Y + SY * (i + 0) + (int)(SY * 0.2) + (i * 20);
		DrawFormatString(X - 70, POS_Y, 0x000000, "%d", i + 1);
		DrawFormatString(X + 10, POS_Y, 0x000000, Temp[i]);
	}

}

//選択した曲名のゲッター
char* cSelectMusic::GetSelectMusicName() {
	return SelectMusicName;
}
