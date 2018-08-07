#include "Line.h"

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
