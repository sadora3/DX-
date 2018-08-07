#include "MusicStartButton.h"
#include"Main.h"

//初期化
void cMusicStartButton::Init() {
	PlayFlag = false;
	StartTime = 0;
}

//更新
void cMusicStartButton::Update() {
	//曲が未再生だった場合
	if (!PlayFlag) {
		//未クリックの場合、返る
		if (MouseCount != 1) { return; }

		//マウス座標を取得
		int MouseX, MouseY;
		GetMousePoint(&MouseX, &MouseY);

		bool TempX;
		bool TempY;

		//FarstDelayの「＜」ボタンの判定
		TempX = MouseX > FD_X + 10 && MouseX < FD_X + 70;
		TempY = MouseY > FD_Y + 70 && MouseY < FD_Y + 140;
		if (TempX && TempY) { FarstDelay -= 0.01f;	return; }

		//FarstDelayの「＞」ボタンの判定
		TempX = MouseX > FD_X + 270 && MouseX < FD_X + 330;
		TempY = MouseY > FD_Y + 70 && MouseY < FD_Y + 140;
		if (TempX && TempY) { FarstDelay += 0.01f;	return; }

		//FarstDelayの「＜＜」ボタンの判定
		TempX = MouseX > FD_X - 200 && MouseX < FD_X - 60;
		TempY = MouseY > FD_Y + 70 && MouseY < FD_Y + 140;
		if (TempX && TempY) { FarstDelay -= 0.1f;	return; }

		//FarstDelayの「＞＞」ボタンの判定
		TempX = MouseX > FD_X + 400 && MouseX < FD_X + 520;
		TempY = MouseY > FD_Y + 70 && MouseY < FD_Y + 140;
		if (TempX && TempY) { FarstDelay += 0.1f;	return; }

		//FarstDelayの「DEFAULT」ボタンの判定
		TempX = MouseX > FD_X + 750 && MouseX < FD_X + 970;
		TempY = MouseY > FD_Y + 70 && MouseY < FD_Y + 140;
		if (TempX && TempY) { FarstDelay = 3.04f;	return; }

		//FarstDelayの「OK」ボタンの判定
		TempX = MouseX > FD_X + 610 && MouseX < FD_X + 700;
		TempY = MouseY > FD_Y + 70 && MouseY < FD_Y + 140;
		if (TempX && TempY) {
			//リストクリア
			NotesList.clear();
			//譜面読み込み
			LoadChart(SelectMusic.GetSelectMusicName());
			return;
		}



		//Y座標の確認
		bool CleckPosY = MouseY > POS_Y && MouseY < POS_Y + SIZE_Y;

		//Startボタンがクリックされたら、曲の再生開始
		if ((MouseX > POS_X && MouseX < POS_X + SIZE_X) && CleckPosY) {
			cTapNote::SetAutoPlayFlag(false);	//オートプレイフラグをON
		}
		//AutoPlayボタンがクリックされたら、曲の再生開始
		else if ((MouseX > POS_X2 && MouseX < POS_X2 + SIZE_X2) && CleckPosY) {
			cTapNote::SetAutoPlayFlag(true);	//オートプレイフラグをON
		}
		else {
			return;
		}

		//BGM再生
		PlayFlag = true;
		//開始時間を記録
		StartTime = GetNowCount();
	}
	else {
		//2秒後に曲を再生
		if (GetNowCount() - StartTime >= 2000) {
			Sound.PlayBgm();
		}

		//ノーツの更新
		auto IT = NotesList.begin();
		while (IT != NotesList.end()) {
			if (IT->Update()) {
				IT = NotesList.erase(IT);
				continue;
			}

			IT++;
		}
	}
}

//描画
void cMusicStartButton::Draw() {
	//スタートボタンの描画
	DrawBox(POS_X, POS_Y, POS_X + SIZE_X, POS_Y + SIZE_Y, 0xff0000, FALSE);
	//文字の描画
	DrawFormatString(POS_X + 10, POS_Y + (int)(SIZE_Y * 0.2f), 0x000000, "Start");

	//オートプレイボタンの描画
	DrawBox(POS_X2, POS_Y, POS_X2 + SIZE_X2, POS_Y + SIZE_Y, 0x0000ff, FALSE);
	//文字の描画
	DrawFormatString(POS_X2 + 10, POS_Y + (int)(SIZE_Y * 0.2f), 0x000000, "AutoPlay");


	//「FarstDelay」の描画
	DrawFormatString(FD_X, FD_Y, 0x000000, "FarstDelay");

	//FarstDelayの描画
	DrawFormatString(FD_X + 100, FD_Y + 70, 0x000000, "%.2f", FarstDelay);

	//FarstDelayの「＜」ボタンの描画
	DrawBox(FD_X + 10, FD_Y + 70, FD_X + 70, FD_Y + 140, 0x000000, FALSE);
	DrawFormatString(FD_X + 10, FD_Y + 70, 0x000000, "＜");

	//FarstDelayの「＞」ボタンの描画
	DrawBox(FD_X + 270, FD_Y + 70, FD_X + 330, FD_Y + 140, 0x000000, FALSE);
	DrawFormatString(FD_X + 270, FD_Y + 70, 0x000000, "＞");

	//FarstDelayの「＜＜」ボタンの描画
	DrawBox(FD_X - 200, FD_Y + 70, FD_X - 60, FD_Y + 140, 0x000000, FALSE);
	DrawFormatString(FD_X - 200, FD_Y + 70, 0x000000, "＜＜");

	//FarstDelayの「＞＞」ボタンの描画
	DrawBox(FD_X + 400, FD_Y + 70, FD_X + 520, FD_Y + 140, 0x000000, FALSE);
	DrawFormatString(FD_X + 400, FD_Y + 70, 0x000000, "＞＞");

	//OKボタンの描画
	DrawBox(FD_X + 610, FD_Y + 70, FD_X + 700, FD_Y + 140, 0x000000, FALSE);
	DrawFormatString(FD_X + 620, FD_Y + 70, 0x000000, "OK");

	//デフォルトボタンの描画
	DrawBox(FD_X + 750, FD_Y + 70, FD_X + 970, FD_Y + 140, 0x000000, FALSE);
	DrawFormatString(FD_X + 750, FD_Y + 70, 0x000000, "DEFAULT");
}

//再生フラグのゲッター
bool cMusicStartButton::GetPlayFlag() const {
	return PlayFlag;
}
