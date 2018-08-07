#pragma once

#include"Main.h"

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

