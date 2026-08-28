#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "TestScene.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Block.h"
#include <cmath>
#include "Food.h"

namespace
{
	enum PLAYER_STATE
	{
		PLAYER_IDLE,
		PLAYER_WALK,
		PLAYER_TURN,//回る
		PLAYER_STATE_MAX //状態の数
	};
	PLAYER_STATE pstate = PLAYER_STATE::PLAYER_IDLE;

	enum PLAYER_DIRECTION
	{
		PLAYER_UP,
		PLAYER_DOWN,
		PLAYER_LEFT,
		PLAYER_RIGHT,
		PLAYER_DIRECTION_MAX//方向の数
	};
	
	PLAYER_DIRECTION pdirection = PLAYER_DOWN;
	float P_ANGLE[4] = { 180.0f,0.0f,90.0f, 270.0f };
	XMVECTOR P_MOVE[4] = { XMVectorSet(0, 0, 1, 0),
						   XMVectorSet(0, 0, -1, 0),
						   XMVectorSet(-1, 0, 0, 0),
						   XMVectorSet(1, 0, 0, 0)};//プレイヤーの武器に応じた移動ベクトルを格納する配列
	float TURN_FRAME = 30.0f;//回転にかかるフレーム数

	float turnStartAngle = 0.0f;//開始角度
	float turnEndAngle = 0.0f;//終了角度
	PLAYER_DIRECTION turnEndDirection = PLAYER_DOWN;
	float AdjustAngle(float angle) //角度の差が少ないほうに回転
	{
		if (angle > 180.0f)
		{
			angle -= 360.0f;
		}

		if (angle < -180.0f)
		{
			angle += 360.0f;
		}

		return angle;
	}


	//プレイヤーがマップのどこにいるか
	int mapX = 0;
	int mapZ = 0;


	Food* food;
}
//namespace
//{
//	const XMVECTOR vFront = { 0, 0, 1, 0 };//タンク前のベクトル
//	const float moveSpeed = 0.1f;//タンクの移動速度
//	const float CAM_HEIGHT_BIAS = 0.2f;//カメラの高さの調整値
//}

Player::Player(GameObject* parent)
	:GameObject(parent), hWalkModel_(-1), hIdleModel_(-1){
	//swordDirには、初期方向として、ローカルモデルの剣の根っこから
	//先端までのベクトルとして（0,1,0)を代入しておく
	//初期位置は原点
}

void Player::Initialize()
{
	hWalkModel_ = Model::Load("Walking.fbx");
	hIdleModel_ = Model::Load("idle.fbx");
	Model::SetAnimFrame(hWalkModel_, 0, 59, 1.0);
	Model::SetAnimFrame(hIdleModel_, 0, 117, 1.0);
	Camera::SetPosition({ 0, 20, -40 });
	Camera::SetTarget({ 0, 0, 0 });
	transform_.position_ = { 2.0f, 0.0f, 0.0f };


	//スコアの初期化
	score_ = 0;
}

