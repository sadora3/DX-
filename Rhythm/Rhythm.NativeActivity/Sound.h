#pragma once

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
