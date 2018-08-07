#pragma once

//設定クラス
class cConfig {
private:
	const int BACK_POS_X = 40;
	const int BACK_POS_Y = 625;

	const int NEXT_PAGE_POS_X = 640;
	const int NEXT_PAGE_POS_Y = 625;

	const int SPEED_POS_X = 500;
	const int SPEED_POS_Y = 100;

	const int THICKNESS_POS_X = 420;
	const int THICKNESS_POS_Y = 300;

	float NotesPosY;
	float NotesWaitTime;

	unsigned char Page;	//ページ数
public:
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
};

