#include "Config.h"
#include"DxLib.h"
#include"Main.h"

//更新
void cConfig::Init() {
	NotesPosY = 1;

	//ページ数の初期化
	Page = 1;
}

//更新
void cConfig::Update() {
	//1ページ目の描画
	if (Page == 1) {
		//ノーツの落下
		if (NotesPosY <= 650.0f) {
			NotesPosY += Time.GetDeltaTime() * 65.0f * cTapNote::GetSpeed();
			NotesWaitTime = 0.0f;
		}
		else {
			NotesWaitTime += Time.GetDeltaTime();
			if (NotesWaitTime > 1.0f) {
				NotesPosY = 0;
			}
		}

		//未クリックの場合、返る
		if (MouseCount != 1) { return; }

		//マウス座標を取得
		int MouseX, MouseY;
		GetMousePoint(&MouseX, &MouseY);

		bool TempX, TempY;

		//「曲選択画面に戻る」ボタンの判定
		TempX = MouseX > BACK_POS_X && MouseX < BACK_POS_X + 490;
		TempY = MouseY > BACK_POS_Y && MouseY < BACK_POS_Y + 70;
		if (TempX && TempY) {
			//曲選択画面に戻る
			ScreenState = SCREEN_SELECT_MUSIC;

			//設定のセーブ
			//FILE* fp;	fopen_s(&fp, "data/config.txt", "w");
			char FilePath[256];
			GetInternalDataPath(FilePath, sizeof(FilePath));
			strcat(FilePath, "/config.txt");
			FILE* fp = fopen(FilePath, "w");
			if (fp == NULL) { return; }
			fprintf(fp, "%f\n", cTapNote::GetSpeed());
			fprintf(fp, "%d\n", cTapNote::GetThickness());
			fprintf(fp, "%d\n", Sound.GetTapSeVolume());
			fclose(fp);
			return;
		}

		//「次のページへ進む」ボタンの判定
		TempX = MouseX > NEXT_PAGE_POS_X && MouseX < NEXT_PAGE_POS_X + 490;
		TempY = MouseY > NEXT_PAGE_POS_Y && MouseY < NEXT_PAGE_POS_Y + 70;
		if (TempX && TempY) {
			Page++;
		}

		//NOTES SPEEDの「＜」ボタンの判定
		TempX = MouseX > SPEED_POS_X + 10 && MouseX < SPEED_POS_X + 70;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { cTapNote::AddSpeed(-0.1f);	return; }

		//NOTES SPEEDの「＞」ボタンの判定
		TempX = MouseX > SPEED_POS_X + 270 && MouseX < SPEED_POS_X + 330;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { cTapNote::AddSpeed(0.1f);	return; }

		//NOTES SPEEDの「＜＜」ボタンの判定
		TempX = MouseX > SPEED_POS_X - 200 && MouseX < SPEED_POS_X - 60;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { cTapNote::AddSpeed(-1.0f);	return; }

		//NOTES SPEEDの「＞＞」ボタンの判定
		TempX = MouseX > SPEED_POS_X + 400 && MouseX < SPEED_POS_X + 520;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { cTapNote::AddSpeed(1.0f);	return; }



		//NOTES THICKNESSの「＜」ボタンの描画
		TempX = MouseX > THICKNESS_POS_X + 80 && MouseX < THICKNESS_POS_X + 140;
		TempY = MouseY > THICKNESS_POS_Y + 70 && MouseY < THICKNESS_POS_Y + 140;
		if (TempX && TempY) { cTapNote::AddThickness(-1);	return; }

		//NOTES SPEEDの「＞」ボタンの判定
		TempX = MouseX > THICKNESS_POS_X + 340 && MouseX < THICKNESS_POS_X + 400;
		TempY = MouseY > THICKNESS_POS_Y + 70 && MouseY < THICKNESS_POS_Y + 140;
		if (TempX && TempY) { cTapNote::AddThickness(1);	return; }
	}
	else if (Page == 2) {
		//未クリックの場合、返る
		if (MouseCount != 1) { return; }

		//マウス座標を取得
		int MouseX, MouseY;
		GetMousePoint(&MouseX, &MouseY);

		bool TempX, TempY;

		//「前のページに戻る」ボタンの判定
		TempX = MouseX > BACK_POS_X && MouseX < BACK_POS_X + 490;
		TempY = MouseY > BACK_POS_Y && MouseY < BACK_POS_Y + 70;
		if (TempX && TempY) {
			Page--;
		}



		//TAP VOLUMEの「＜」ボタンの判定
		TempX = MouseX > SPEED_POS_X + 10 && MouseX < SPEED_POS_X + 70;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { Sound.AddTapSeVolume(-1);	return; }

		//TAP VOLUMEの「＞」ボタンの判定
		TempX = MouseX > SPEED_POS_X + 270 && MouseX < SPEED_POS_X + 330;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { Sound.AddTapSeVolume(1);	return; }

		//TAP VOLUMEの「＜＜」ボタンの判定
		TempX = MouseX > SPEED_POS_X - 200 && MouseX < SPEED_POS_X - 60;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { Sound.AddTapSeVolume(-10);	return; }

		//TAP VOLUMEの「＞＞」ボタンの判定
		TempX = MouseX > SPEED_POS_X + 400 && MouseX < SPEED_POS_X + 520;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { Sound.AddTapSeVolume(10);	return; }
	}
}

