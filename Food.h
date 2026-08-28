#pragma once
#include "Engine/GameObject.h"

enum FoodType
{
	FOODTYPE_NORMAL,
	FOODTYPE_POWER,
	FOODTYPE_MAX
};

class Food :
	public GameObject
{
public:
	Food(GameObject* parent);
	~Food();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release()override;
	void SetFoodType(FoodType type);
	void OnCollision(GameObject* pTarget) override;
	int GetScore();
	bool IsHitPlayer();
private:
	FoodType type_;
	int hModel_;
	int score_;
	bool hitfood_;

};