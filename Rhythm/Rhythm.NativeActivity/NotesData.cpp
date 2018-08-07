#include "NotesData.h"
#include"DxLib.h"

//判定数の描画
void cNotesData::DecisionNumDraw() {
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
float cNotesData::GetSpeed() const {
	return Speed;
}

//太さのゲッター
int cNotesData::GetThickness() const {
	return Thickness;
}

//BPMのゲッター
float cNotesData::GetBpm() const {
	return Bpm;
}

//速さの加減算
void cNotesData::AddSpeed(const float _Speed) {
	Speed += _Speed;

	if (Speed < 1.0f) { Speed = 1.0f; }
	else if (Speed > 50.0f) { Speed = 50.0f; }
}

//速さのセッター
void cNotesData::SetSpeed(const float _Speed) {
	Speed = _Speed;
}

//太さの加減算
void cNotesData::AddThickness(const int _Thickness) {
	Thickness += _Thickness;

	if (Thickness < 1) { Thickness = 1; }
	else if (Thickness > 15) { Thickness = 15; }
}

//太さのセッター
void cNotesData::SetThickness(const int _Thickness) {
	Thickness = _Thickness;
}

//BPMのセッター
void cNotesData::SetBpm(const float _Bpm) {
	Bpm = _Bpm;
}

//オートプレイフラグのセッター
void cNotesData::SetAutoPlayFlag(const bool _Flag) {
	AutoPlayFlag = _Flag;
}