//描画
void cConfig::Draw() {
	//1ページ目の描画
	if (Page == 1) {
		//「曲選択画面に戻る」ボタンの描画
		DrawBox(BACK_POS_X, BACK_POS_Y, BACK_POS_X + 490, BACK_POS_Y + 70, 0x000000, FALSE);
		DrawFormatString(BACK_POS_X, BACK_POS_Y, 0x000000, "曲選択画面に戻る");
		//「次のページへ進む」ボタンの描画
		DrawBox(NEXT_PAGE_POS_X, NEXT_PAGE_POS_Y, NEXT_PAGE_POS_X + 490, NEXT_PAGE_POS_Y + 70, 0x000000, FALSE);
		DrawFormatString(NEXT_PAGE_POS_X, NEXT_PAGE_POS_Y, 0x000000, "次のページへ進む");

		//「NOTES SPEED」の描画
		DrawFormatString(SPEED_POS_X, SPEED_POS_Y, 0x000000, "NOTES SPEED");

		//NOTES SPEEDの描画
		DrawFormatString(SPEED_POS_X + 120, SPEED_POS_Y + 70, 0x000000, "%.1f", cTapNote::GetSpeed());

		//NOTES SPEEDの「＜」ボタンの描画
		DrawBox(SPEED_POS_X + 10, SPEED_POS_Y + 70, SPEED_POS_X + 70, SPEED_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(SPEED_POS_X + 10, SPEED_POS_Y + 70, 0x000000, "＜");

		//NOTES SPEEDの「＞」ボタンの描画
		DrawBox(SPEED_POS_X + 270, SPEED_POS_Y + 70, SPEED_POS_X + 330, SPEED_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(SPEED_POS_X + 270, SPEED_POS_Y + 70, 0x000000, "＞");

		//NOTES SPEEDの「＜＜」ボタンの描画
		DrawBox(SPEED_POS_X - 200, SPEED_POS_Y + 70, SPEED_POS_X - 60, SPEED_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(SPEED_POS_X - 200, SPEED_POS_Y + 70, 0x000000, "＜＜");

		//NOTES SPEEDの「＞＞」ボタンの描画
		DrawBox(SPEED_POS_X + 400, SPEED_POS_Y + 70, SPEED_POS_X + 520, SPEED_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(SPEED_POS_X + 400, SPEED_POS_Y + 70, 0x000000, "＞＞");



		//「NOTES THICKNESS」の描画
		DrawFormatString(THICKNESS_POS_X, THICKNESS_POS_Y, 0x000000, "NOTES THICKNESS");

		//NOTES THICKNESSの描画
		DrawFormatString(THICKNESS_POS_X + 220, THICKNESS_POS_Y + 70, 0x000000, "%d", cTapNote::GetThickness());

		//NOTES THICKNESSの「＜」ボタンの描画
		DrawBox(THICKNESS_POS_X + 80, THICKNESS_POS_Y + 70, THICKNESS_POS_X + 140, THICKNESS_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(THICKNESS_POS_X + 80, THICKNESS_POS_Y + 70, 0x000000, "＜");

		//NOTES THICKNESSの「＞」ボタンの描画
		DrawBox(THICKNESS_POS_X + 340, THICKNESS_POS_Y + 70, THICKNESS_POS_X + 400, THICKNESS_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(THICKNESS_POS_X + 340, THICKNESS_POS_Y + 70, 0x000000, "＞");

		//ノーツの描画
		if (NotesWaitTime <= 0.05f) {
			DrawLine(520, NotesPosY, 760, NotesPosY, 0xff0000, cTapNote::GetThickness());
		}
	}
	else if (Page == 2) {
		//「前のページに戻る」ボタンの描画
		DrawBox(BACK_POS_X, BACK_POS_Y, BACK_POS_X + 490, BACK_POS_Y + 70, 0x000000, FALSE);
		DrawFormatString(BACK_POS_X, BACK_POS_Y, 0x000000, "前のページに戻る");

		//「TAP VOLUME」の描画
		DrawFormatString(SPEED_POS_X, SPEED_POS_Y, 0x000000, "TAP VOLUME");

		//TAP VOLUMEの描画
		DrawFormatString(SPEED_POS_X + 120, SPEED_POS_Y + 70, 0x000000, "%d", Sound.GetTapSeVolume());

		//TAP VOLUMEの「＜」ボタンの描画
		DrawBox(SPEED_POS_X + 10, SPEED_POS_Y + 70, SPEED_POS_X + 70, SPEED_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(SPEED_POS_X + 10, SPEED_POS_Y + 70, 0x000000, "＜");

		//TAP VOLUMEの「＞」ボタンの描画
		DrawBox(SPEED_POS_X + 270, SPEED_POS_Y + 70, SPEED_POS_X + 330, SPEED_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(SPEED_POS_X + 270, SPEED_POS_Y + 70, 0x000000, "＞");

		//TAP VOLUMEの「＜＜」ボタンの描画
		DrawBox(SPEED_POS_X - 200, SPEED_POS_Y + 70, SPEED_POS_X - 60, SPEED_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(SPEED_POS_X - 200, SPEED_POS_Y + 70, 0x000000, "＜＜");

		//TAP VOLUMEの「＞＞」ボタンの描画
		DrawBox(SPEED_POS_X + 400, SPEED_POS_Y + 70, SPEED_POS_X + 520, SPEED_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(SPEED_POS_X + 400, SPEED_POS_Y + 70, 0x000000, "＞＞");

	}
}