void Player::Update()
{
	//XMVECTOR vPos = XMLoadFloat3(&transform_.position_);//ロード：書き込み
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));//Y軸回転行列を作る

	//XMVECTOR vMove = XMVector3TransformCoord(vFront, matRot);
	
	//transform_.rotate_.y +=1;
	//static float angle = 0.0;
	//angle = angle + 0.3f;
	//XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//XMMATRIX rotateX = XMMatrixRotationX(XMConvertToRadians(angle));
	//XMMATRIX rotate = XMMatrixRotationY(XMConvertToRadians(angle));
	//XMMATRIX translate = XMMatrixTranslation(1.0f, 0.0f, 0.0f);

	//SetWorldMatrix(scale *  rotate * translate);

	//XMVECTOR front = { 0, 0, 1, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));//Y軸回転行列を作る
	//XMVECTOR move = XMVector3TransformCoord(front, matRot);
	XMVECTOR move = XMVectorSet( 0, 0, 0, 0 );
	const float SPEED = 0.1f;
	static float turnFrame = 0.0f;//回転中のフレーム数を管理する変数
	float angle = 0.0f;
	if (pstate != PLAYER_STATE::PLAYER_TURN) {
		pstate = PLAYER_IDLE;

	}

	PLAYER_DIRECTION oldDir = pdirection; //pdirection <= 今の向き


	

	if (pstate != PLAYER_STATE::PLAYER_TURN)
	{
		if (Input::IsKey(DIK_LEFT))
		{
			pdirection = PLAYER_DIRECTION::PLAYER_LEFT;
			pstate = PLAYER_STATE::PLAYER_WALK;
		}
		if (Input::IsKey(DIK_RIGHT))
		{
			pdirection = PLAYER_DIRECTION::PLAYER_RIGHT;
			pstate = PLAYER_STATE::PLAYER_WALK;
		}
		if (Input::IsKey(DIK_UP))
		{
			pdirection = PLAYER_DIRECTION::PLAYER_UP;
			pstate = PLAYER_STATE::PLAYER_WALK;
		}
		if (Input::IsKey(DIK_DOWN))
		{
			pdirection = PLAYER_DIRECTION::PLAYER_DOWN;
			pstate = PLAYER_STATE::PLAYER_WALK;
		}

	}
	if (oldDir != pdirection) {
		pstate = PLAYER_STATE::PLAYER_TURN;
		turnFrame = 0.0f;
		turnStartAngle = transform_.rotate_.y;

		turnEndDirection = pdirection;//入力方向に30フレームで回転する
		turnEndAngle = P_ANGLE[turnEndDirection];
	}

	if (pstate == PLAYER_STATE::PLAYER_TURN)
	{
		turnFrame += 1.0f;
		float t = turnFrame / TURN_FRAME; //0.0～1.0
		//30フレームで回転
		//回転中の処理
		if (t > 1.0f)
		{
			t = 1.0f; //1.0を超えないようにする
		}

		//開始角度から終了角度までの差
		float angleDifference = turnEndAngle - turnStartAngle;

		// -180～180度にして、最短方向の角度差にする
		angleDifference = AdjustAngle(angleDifference);

		// 最短方向へ補間
		float angle = turnStartAngle + angleDifference * t;

		transform_.rotate_.y = angle;
		if (turnFrame >= TURN_FRAME) {
			pdirection = turnEndDirection;
			transform_.rotate_.y = P_ANGLE[pdirection];
			pstate = PLAYER_STATE::PLAYER_WALK;
		}
		return;
	}
	else if (pstate != PLAYER_STATE::PLAYER_IDLE)
	{
		move = P_MOVE[pdirection];
		angle = P_ANGLE[pdirection];
		transform_.rotate_.y = angle;
	}
	XMVECTOR pos = XMLoadFloat3(&transform_.position_);
	pos = pos + SPEED * move;
	XMStoreFloat3(&transform_.position_, pos);
	XMFLOAT3 wpos = transform_.position_;
	////壁オブジェクトに食い込んでたら戻す
	std::vector<std::vector<int>> gmap = block_->GetMapData();
	// プレイヤーがいるマス番号
	
	if (wpos.x >= 0) mapX = (int)(wpos.x / 4) + 5;//+5 は調整した値　4はブロックの幅
	else mapX = (int)(wpos.x) / 4 + 4;
	if (wpos.z >= 0) mapZ = abs((int)((wpos.z + 2) / 4) -4);
	else mapZ = (abs((int)(wpos.z / 4))) + 5;

	// 壁判定
	if (gmap[mapZ][mapX] == 1)    // 1が壁の場合
	{
		pos = pos - SPEED * move;
		XMStoreFloat3(&transform_.position_, pos);

	}
	if (gmap[mapZ][mapX] == 2)
	{
		Food* food = block_->GetFood(mapZ, mapX);
		if (food != nullptr)
		{
			//スコアを足してオブジェクトを削除する
			score_ += block_->RemoveFood(mapZ, mapX);;
			food = nullptr;
		}
	}


	//Debug::Log("score_ = ");
	//Debug::Log(score_, true);
	
	//壁オブジェクトに食い込んでたら戻す！
	

}

void Player::Draw()
{
	transform_.scale_ = { 1,1,1 };

	if (pstate == PLAYER_STATE::PLAYER_IDLE)
	{
		Model::SetTransform(hIdleModel_, transform_);
		Model::Draw(hIdleModel_);
	}
	else if (pstate == PLAYER_STATE::PLAYER_WALK || pstate == PLAYER_STATE::PLAYER_TURN) {
		Model::SetTransform(hWalkModel_, transform_);
		Model::Draw(hWalkModel_);
	}
	
}


void Player::Release()
{
}

int Player::GetScore()
{
	return score_;
}
