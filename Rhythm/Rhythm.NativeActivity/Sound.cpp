#include"Sound.h"
#include"DxLib.h"

//初期化
void cSound::Init() {
	//BGMの音量調整
	ChangeVolumeSoundMem(168, BgmSh);
	//タップ音の読み込み
	TapSh = LoadSoundMem("data/TAP.wav");
}

//BGMの再生
void cSound::PlayBgm() {
	//BGMが未再生の場合、再生
	if (!BgmPlayFlag) {
		BgmPlayFlag = true;
		PlaySoundMem(BgmSh, DX_PLAYTYPE_BACK);
	}
}

//タップ音の再生
void cSound::PlayTap() {
	PlaySoundMem(TapSh, DX_PLAYTYPE_BACK);
}

//後処理
void cSound::Uninit() {
	DeleteSoundMem(BgmSh);
	DeleteSoundMem(TapSh);
}

//タップ音のボリュームのゲッター
int cSound::GetTapSeVolume() const {
	return TapSeVolume;
}

//タップ音のボリュームの加減算
void cSound::AddTapSeVolume(int _Val) {
	if (_Val == 1 && TapSeVolume < 255) {
		TapSeVolume += _Val;
	}
	else if (_Val == -1 && TapSeVolume > 1) {
		TapSeVolume += _Val;
	}
	else if (_Val == 10) {
		if (TapSeVolume < 244) {
			TapSeVolume += _Val;
		}
		else {
			TapSeVolume = 255;
		}
	}
	else if (_Val == -10) {
		if (TapSeVolume > 10) {
			TapSeVolume += _Val;
		}
		else {
			TapSeVolume = 0;
		}
	}

	//ボリュームの反映
	ChangeVolumeSoundMem(TapSeVolume, TapSh);
}

//タップ音のボリュームのセッター
void cSound::SetTapSeVolume(int _Val) {
	TapSeVolume = _Val;

	//ボリュームの反映
	ChangeVolumeSoundMem(TapSeVolume, TapSh);
}

//BGMのセッター
void cSound::SetBgm(const char* _BgmName) {
	//BGMの読み込み
	BgmSh = LoadSoundMem(_BgmName);
}

