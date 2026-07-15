#include "Enemy.h"
#include "Engine/Model.h"

namespace
{
	enum Enemy_DIRECTION
	{
		Enemy_UP,
		Enemy_DOWN,
		Enemy_LEFT,
		Enemy_RIGHT,
		Enemy_DIRECTION_MAX//方向の数
	};
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent), hModel_(-1), hWalkModel_(-1), hIdleModel_(-1)
{
	hModel_ = Model::Load("Enemy.fbx");
}

void Enemy::Initialize()
{
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	transform_.scale_ = { 0.5, 0.5, 0.5 };
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
