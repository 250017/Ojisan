#include "TestScene.h"
#include "Player.h"
#include "Ground.h"
#include "Block.h"
#include "Enemy.h"
//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{	
	//pWp = Instantiate<Weapon>(this);
	Player* pPlayer = Instantiate <Player>(this);
	Enemy* eEnemy = Instantiate <Enemy>(this);
	Ground* pGround = Instantiate <Ground>(this);
	Block* pBlock = Instantiate <Block>(this);
	pPlayer->SetBlock(pBlock);



}

//更新
void TestScene::Update()
{
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}
