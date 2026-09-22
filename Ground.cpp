#include "Ground.h"
#include "Engine/Model.h"

Ground::Ground(GameObject* parent)
	:GameObject(parent, "Ground"),hModel_(-1)
{
	
}

//初期化
void Ground::Initialize()
{
	hModel_ = Model::Load("BG.fbx");
	assert(hModel_ >= 0);
	transform_.rotate_.x = 0.0f;
	transform_.position_.y = 20.0f;
	transform_.position_.x = 40.0f;
	transform_.scale_ = { 15.0f, 15.0f, 1.0 };
}

//更新
void Ground::Update()
{
}

//描画
void Ground::Draw()
{
	Model::SetAnimFrame;
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	Transform tr;
}

//解放
void Ground::Release()
{
}
