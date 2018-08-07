#pragma once

//インクルード
#include"DxLib.h"
#include<stdio.h>
#include<list>
#include<string>
#include"Time.h"
#include"Sound.h"
#include"SelectMusic.h"
#include"Tap.h"
#include"Line.h"
#include"NotesData.h"
#include"TapNotes.h"
#include"HoldNotes.h"

//定数定義
const int SCREEN_WIDTH = 1280;			//画面の横幅
const int SCREEN_HEIGHT = 720;			//画面の縦幅

//画面の状態
enum SCREEN_STATE {
	SCREEN_TITLE = 1,
	SCREEN_SELECT_MUSIC,
	SCREEN_CONFIG,
	SCREEN_GAME
};

//グローバル変数宣言
SCREEN_STATE ScreenState = SCREEN_TITLE;	//画面状態
float FarstDelay = 3.04f;	//曲の再生遅延時間
int MouseCount = 0;


cTime Time;				//タイムオブジェクト
cSound Sound;			//サウンドオブジェクト
cSelectMusic SelectMusic;	//曲選択オブジェクト
cTap Tap;				//入力オブジェクト
cLine Line;				//線オブジェクト
const cNotesData NotesData;				//ノーツデータ
std::list<cTapNote> TapNotesList;		//TAPノーツリスト
std::list<cHoldNotes> HoldNotesList;	//HOLDノーツリスト

//関数のプロトタイプ宣言
bool ProcessLoop();	//メインループ処理
void Init();		//初期化
void Update();		//更新
void Draw();		//描画
void Uninit();		//後処理
void LoadChart(const char _MusicName[]);	//譜面の読み込み




