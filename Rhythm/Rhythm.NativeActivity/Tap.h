#pragma once

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

