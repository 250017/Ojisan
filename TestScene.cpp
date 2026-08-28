#include "TestScene.h"
#include "Player.h"
#include "Ground.h"
#include "Block.h"
#include "Enemy.h"
#include "Food.h"
#include "Engine/Text.h"

namespace {
	Player* player;
	Enemy* enemy;
	Ground* ground;
	Block* block;
}


//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{	
	//pWp = Instantiate<Weapon>(this);
	player = Instantiate <Player>(this);
	enemy = Instantiate <Enemy>(this);
	ground = Instantiate <Ground>(this);
	block = Instantiate <Block>(this);
	player->SetBlock(block);

	pText_ = new Text;
	pText_->Initialize();

}

//更新
void TestScene::Update()
{
}

//描画
void TestScene::Draw()
{
	std::string scrText;
	scrText = "SCORE;" + std::to_string(player->GetScore());
	pText_->Draw(20, 20, scrText.c_str());
}

//開放
void TestScene::Release()
{
	pText_->Release();//テキストの開放
}
