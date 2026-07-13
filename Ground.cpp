#include "Ground.h"
#include "Engine/Model.h"

Ground::Ground(GameObject* parent)
	:GameObject(parent, "Ground"),hModel_(-1)
{
	
}

//初期化
void Ground::Initialize()
{
	hModel_ = Model::Load("Ground.fbx");
	assert(hModel_ >= 0);
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
