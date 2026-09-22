#include "Result.h"
#include "Engine/SceneManager.h"
#include "Engine/Text.h"

namespace
{
	// ゲームの1秒間のフレーム数を設定する
	const int FRAME_RATE = 60;

	// リザルト画面を表示する秒数を設定する
	const int RESULT_SECONDS = 3;

	// シーンを切り替えるフレーム数を計算する
	const int CHANGE_FRAME = FRAME_RATE * RESULT_SECONDS;
	//Text* text;

	int textX;
	int textY;
}

// リザルト画面を生成する
Result::Result(GameObject* parent)
	: GameObject(parent, "Result"),
	frameCount_(0)
{}

// リザルト画面を初期化する
void Result::Initialize()
{
	// 経過フレーム数を0に戻す
	frameCount_ = 0;
	//text->Initialize();
	textX = 0.0;
	textY = 0.0;
}

// リザルト画面を更新する
void Result::Update()
{
	// 経過フレーム数を1増やす
	frameCount_++;

	// 3秒経過したか確認する
	if (frameCount_ >= CHANGE_FRAME)
	{
		// TestSceneへシーンを切り替える
		SceneManager* pSceneManager = (SceneManager*)(this->GetParent());
		pSceneManager->ChangeScene(SCENE_ID_TEST);
	}
}

// リザルト画面を描画する
void Result::Draw()
{
	//text->Draw(textX, textY, ("GAMEOVER"));
}

// リザルト画面で使用したデータを解放する
void Result::Release()
{
	// 解放するデータがないため、ここでは何もしない
}