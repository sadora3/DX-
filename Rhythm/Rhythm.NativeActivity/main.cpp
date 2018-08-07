//インクルード
#include"Main.h"
#include"Fps.h"
#include"Title.h"
#include"Config.h"
#include"MusicStartButton.h"

cFps Fps;		//Fpsオブジェクト
cTitle Title;	//タイトルオブジェクト
cConfig Config;	//設定オブジェクト
cMusicStartButton MusicStartButton;		//曲再生ボタン

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
	char FilePath[256];
	GetInternalDataPath(FilePath, sizeof(FilePath));
	strcat(FilePath, "/config.txt");
	int fp = FileRead_open(FilePath);
	if (fp == 0) { return; }
	float TempSpeed;
	int TempThickness;
	int TempTapSeVolume;
	if (FileRead_scanf(fp, "%f", &TempSpeed) != -1) { cTapNote::SetSpeed(TempSpeed); }
	if (FileRead_scanf(fp, "%d", &TempThickness) != -1) { cTapNote::SetThickness(TempThickness); }
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
			cTapNote::DecisionNumDraw();

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

//譜面の読み込み
void LoadChart(const char _MusicName[]) {
	int fp = FileRead_open(_MusicName);
	if (fp == 0) { return; }
	float TempBeat, TempScale;
	int TempLane, TempHoldFlag, TempHoldTime;
	while (FileRead_scanf(fp, "%d %f %f %d %d", &TempHoldFlag, &TempBeat, &TempScale, &TempLane, &TempHoldTime) != -1) {
		NotesList.push_back(cTapNote());
		(--NotesList.end())->Init(TempBeat, TempScale, TempLane, TempHoldFlag, TempHoldTime);
	}
	FileRead_close(fp);
}

