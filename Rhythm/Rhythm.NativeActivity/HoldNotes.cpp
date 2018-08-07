#include "HoldNotes.h"



//初期化
;//第一引数：何ビート目か
;//第二引数：SPBのスケーリング(0.0f～1.0f)。4分なら0、8分なら0.5、16分なら0.25 or 0.75
;//第三引数：レーンの番号(1～5)
;//第四引数：ホールドフラグ
;//第五引数：ホールドタイム(1～。1の場合8分1個。10の場合8分10個)
void cHoldNote::Init(const float _Beat, const float _Scale, const int _Lane, const bool _HoldFlag, const int _HoldTime) {
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


