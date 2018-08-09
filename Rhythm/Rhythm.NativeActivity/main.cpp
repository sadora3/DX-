//インクルード
#include"Main.h"

int MouseCount = 0;

//エントリーポイント
int android_main() {
	SetGraphMode(1280, 720, 32);			//画面解像度の設定
	SetBackgroundColor(200, 200, 250);		//背景色の設定
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) { return -1; }

	//初期化
	Init();

	

	//メインループ
	while (ProcessLoop()) {
		//更新
		Update();
		//描画
		Draw();

		ScreenFlip();
		Fps.Wait();
	}

	//後処理
	Uninit();
	DxLib_End();

	return  0;
}

//メインループ処理
bool ProcessLoop() {
	if (ProcessMessage() != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }
	return true;
}

//初期化
void Init() {
	//フォントサイズの変更
	SetFontSize(60);

	//サウンド初期化
	Sound.Init();
	//タイム初期化
	Time.Init();
	//タップの初期化
	Tap.Init();

	//設定の初期化
	Config.Init();
	//曲再生ボタンの初期化
	MusicStartButton.Init();

	//設定データの読み込み
	//int fp = FileRead_open("data/config.txt");
	char FilePath[256];
	GetInternalDataPath(FilePath, sizeof(FilePath));
	strcat(FilePath, "/config.txt");
	int fp = FileRead_open(FilePath);
	if (fp == 0) { return; }
	float TempSpeed;
	int TempThickness;
	int TempTapSeVolume;
	if (FileRead_scanf(fp, "%f", &TempSpeed) != -1) { cNote::SetSpeed(TempSpeed); }
	if (FileRead_scanf(fp, "%d", &TempThickness) != -1) { cNote::SetThickness(TempThickness); }
	if (FileRead_scanf(fp, "%d", &TempTapSeVolume) != -1) { Sound.SetTapSeVolume(TempTapSeVolume); }
	FileRead_close(fp);
}

//更新
void Update() {
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		MouseCount++;
	}
	else {
		MouseCount = 0;
	}

	//FPSの更新
	Fps.Update();

	switch (ScreenState) {
	case SCREEN_TITLE:
		//タイトルの更新
		Title.Update();
		break;

	case SCREEN_SELECT_MUSIC:
		//曲選択の更新
		SelectMusic.Update();
		break;

	case SCREEN_CONFIG:
		//タイムの更新
		Time.Update();
		//設定の更新
		Config.Update();
		break;

	case SCREEN_GAME:
		//タイムの更新
		Time.Update();
		//タップの更新
		Tap.Update();
		//曲再生ボタンの更新
		MusicStartButton.Update();
		break;
	}
}

