#pragma once

#include "Engine/GameObject.h"

// リザルト画面を管理するクラス
class Result : public GameObject
{
public:
	// リザルト画面を生成する
	Result(GameObject* parent);

	// リザルト画面を初期化する
	void Initialize() override;

	// リザルト画面を更新する
	void Update() override;

	// リザルト画面を描画する
	void Draw() override;

	// リザルト画面で使用したデータを解放する
	void Release() override;

private:
	// リザルト画面を表示したフレーム数を保持する
	int frameCount_;
};