#pragma once

//インクルード
#include"DxLib.h"
#include<stdio.h>
#include<list>
#include<string>

//定数定義
const int SCREEN_WIDTH = 1280;			//画面の横幅
const int SCREEN_HEIGHT = 720;			//画面の縦幅

enum SCREEN_STATE {
	SCREEN_TITLE = 1,
	SCREEN_SELECT_MUSIC,
	SCREEN_CONFIG,
	SCREEN_GAME
};

//FPSクラス
class cFps {
private:
	int StartTime;      //測定開始時刻
	int Count;          //カウンタ
	float Fps;          //fps
	const int N = 60;	//平均を取るサンプル数
	const int FPS = 60;	//設定したFPS
public:
	void Update();	//更新
	void Draw();	//描画
	void Wait();	//制御
};

//タイムクラス
class cTime {
private:
	int OldTime;	//1ループ前の時間
	int NowTime;	//現在の時間
	int DeltaTime;	//前のループから、何秒経ったか
public:
	void Init();				//初期化
	void Update();				//更新
	float GetDeltaTime() const;	//デルタタイムのゲッター
};

//サウンドクラス
class cSound {
private:
	int BgmSh;			//BGMのサウンドハンドル
	int TapSh;			//タップ音のサウンドハンドル
	bool BgmPlayFlag;	//BGMの再生フラグ
	int TapSeVolume;	//タップ音のボリューム
public:
	void Init();		//初期化
	void PlayBgm();		//BGMの再生
	void PlayTap();		//タップ音の再生
	void Uninit();		//後処理

	int GetTapSeVolume() const;	//タップ音のボリュームのゲッター

	void AddTapSeVolume(int _Val);		//タップ音のボリュームの加減算
	void SetTapSeVolume(int _Val);		//タップ音のボリュームのセッター
	void SetBgm(const char* _BgmName);
};

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

//設定クラス
class cConfig {
private:
	const int BACK_POS_X = 40;
	const int BACK_POS_Y = 625;

	const int NEXT_PAGE_POS_X = 640;
	const int NEXT_PAGE_POS_Y = 625;

	const int SPEED_POS_X = 500;
	const int SPEED_POS_Y = 100;

	const int THICKNESS_POS_X = 420;
	const int THICKNESS_POS_Y = 300;

	float NotesPosY;
	float NotesWaitTime;

	unsigned char Page;	//ページ数
public:
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
};

//タップクラス
class cTap {
private:
	static const char MAX_TAP_NUM = 5;	//最大タップ数

	int TouchNum;				//タッチされた数
	short Count[MAX_TAP_NUM];	//カウント
	int PosX[MAX_TAP_NUM];		//座標X
	int Id[MAX_TAP_NUM];		//ID
public:
	void Init();	//初期化
	void Update();	//更新

	const char GetMaxTapNum()const;	//最大タップ数のゲッター
	int GetTouchNum()const;			//タッチされた数のゲッター
	short GetCount(const int _i)const;	//カウントのゲッター
	int GetPosX(const int _i)const;		//座標Xのゲッター
	int GetId(const int _i)const;			//IDのゲッター
};

//線に関するもの全般クラス
class cLine {
private:
	//判定ライン
	const int DECISION_LINE_WIDTH = 1200;	//判定ラインの横幅
	const int DECISION_LINE_Y = 650;		//判定ラインのY座標
	const int DECISION_LINE_X1 = (int)(SCREEN_WIDTH * 0.5 - DECISION_LINE_WIDTH * 0.5);	//判定ラインの左のX座標
	const int DECISION_LINE_X2 = (int)(SCREEN_WIDTH * 0.5 + DECISION_LINE_WIDTH * 0.5);	//判定ラインの右のX座標

																						//レーン
	const int LANE_NUM = 5;	//レーンの数
	const int LANE_INTERVAL = DECISION_LINE_WIDTH / LANE_NUM;	//レーンの間隔
public:
	void Draw();	//描画

