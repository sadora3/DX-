#include "TapNotes.h"


//初期化
;//第一引数：何ビート目か
;//第二引数：SPBのスケーリング(0.0f～1.0f)。4分なら0、8分なら0.5、16分なら0.25 or 0.75
;//第三引数：レーンの番号(1～5)
void cTapNote::Init(const float _Beat, const float _Scale, const int _Lane) {
	//1ビートにつき何秒か
	float SPB = 60.0f / NotesData.GetBpm();

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

//更新
bool cTapNote::Update() {
	//タップ
	if (TapProc()) {
		return true;
	}

	//画面下部に到達してない場合
	if (PosY < SCREEN_HEIGHT || HoldFlag) {
		//ホールドノーツで画面下部にいった場合は、ミス判定を+1する
		if (PosY >= SCREEN_HEIGHT && !HoldFirstTap) {
			MissNum++;
			HoldFirstTap = true;
		}

		//落下
		PosY += Time.GetDeltaTime() * 65.0f * Speed;
	}
	else {
		//画面下部に来たら、殺す
		MissNum++;
		ComboNum = 0;
		return true;
	}

	return false;
}

//タップ
bool cTapNote::TapProc() {
	//タップタイムをカウントダウン
	TapTime -= Time.GetDeltaTime();

	//オートプレイフラグがONの場合、返る
	if (AutoPlayFlag) {
		bool ExistFlag = AutoTapProc();
		return ExistFlag;
	}

	//タップタイムがバッドよりもあった場合、返る
	if (TapTime > BAD_DECISION) { return false; }

	//ホールドの処理
	if (HoldFlag) {
		//セーフタイムを減少
		if (HoldSafeTime >= 0.0f) {
			HoldSafeTime -= Time.GetDeltaTime();
		}

		//1ビートにつき何秒か
		const float SPB = 60.0f / Bpm;
		//8分の判定
		float HoldTapTime = TapTime + (0.5f * HoldCount) * SPB;

		//タップされている間の処理
		bool TapSuccessFlag = false;
		for (int TapNum = 0; TapNum < Tap.GetMaxTapNum(); TapNum++) {
			//IDが-1の場合、次へ
			if (Tap.GetId(TapNum) == -1) { continue; }

			//タップされた場所が、ノーツと同じレーンだった場合
			if (Tap.GetPosX(TapNum) > PosX && Tap.GetPosX(TapNum) < PosX + SIZE) {
				//セーフタイムのセット
				HoldSafeTime = 0.5f;
			}

			//セーフタイムが残っている間
			if (HoldSafeTime > 0.0f) {
				//8分のタイミングでホールドされているか判定
				if (HoldTapTime < 0.01f && HoldTapTime > -0.01f) {
					TapSuccessFlag = true;
					PerfectNum++;
					ComboNum++;
					HoldCount++;
					break;
				}
			}
		}
		//タップされていない時の処理
		if (!TapSuccessFlag) {
			if (HoldTapTime < 0.01f && HoldTapTime > -0.01f) {
				MissNum++;
				ComboNum = 0;
				HoldCount++;
			}
		}

		//最後まで来てたら、殺す
		if (HoldCount > HoldTime) {
			Sound.PlayTap();
			return true;
		}
	}

	//タップされたら
	for (int TapNum = 0; TapNum < Tap.GetMaxTapNum(); TapNum++) {
		//IDが-1の場合、次へ
		if (Tap.GetId(TapNum) == -1) { continue; }

		//最初の1フレーム目だけ反応
		if (Tap.GetCount(TapNum) == 1) {
			//既にタップされてたら、返る(ホールドノーツ用)
			if (HoldFirstTap) { continue; }

			//タップされたレーンが違ったら、次へ
			if (Tap.GetPosX(TapNum) < PosX || Tap.GetPosX(TapNum) > PosX + SIZE) { continue; }

			//タップに成功したかどうか
			bool TapSuccess = false;

			//パーフェクトの判定
			if (TapTime <= PERFECT_DECISION && TapTime >= -PERFECT_DECISION) {
				PerfectNum++;
				ComboNum++;
				TapSuccess = true;
			}
			//早い・遅い
			else if (TapTime <= EARLY_DECISION && TapTime >= -EARLY_DECISION) {
				if (TapTime > 0) {
					EarlyNum++;
					ComboNum++;
					TapSuccess = true;
				}
				else {
					LateNum++;
					ComboNum++;
					TapSuccess = true;
				}
			}
			//バッド
			else if (TapTime <= BAD_DECISION && TapTime >= -BAD_DECISION) {
				BadNum++;
				ComboNum = 0;
				TapSuccess = true;
			}

			//タップに成功してた場合
			if (TapSuccess) {
				//タップ音再生
				Sound.PlayTap();

				//ホールドノーツであれば、最初のタップをONにする
				if (HoldFlag) {
					HoldFirstTap = true;
				}
				else {
					//タップノーツであれば、殺す
					return true;
				}
			}
		}
	}

	return false;
}

//オートタップ
bool cTapNote::AutoTapProc() {
	//タップタイムがバッドよりもあった場合、返る
	if (TapTime > BAD_DECISION) { return false; }

	//ホールドの処理
	if (HoldFlag) {
		//1ビートにつき何秒か
		const float SPB = 60.0f / Bpm;
		//8分の判定
		float HoldTapTime = TapTime + (0.5f * HoldCount) * SPB;

		//タップされている間の処理
		if (HoldTapTime <= 0.0f) {
			PerfectNum++;
			ComboNum++;
			HoldCount++;
		}

		//最後まで来たら、殺す
		if (HoldCount > HoldTime) {
			Sound.PlayTap();
			return true;
		}
	}

	//既にタップされてたら、返る(ホールドノーツ用)
	if (HoldFirstTap) { return false; }

	//オートタップ
	if (TapTime <= 0.0f) {
		PerfectNum++;
		ComboNum++;

		//タップ音再生
		Sound.PlayTap();

		//ホールドノーツであれば、最初のタップをONにする
		if (HoldFlag) {
			HoldFirstTap = true;
		}
		else {
			//タップノーツであれば、殺す
			return true;
		}
	}

	return false;
}

//描画
void cTapNote::Draw() {
	//画面に入らないノーツは、返る
	if (PosY < 0.0f) { return; }

	//ノーツの描画
	if (!HoldFlag) {
		//タップノーツの描画
		DrawLine(PosX, (int)PosY, PosX + SIZE, (int)PosY, 0xFF0000, Thickness);
	}
	else {
		//ホールドノーツの描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
		DrawBox(PosX, (int)PosY - HoldPosY, PosX + SIZE, (int)PosY, 0xF3F781, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
