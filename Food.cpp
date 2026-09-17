#include "Food.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"

Food::Food(GameObject* parent)
	:GameObject(parent, "Food"), type_(FoodType::FOODTYPE_NORMAL), hModel_(-1), score_(0)
{ 
}

Food::~Food()
{
}

void Food::Initialize()
{
	score_ = 5;

	hitfood_ = false;
}

void Food::Update()
{
}

void Food::Draw()
{
	if (IsDead())
	{
		return;
	}

	switch (type_)
	{
	case FoodType::FOODTYPE_NORMAL:
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
		break;
	}
	
}


void Food::Release()
{
}

void Food::SetFoodType(FoodType type)
{
	type_ = type;
	if (type_ == FoodType::FOODTYPE_NORMAL)
	{
		hModel_ = Model::Load("esa.fbx");
		score_ = 10;
	}
	else if (type_ == FoodType::FOODTYPE_POWER)
	{
		hModel_ = Model::Load("poweresa.fbx");
		score_ = 20;
	}
}

void Food::OnCollision(GameObject* pTarget)
{
}

int Food::GetScore()
{
	Debug::Log("score_ = ");
	Debug::Log(score_, true);
	return score_;
}

bool Food::IsHitPlayer()
{
	hitfood_ = true;;
	return hitfood_;
}
