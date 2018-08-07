#pragma once

//ノーツデータクラス
class cNotesData {
private:
	const int SIZE = Line.GetLaneInterval();	//大きさ
	const float PERFECT_DECISION = 0.05f;	//パーフェクト判定
	const float EARLY_DECISION = 0.1f;		//早い判定
	const float BAD_DECISION = 0.15f;		//ナイス判定

	float Speed;		//速さ
	int Thickness;		//太さ
	float Bpm;			//ビート/分
	short PerfectNum;	//パーフェクトの数
	short EarlyNum;		//早いの数
	short LateNum;		//遅いの数
	short BadNum;		//バッドの数
	short MissNum;		//ミスの数
	short ComboNum;		//コンボの数
	bool AutoPlayFlag;	//オートプレイフラグ
public:
	cNotesData() : Speed(8.0f), Thickness(3) {}

	void DecisionNumDraw();	//判定数の描画

	float GetSpeed() const;		//速さのゲッター
	int GetThickness() const;		//太さのゲッター
	float GetBpm() const;		//BPMのゲッター

	void AddSpeed(const float _Speed);	//速さの加減算
	void SetSpeed(const float _Speed);	//速さのセッター
	void AddThickness(const int _Thickness);	//太さの加減算
	void SetThickness(const int _Thickness);	//太さのセッター
	void SetBpm(const float _Bpm);	//BPMのセッター
	void SetAutoPlayFlag(const bool _Flag);	//オートプレイフラグのセッター
};

