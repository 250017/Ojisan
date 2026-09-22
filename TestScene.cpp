#include "TestScene.h"
#include "Player.h"
#include "Ground.h"
#include "Block.h"
#include "Enemy.h"
#include "Food.h"
#include "Engine/Text.h"
#include <string>

namespace
{
	// プレイヤーのポインタを保持する
	Player* player = nullptr;

	// 敵のポインタを保持する
	Enemy* enemy = nullptr;

	// 地面のポインタを保持する
	Ground* ground = nullptr;

	// マップのポインタを保持する
	Block* block = nullptr;
}

// TestSceneを生成する
TestScene::TestScene(GameObject* parent)
	: GameObject(parent, "TestScene"),
	pText_(nullptr)
{}

// TestSceneを初期化する
void TestScene::Initialize()
{
	// プレイヤーを生成する
	player = Instantiate<Player>(this);

	// 敵を生成する
	enemy = Instantiate<Enemy>(this);

	// 地面を生成する
	ground = Instantiate<Ground>(this);

	// マップを生成する
	block = Instantiate<Block>(this);

	// プレイヤーへマップのポインタを渡す
	player->SetBlock(block);

	// テキスト描画クラスを生成する
	pText_ = new Text();

	// テキスト描画クラスを初期化する
	pText_->Initialize();
}

// TestSceneを更新する
void TestScene::Update()
{
	// 現在はシーン側で更新する処理がない
}

// TestSceneを描画する
void TestScene::Draw()
{
	// シーン切り替えによってPlayerが無効になっていないか確認する
	if (player == nullptr)
	{
		// Playerが無効なら描画処理を終了する
		return;
	}

	// シーン切り替えによってTextが無効になっていないか確認する
	if (pText_ == nullptr)
	{
		// Textが無効なら描画処理を終了する
		return;
	}

	// プレイヤーのスコアを文字列に変換する
	std::string scoreText =
		"SCORE:" + std::to_string(player->GetScore());

	// スコアを画面へ描画する
	//pText_->Draw(20, 20, scoreText.c_str());
}

void TestScene::Release()
{
	// Text本体が存在するか確認する
	if (pText_ != nullptr)
	{
		// Text本体を削除する
		delete pText_;

		// 解放済みのTextを使用しないようにする
		pText_ = nullptr;
	}

	// 古いシーンのPlayerを無効化する
	player = nullptr;

	// 古いシーンのEnemyを無効化する
	enemy = nullptr;

	// 古いシーンのGroundを無効化する
	ground = nullptr;

	// 古いシーンのBlockを無効化する
	block = nullptr;
}