//描画
void Draw() {
	//FPSの描画
	Fps.Draw();

	switch (ScreenState) {
	case SCREEN_TITLE:
		//タイトルの描画
		Title.Draw();
		break;

	case SCREEN_SELECT_MUSIC:
		//曲選択の描画
		SelectMusic.Draw();
		break;

	case SCREEN_CONFIG:
		//設定の描画
		Config.Draw();
		break;

	case SCREEN_GAME:
		//曲が再生中の場合
		if (MusicStartButton.GetPlayFlag()) {
			//判定数の描画
			cNote::DecisionNumDraw();

			//レーンのバック描画
			const int BackPosX1 = Line.GetDecisionLineX1();
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox(BackPosX1, 0, BackPosX1 + Line.GetLaneInterval() * 5, SCREEN_HEIGHT, 0x424242, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			//判定ラインの描画
			Line.Draw();

			//ノーツの描画
			for (auto IT = NotesList.begin(); IT != NotesList.end(); IT++) {
				IT->Draw();
			}
		}
		else {
			//曲再生ボタンの描画
			MusicStartButton.Draw();
		}
		break;
	}

	//あとで消す
	DrawFormatString(0, 100, 0x000000, "%d", NotesList.size());
}

//後処理
void Uninit() {
	//サウンドの後処理
	Sound.Uninit();
}

//更新
void cFps::Update() {
	//1フレーム目なら時刻を記憶
	if (Count == 0) {
		StartTime = GetNowCount();
	}
	//60フレーム目なら平均を計算する
	if (Count == N) {
		int t = GetNowCount();
		Fps = 1000.f / ((t - StartTime) / (float)N);
		Count = 0;
		StartTime = t;
	}
	Count++;
}

//描画
void cFps::Draw() {
	//FPSの描画
	DrawFormatString(0, 0, GetColor(0, 0, 0), "%.0f", Fps);
}

//制御
void cFps::Wait() {
	int tookTime = GetNowCount() - StartTime;		//かかった時間
	int waitTime = Count * 1000 / FPS - tookTime;	//待つべき時間
	if (waitTime > 0) {
		WaitTimer(waitTime);	//待機
	}
}

//初期化
void cTime::Init() {
	//現在時間の取得
	NowTime = GetNowCount();
}

//更新
void cTime::Update() {
	OldTime = NowTime;				//前のループの時間を記憶
	NowTime = GetNowCount();		//現在時間の更新
	DeltaTime = NowTime - OldTime;	//前のループから、何秒経ったかの更新
}

//デルタタイムのゲッター
float cTime::GetDeltaTime() const {
	return DeltaTime / 1000.0f;
}

//初期化
void cSound::Init() {
	//BGMの音量調整
	ChangeVolumeSoundMem(168, BgmSh);
	//タップ音の読み込み
	TapSh = LoadSoundMem("data/TAP.wav");
}

//BGMの再生
void cSound::PlayBgm() {
	//BGMが未再生の場合、再生
	if (!BgmPlayFlag) {
		BgmPlayFlag = true;
		PlaySoundMem(BgmSh, DX_PLAYTYPE_BACK);
	}
}

//タップ音の再生
void cSound::PlayTap() {
	PlaySoundMem(TapSh, DX_PLAYTYPE_BACK);
}

//後処理
void cSound::Uninit() {
	DeleteSoundMem(BgmSh);
	DeleteSoundMem(TapSh);
}

//タップ音のボリュームのゲッター
int cSound::GetTapSeVolume() const {
	return TapSeVolume;
}

//タップ音のボリュームの加減算
void cSound::AddTapSeVolume(int _Val) {
	if (_Val == 1 && TapSeVolume < 255) {
		TapSeVolume += _Val;
	}
	else if (_Val == -1 && TapSeVolume > 1) {
		TapSeVolume += _Val;
	}
	else if (_Val == 10) {
		if (TapSeVolume < 244) {
			TapSeVolume += _Val;
		}
		else {
			TapSeVolume = 255;
		}
	}
	else if (_Val == -10) {
		if (TapSeVolume > 10) {
			TapSeVolume += _Val;
		}
		else {
			TapSeVolume = 0;
		}
	}

	//ボリュームの反映
	ChangeVolumeSoundMem(TapSeVolume, TapSh);
}

//タップ音のボリュームのセッター
void cSound::SetTapSeVolume(int _Val) {
	TapSeVolume = _Val;

	//ボリュームの反映
	ChangeVolumeSoundMem(TapSeVolume, TapSh);
}

//BGMのセッター
void cSound::SetBgm(const char* _BgmName) {
	//BGMの読み込み
	BgmSh = LoadSoundMem(_BgmName);
}

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
				cNote::SetBpm(257.0f);

				//FarstDelayの設定（これはバグを生むかもしれないの）
				FarstDelay = 3.04f;

				//選択した曲名の記憶
				strcpy(SelectMusicName, "data/End.txt");

				//譜面読み込み
				int fp = FileRead_open(SelectMusicName);
				if (fp == 0) { return; }
				float TempBeat, TempScale;
				int TempLane, TempHoldFlag, TempHoldTime;
				while (FileRead_scanf(fp, "%f %f %d %d %d", &TempBeat, &TempScale, &TempLane, &TempHoldFlag, &TempHoldTime) != -1) {
					NotesList.push_back(cNote());
					(--NotesList.end())->Init(TempBeat, TempScale, TempLane, TempHoldFlag, TempHoldTime);
				}
				FileRead_close(fp);

				//曲の読み込み
				Sound.SetBgm("data/End.wav");
			}

			//La Baguette Magiqueだった場合
			if (i == 1) {
				//BPMのセット
				cNote::SetBpm(230.0f);

				//FarstDelayの設定（これはバグを生むかもしれないの）
				FarstDelay = 2.72f;

				//選択した曲名の記憶
				strcpy(SelectMusicName, "data/La Baguette Magique.txt");

				//譜面読み込み
				int fp = FileRead_open(SelectMusicName);
				if (fp == 0) { return; }
				float TempBeat, TempScale;
				int TempLane, TempHoldFlag, TempHoldTime;
				while (FileRead_scanf(fp, "%f %f %d %d %d", &TempBeat, &TempScale, &TempLane, &TempHoldFlag, &TempHoldTime) != -1) {
					NotesList.push_back(cNote());
					(--NotesList.end())->Init(TempBeat, TempScale, TempLane, TempHoldFlag, TempHoldTime);
				}
				FileRead_close(fp);

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
			NotesPosY += Time.GetDeltaTime() * 65.0f * cNote::GetSpeed();
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
			fprintf(fp, "%f\n", cNote::GetSpeed());
			fprintf(fp, "%d\n", cNote::GetThickness());
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
		if (TempX && TempY) { cNote::AddSpeed(-0.1f);	return; }

		//NOTES SPEEDの「＞」ボタンの判定
		TempX = MouseX > SPEED_POS_X + 270 && MouseX < SPEED_POS_X + 330;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { cNote::AddSpeed(0.1f);	return; }

		//NOTES SPEEDの「＜＜」ボタンの判定
		TempX = MouseX > SPEED_POS_X - 200 && MouseX < SPEED_POS_X - 60;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { cNote::AddSpeed(-1.0f);	return; }

		//NOTES SPEEDの「＞＞」ボタンの判定
		TempX = MouseX > SPEED_POS_X + 400 && MouseX < SPEED_POS_X + 520;
		TempY = MouseY > SPEED_POS_Y + 70 && MouseY < SPEED_POS_Y + 140;
		if (TempX && TempY) { cNote::AddSpeed(1.0f);	return; }



		//NOTES THICKNESSの「＜」ボタンの描画
		TempX = MouseX > THICKNESS_POS_X + 80 && MouseX < THICKNESS_POS_X + 140;
		TempY = MouseY > THICKNESS_POS_Y + 70 && MouseY < THICKNESS_POS_Y + 140;
		if (TempX && TempY) { cNote::AddThickness(-1);	return; }

		//NOTES SPEEDの「＞」ボタンの判定
		TempX = MouseX > THICKNESS_POS_X + 340 && MouseX < THICKNESS_POS_X + 400;
		TempY = MouseY > THICKNESS_POS_Y + 70 && MouseY < THICKNESS_POS_Y + 140;
		if (TempX && TempY) { cNote::AddThickness(1);	return; }
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
		DrawFormatString(SPEED_POS_X + 120, SPEED_POS_Y + 70, 0x000000, "%.1f", cNote::GetSpeed());

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
		DrawFormatString(THICKNESS_POS_X + 220, THICKNESS_POS_Y + 70, 0x000000, "%d", cNote::GetThickness());

		//NOTES THICKNESSの「＜」ボタンの描画
		DrawBox(THICKNESS_POS_X + 80, THICKNESS_POS_Y + 70, THICKNESS_POS_X + 140, THICKNESS_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(THICKNESS_POS_X + 80, THICKNESS_POS_Y + 70, 0x000000, "＜");

		//NOTES THICKNESSの「＞」ボタンの描画
		DrawBox(THICKNESS_POS_X + 340, THICKNESS_POS_Y + 70, THICKNESS_POS_X + 400, THICKNESS_POS_Y + 140, 0x000000, FALSE);
		DrawFormatString(THICKNESS_POS_X + 340, THICKNESS_POS_Y + 70, 0x000000, "＞");

		//ノーツの描画
		if (NotesWaitTime <= 0.05f) {
			DrawLine(520, NotesPosY, 760, NotesPosY, 0xff0000, cNote::GetThickness());
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

//初期化
void cTap::Init() {
	for (int i = 0; i < MAX_TAP_NUM; i++) {
		Count[i] = 0;	//タップカウントの初期化
		Id[i] = -1;		//IDの初期化
	}
}

//更新
void cTap::Update() {
	//IDのリセット
	for (int i = 0; i < MAX_TAP_NUM; i++) {
		Id[i] = -1;
	}

	//タップの数だけループ
	TouchNum = GetTouchInputNum();
	int TempPosX, TempId;
	for (int TapNum = 0; TapNum < TouchNum; TapNum++) {
		// タッチされている箇所の座標を取得
		GetTouchInput(TapNum, &TempPosX, NULL, &TempId, NULL);

		//座標を代入
		PosX[TempId] = TempPosX;
		//IDを代入
		Id[TempId] = TempId;
		//タッチされているフレーム数をカウント
		Count[TempId]++;
	}
	//タップされていないタップカウントは初期化
	for (int TapNum = 0; TapNum < MAX_TAP_NUM; TapNum++) {
		//タップされていたら、次へ
		if (Id[TapNum] != -1) { continue; }

		Count[TapNum] = 0;
		PosX[TapNum] = 0;
	}
}

//最大タップ数のゲッター
const char cTap::GetMaxTapNum() const {
	return MAX_TAP_NUM;
}

//タッチされた数のゲッター
int cTap::GetTouchNum() const {
	return TouchNum;
}

//カウントのゲッター
short cTap::GetCount(const int _i) const {
	return Count[_i];
}

//座標Xのゲッター
int cTap::GetPosX(const int _i) const {
	return PosX[_i];
}

//IDのゲッター
int cTap::GetId(const int _i) const {
	return Id[_i];
}

//描画
void cLine::Draw() {
	//判定ラインの描画
	DrawLine(DECISION_LINE_X1, DECISION_LINE_Y, DECISION_LINE_X2, DECISION_LINE_Y, GetColor(255, 255, 0));

	//レーンの描画
	for (int i = 0; i < LANE_NUM + 1; i++) {
		const int POS_X = DECISION_LINE_X1 + i * LANE_INTERVAL;
		DrawLine(POS_X, 0, POS_X, SCREEN_HEIGHT, GetColor(0, 0, 0));
	}
}

//判定ラインの左のX座標のゲッター
int cLine::GetDecisionLineX1() const {
	return DECISION_LINE_X1;
}

//レーンの間隔のゲッター
int cLine::GetLaneInterval() const {
	return LANE_INTERVAL;
}

//初期化
;//第一引数：何ビート目か
;//第二引数：SPBのスケーリング(0.0f～1.0f)。4分なら0、8分なら0.5、16分なら0.25 or 0.75
;//第三引数：レーンの番号(1～5)
;//第四引数：ホールドフラグ
;//第五引数：ホールドタイム(1～。1の場合8分1個。10の場合8分10個)
void cNote::Init(const float _Beat, const float _Scale, const int _Lane, const bool _HoldFlag, const int _HoldTime) {
	//1ビートにつき何秒か
	const float SPB = 60.0f / Bpm;

	//タップタイムの計算
	TapTime = (_Beat + _Scale) * SPB + FarstDelay;
	//Y座標の計算
	PosY = 650.0f - 65.0f * Speed * TapTime;
	//X座標の計算
	PosX = Line.GetDecisionLineX1() + (_Lane - 1) * SIZE;

	//ホールドフラグの設定
	HoldFlag = _HoldFlag;
	//ホールド時間の設定
	HoldTime = _HoldTime;
	//ホールドノーツの長さ
	HoldPosY = (int)(HoldTime * 65.0f * Speed * 0.5f * SPB);
	//最初は未タップ状態にする
	HoldFirstTap = false;
	//ホールド判定のカウントの初期化
	HoldCount = 1;
}

//更新
bool cNote::Update() {
	//タップ
	if (TapProc()) {
		return true;
	}

	//画面下部に到達してない場合
	if (PosY < SCREEN_HEIGHT || HoldFlag) {
		//ホールドノーツで画面下部にいった場合は、ミス判定を+1する
		if (PosY >= SCREEN_HEIGHT && !HoldFirstTap) {
			MissNum++;
			HoldFirstTap = true;
		}

		//落下
		PosY += Time.GetDeltaTime() * 65.0f * Speed;
	}
	else {
		//画面下部に来たら、殺す
		MissNum++;
		ComboNum = 0;
		return true;
	}

	return false;
}

//タップ
bool cNote::TapProc() {
	//タップタイムをカウントダウン
	TapTime -= Time.GetDeltaTime();

	//オートプレイフラグがONの場合、返る
	if (AutoPlayFlag) {
		bool ExistFlag = AutoTapProc();
		return ExistFlag;
	}

	//タップタイムがバッドよりもあった場合、返る
	if (TapTime > BAD_DECISION) { return false; }

	//ホールドの処理
	if (HoldFlag) {
		//セーフタイムを減少
		if (HoldSafeTime >= 0.0f) {
			HoldSafeTime -= Time.GetDeltaTime();
		}

		//1ビートにつき何秒か
		const float SPB = 60.0f / Bpm;
		//8分の判定
		float HoldTapTime = TapTime + (0.5f * HoldCount) * SPB;

		//タップされている間の処理
		bool TapSuccessFlag = false;
		for (int TapNum = 0; TapNum < Tap.GetMaxTapNum(); TapNum++) {
			//IDが-1の場合、次へ
			if (Tap.GetId(TapNum) == -1) { continue; }

			//タップされた場所が、ノーツと同じレーンだった場合
			if (Tap.GetPosX(TapNum) > PosX && Tap.GetPosX(TapNum) < PosX + SIZE) {
				//セーフタイムのセット
				HoldSafeTime = 0.5f;
			}

			//セーフタイムが残っている間
			if (HoldSafeTime > 0.0f) {
				//8分のタイミングでホールドされているか判定
				if (HoldTapTime < 0.01f && HoldTapTime > -0.01f) {
					TapSuccessFlag = true;
					PerfectNum++;
					ComboNum++;
					HoldCount++;
					break;
				}
			}
		}
		//タップされていない時の処理
		if (!TapSuccessFlag) {
			if (HoldTapTime < 0.01f && HoldTapTime > -0.01f) {
				MissNum++;
				ComboNum = 0;
				HoldCount++;
			}
		}

		//最後まで来てたら、殺す
		if (HoldCount > HoldTime) {
			Sound.PlayTap();
			return true;
		}
	}

	//タップされたら
	for (int TapNum = 0; TapNum < Tap.GetMaxTapNum(); TapNum++) {
		//IDが-1の場合、次へ
		if (Tap.GetId(TapNum) == -1) { continue; }

		//最初の1フレーム目だけ反応
		if (Tap.GetCount(TapNum) == 1) {
			//既にタップされてたら、返る(ホールドノーツ用)
			if (HoldFirstTap) { continue; }

			//タップされたレーンが違ったら、次へ
			if (Tap.GetPosX(TapNum) < PosX || Tap.GetPosX(TapNum) > PosX + SIZE) { continue; }

			//タップに成功したかどうか
			bool TapSuccess = false;

			//パーフェクトの判定
			if (TapTime <= PERFECT_DECISION && TapTime >= -PERFECT_DECISION) {
				PerfectNum++;
				ComboNum++;
				TapSuccess = true;
			}
			//早い・遅い
			else if (TapTime <= EARLY_DECISION && TapTime >= -EARLY_DECISION) {
				if (TapTime > 0) {
					EarlyNum++;
					ComboNum++;
					TapSuccess = true;
				}else {
					LateNum++;
					ComboNum++;
					TapSuccess = true;
				}
			}
			//バッド
			else if (TapTime <= BAD_DECISION && TapTime >= -BAD_DECISION) {
				BadNum++;
				ComboNum = 0;
				TapSuccess = true;
			}

			//タップに成功してた場合
			if (TapSuccess) {
				//タップ音再生
				Sound.PlayTap();

				//ホールドノーツであれば、最初のタップをONにする
				if (HoldFlag) {
					HoldFirstTap = true;
				}
				else {
					//タップノーツであれば、殺す
					return true;
				}
			}
		}
	}

	return false;
}

//オートタップ
bool cNote::AutoTapProc() {
	//タップタイムがバッドよりもあった場合、返る
	if (TapTime > BAD_DECISION) { return false; }

	//ホールドの処理
	if (HoldFlag) {
		//1ビートにつき何秒か
		const float SPB = 60.0f / Bpm;
		//8分の判定
		float HoldTapTime = TapTime + (0.5f * HoldCount) * SPB;

		//タップされている間の処理
		if (HoldTapTime <= 0.0f) {
			PerfectNum++;
			ComboNum++;
			HoldCount++;
		}

		//最後まで来たら、殺す
		if (HoldCount > HoldTime) {
			Sound.PlayTap();
			return true;
		}
	}

	//既にタップされてたら、返る(ホールドノーツ用)
	if (HoldFirstTap) { return false; }

	//オートタップ
	if (TapTime <= 0.0f) {
		PerfectNum++;
		ComboNum++;

		//タップ音再生
		Sound.PlayTap();

		//ホールドノーツであれば、最初のタップをONにする
		if (HoldFlag) {
			HoldFirstTap = true;
		}
		else {
			//タップノーツであれば、殺す
			return true;
		}
	}

	return false;
}

//描画
void cNote::Draw() {
	//画面に入らないノーツは、返る
	if (PosY < 0.0f) { return; }

	//ノーツの描画
	if (!HoldFlag) {
		//タップノーツの描画
		DrawLine(PosX, (int)PosY, PosX + SIZE, (int)PosY, 0xFF0000, Thickness);
	}
	else {
		//ホールドノーツの描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
		DrawBox(PosX, (int)PosY - HoldPosY, PosX + SIZE, (int)PosY, 0xF3F781, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

//判定数の描画
void cNote::DecisionNumDraw() {
	//パーフェクトの数の描画
	DrawFormatString(300, 150, 0xffff00, "P:%d", PerfectNum);
	//早いの数の描画
	DrawFormatString(500, 115, 0xff8000, "E:%d", EarlyNum);
	//早いの数の描画
	DrawFormatString(500, 185, 0xff8000, "L:%d", LateNum);
	//バッドの数の描画
	DrawFormatString(650, 150, 0x0000ff, "B:%d", BadNum);
	//ミスの数の描画
	DrawFormatString(800, 150, 0x505050, "M:%d", MissNum);
	//コンボ数の描画
	DrawFormatString(580, 250, 0xffffff, "%d", ComboNum);
}

//速さのゲッター
float cNote::GetSpeed() {
	return Speed;
}

//太さのゲッター
int cNote::GetThickness() {
	return Thickness;
}

//速さの加減算
void cNote::AddSpeed(const float _Speed) {
	Speed += _Speed;

	if (Speed < 1.0f) { Speed = 1.0f; }
	else if (Speed > 50.0f) { Speed = 50.0f; }
}

//速さのセッター
void cNote::SetSpeed(const float _Speed) {
	Speed = _Speed;
}

//太さの加減算
void cNote::AddThickness(const int _Thickness) {
	Thickness += _Thickness;

	if (Thickness < 1) { Thickness = 1; }
	else if (Thickness > 15) { Thickness = 15; }
}

//太さのセッター
void cNote::SetThickness(const int _Thickness) {
	Thickness = _Thickness;
}

//BPMのセッター
void cNote::SetBpm(const float _Bpm) {
	Bpm = _Bpm;
}

//オートプレイフラグのセッター
void cNote::SetAutoPlayFlag(const bool _Flag) {
	AutoPlayFlag = _Flag;
}

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
			int fp = FileRead_open(SelectMusic.GetSelectMusicName());
			if (fp == 0) { return; }
			float TempBeat, TempScale;
			int TempLane, TempHoldFlag, TempHoldTime;
			while (FileRead_scanf(fp, "%f %f %d %d %d", &TempBeat, &TempScale, &TempLane, &TempHoldFlag, &TempHoldTime) != -1) {
				NotesList.push_back(cNote());
				(--NotesList.end())->Init(TempBeat, TempScale, TempLane, TempHoldFlag, TempHoldTime);
			}
			FileRead_close(fp);
			return;
		}



		//Y座標の確認
		bool CleckPosY = MouseY > POS_Y && MouseY < POS_Y + SIZE_Y;

		//Startボタンがクリックされたら、曲の再生開始
		if ((MouseX > POS_X && MouseX < POS_X + SIZE_X) && CleckPosY) {
			cNote::SetAutoPlayFlag(false);	//オートプレイフラグをON
		}
		//AutoPlayボタンがクリックされたら、曲の再生開始
		else if ((MouseX > POS_X2 && MouseX < POS_X2 + SIZE_X2) && CleckPosY) {
			cNote::SetAutoPlayFlag(true);	//オートプレイフラグをON
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