	int GetDecisionLineX1()const;	//判定ラインの左のX座標のゲッター
	int GetLaneInterval()const;		//レーンの間隔のゲッター
};

//ノーツクラス
class cNote {
private:
	static const int SIZE;	//大きさ
	static const float PERFECT_DECISION;	//パーフェクト判定
	static const float EARLY_DECISION;		//早い判定
	static const float BAD_DECISION;		//ナイス判定

	static float Speed;			//速さ
	static int Thickness;		//太さ
	static float Bpm;			//ビート/分
	static short PerfectNum;	//パーフェクトの数
	static short EarlyNum;		//早いの数
	static short LateNum;		//遅いの数
	static short BadNum;		//バッドの数
	static short MissNum;		//ミスの数
	static short ComboNum;		//コンボの数
	static bool AutoPlayFlag;	//オートプレイフラグ

	float TapTime;	//タップタイム(0になった瞬間にタップするとPEFECT)
	int PosX;		//X座標
	float PosY;		//Y座標

	bool HoldFlag;		//ホールドフラグ(これがONのノーツはホールドノーツになる)
	int HoldTime;		//ホールド時間(1～。1の場合8分1個。10の場合8分10個)
	int HoldPosY;		//ホールドノーツの長さ
	bool HoldFirstTap;	//ホールドノーツの最初のタップがされたかどうか
	int HoldCount;		//ホールドの判定カウント
	float HoldSafeTime;	//ホールド後、指を離しても大丈夫な時間

	bool TapProc();		//タップ
	bool AutoTapProc();	//オートタップ
public:
	void Init(const float _Beat, const float _Scale, const int _Lane, const bool _HoldFlag, const int _HoldTime);	//初期化
	bool Update();	//更新
	void Draw();	//描画

	static void DecisionNumDraw();	//判定数の描画

	static float GetSpeed();		//速さのゲッター
	static int GetThickness();		//太さのゲッター

	static void AddSpeed(const float _Speed);	//速さの加減算
	static void SetSpeed(const float _Speed);	//速さのセッター
	static void AddThickness(const int _Thickness);	//太さの加減算
	static void SetThickness(const int _Thickness);	//太さのセッター
	static void SetBpm(const float _Bpm);	//BPMのセッター
	static void SetAutoPlayFlag(const bool _Flag);	//オートプレイフラグのセッター
};

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

//グローバル変数宣言
SCREEN_STATE ScreenState = SCREEN_TITLE;	//画面状態
float FarstDelay = 3.04f;	//曲の再生遅延時間

cFps Fps;				//Fpsオブジェクト
cTime Time;				//タイムオブジェクト
cSound Sound;			//サウンドオブジェクト
cTitle Title;			//タイトルオブジェクト
cSelectMusic SelectMusic;	//曲選択オブジェクト
cConfig Config;			//設定オブジェクト
cTap Tap;				//入力オブジェクト
cLine Line;				//線オブジェクト
std::list<cNote> NotesList;	//ノーツデータリスト
cMusicStartButton MusicStartButton;	//曲再生ボタン

const int cNote::SIZE = Line.GetLaneInterval();	//大きさ
const float cNote::PERFECT_DECISION = 0.05f;		//パーフェクト判定
const float cNote::EARLY_DECISION = 0.1f;			//早い判定
const float cNote::BAD_DECISION = 0.15f;			//バッド判定
float cNote::Speed = 8.0f;		//速さ
int cNote::Thickness = 3;		//太さ
float cNote::Bpm;				//ビート/分
short cNote::PerfectNum = 0;	//パーフェクトの数
short cNote::EarlyNum = 0;		//早いの数
short cNote::LateNum = 0;		//遅いの数
short cNote::BadNum = 0;		//バッドの数
short cNote::MissNum = 0;		//ミスの数
short cNote::ComboNum = 0;		//コンボの数
bool cNote::AutoPlayFlag;		//オートプレイフラグ

;//関数のプロトタイプ宣言
bool ProcessLoop();	//メインループ処理
void Init();		//初期化
void Update();		//更新
void Draw();		//描画
void Uninit();		//後処理




