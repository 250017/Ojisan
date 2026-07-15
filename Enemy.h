#pragma once
#pragma once
#include "Engine/GameObject.h"

class Block;


class Enemy :
	public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Enemy(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

private:
	int hModel_;
	int hWalkModel_;//歩きアニメーションのモデルハンドル
	int hIdleModel_;//待機アニメーションのモデルハンドル

	Block* block_;//地面オブジェクトのポインタ
};

