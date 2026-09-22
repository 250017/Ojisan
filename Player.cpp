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
		PLAYER_DIRECTION_MAX,//方向の数
		
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
	int mapY = 0;

	float GRAVITY;
	float jampSpeed;


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

	IsJamp_ = false;

	GRAVITY = -0.3f;
	jampSpeed = GRAVITY;
}

void Player::Update()
{
	// プレイヤーの移動速度を設定する
	const float SPEED = 0.3f;

	// 回転中の経過フレームを保持する
	static float turnFrame = 0.0f;

	// 今回のフレームで使用する移動ベクトルを初期化する
	XMVECTOR move = XMVectorZero();

	// 移動前の座標を保存する
	XMFLOAT3 oldPosition = transform_.position_;

	// 重力を適用する前のY座標を保存する
	float oldY = transform_.position_.y;

	// 回転中でなければ待機状態へ戻す
	if (pstate != PLAYER_TURN)
	{
		// プレイヤーを待機状態に設定する
		pstate = PLAYER_IDLE;
	}

	// 入力前の向きを保存する
	PLAYER_DIRECTION oldDirection = pdirection;

	// 回転中でなければキー入力を受け付ける
	if (pstate != PLAYER_TURN)
	{
		// 左キーが押されているか確認する
		if (Input::IsKey(DIK_LEFT))
		{
			// プレイヤーを左向きにする
			pdirection = PLAYER_LEFT;

			// プレイヤーを歩行状態にする
			pstate = PLAYER_WALK;
		}

		// 右キーが押されているか確認する
		if (Input::IsKey(DIK_RIGHT))
		{
			// プレイヤーを右向きにする
			pdirection = PLAYER_RIGHT;

			// プレイヤーを歩行状態にする
			pstate = PLAYER_WALK;
		}

		// 上キーが押されているか確認する
		if (Input::IsKey(DIK_UP))
		{
			// プレイヤーを上向きにする
			pdirection = PLAYER_UP;

			// プレイヤーを歩行状態にする
			pstate = PLAYER_WALK;
		}

		// 下キーが押されているか確認する
		if (Input::IsKey(DIK_DOWN))
		{
			// プレイヤーを下向きにする
			pdirection = PLAYER_DOWN;

			// プレイヤーを歩行状態にする
			pstate = PLAYER_WALK;
		}
	}

	// 入力によって向きが変わったか確認する
	if (oldDirection != pdirection)
	{
		// プレイヤーを回転状態にする
		pstate = PLAYER_TURN;

		// 回転フレームを初期化する
		turnFrame = 0.0f;

		// 現在の角度を回転開始角度として保存する
		turnStartAngle = transform_.rotate_.y;

		// 入力された方向を回転終了後の方向として保存する
		turnEndDirection = pdirection;

		// 回転終了後の角度を取得する
		turnEndAngle = P_ANGLE[turnEndDirection];
	}

	// プレイヤーが回転中か確認する
	if (pstate == PLAYER_TURN)
	{
		// 回転フレームを進める
		turnFrame += 1.0f;

		// 回転の進行度を計算する
		float t = turnFrame / TURN_FRAME;

		// 回転の進行度が1.0を超えないようにする
		if (t > 1.0f)
		{
			// 回転の進行度を1.0に固定する
			t = 1.0f;
		}

		// 開始角度と終了角度の差を計算する
		float angleDifference = turnEndAngle - turnStartAngle;

		// 最短方向へ回転するように角度差を調整する
		angleDifference = AdjustAngle(angleDifference);

		// 現在の回転角度を補間して求める
		transform_.rotate_.y =
			turnStartAngle + angleDifference * t;

		// 回転が完了したか確認する
		if (turnFrame >= TURN_FRAME)
		{
			// 回転終了後の方向を設定する
			pdirection = turnEndDirection;

			// 回転終了後の角度を設定する
			transform_.rotate_.y = P_ANGLE[pdirection];

			// プレイヤーを歩行状態にする
			pstate = PLAYER_WALK;
		}
	}
	else if (pstate == PLAYER_WALK)
	{
		// 現在向いている方向の移動ベクトルを取得する
		move = P_MOVE[pdirection];

		// 現在向いている方向の回転角度を設定する
		transform_.rotate_.y = P_ANGLE[pdirection];
	}

	// 現在の座標をXMVECTORへ変換する
	XMVECTOR position = XMLoadFloat3(&transform_.position_);

	// 入力された方向へプレイヤーを移動させる
	position = position + SPEED * move;

	// 移動後の座標をプレイヤーへ戻す
	XMStoreFloat3(&transform_.position_, position);

	// 重力を適用する直前のY座標を保存する
	oldY = transform_.position_.y;

	//ジャんぷ処理
	if (Input::IsKeyDown(DIK_SPACE) && IsJamp_ != true);
	{
		jampSpeed = 2.0f;
		IsJamp_ = true;
	}
	if (jampSpeed >= GRAVITY)
	{
		jampSpeed += GRAVITY;
	}
	// プレイヤーを重力で下方向へ移動させる
	transform_.position_.y += jampSpeed;

	// Blockのポインタが有効か確認する
	if (block_ == nullptr)
	{
		// Blockが取得できていない場合は処理を終了する
		return;
	}

	// 現在のマップデータを取得する
	std::vector<std::vector<int>> gmap = block_->GetMapData();

	// マップデータが空か確認する
	if (gmap.empty())
	{
		// マップデータがなければ処理を終了する
		return;
	}

	// プレイヤーの現在座標を取得する
	XMFLOAT3 worldPosition = transform_.position_;

	// X座標が0以上か確認する
	if (worldPosition.x >= 0.0f)
	{
		// 正のX座標からマップの列番号を計算する
		mapX = static_cast<int>(worldPosition.x / 4.0f) + 5;
	}
	else
	{
		// 負のX座標からマップの列番号を計算する
		mapX = static_cast<int>(worldPosition.x / 4.0f) + 4;
	}

	// Y座標が0以上か確認する
	if (worldPosition.y >= 0.0f)
	{
		// 正のY座標からマップの行番号を計算する
		mapY = std::abs(
			static_cast<int>((worldPosition.y + 2.0f) / 4.0f) - 4
		);
	}
	else
	{
		// 負のY座標からマップの行番号を計算する
		mapY =
			std::abs(static_cast<int>(worldPosition.y / 4.0f)) + 5;
	}

	// マップの行番号が範囲外か確認する
	if (mapY < 0 ||
		mapY >= static_cast<int>(gmap.size()))
	{
		// Y座標だけを重力適用前の位置へ戻す
		transform_.position_.y = oldY;

		// 範囲外のvectorへアクセスせず処理を終了する
		return;
	}

	// マップの列番号が範囲外か確認する
	if (mapX < 0 ||
		mapX >= static_cast<int>(gmap[mapY].size()))
	{
		// X座標だけを移動前の位置へ戻す
		transform_.position_.x = oldPosition.x;

		// 範囲外のvectorへアクセスせず処理を終了する
		return;
	}

	// プレイヤーがいるマスの番号を取得する
	int mapChip = gmap[mapY][mapX];

	// 現在のマスが地面または壁か確認する
	if (mapChip == 1)
	{
		// Y座標だけを重力適用前の位置へ戻す
		transform_.position_.y = oldY;
		IsJamp_ = false;
	}
	else if (mapChip == 2)
	{
		// 現在のマスに置かれているFoodを取得する
		Food* currentFood = block_->GetFood(mapY, mapX);

		// Foodが存在するか確認する
		if (currentFood != nullptr)
		{
			// Foodを削除してスコアを加算する
			score_ += block_->RemoveFood(mapY, mapX);
		}
	}

	// デバッグ画面へmapXの見出しを表示する
	Debug::Log("mapX = ");

	// 現在のmapXを表示する
	Debug::Log(mapX, true);

	// デバッグ画面へmapYの見出しを表示する
	Debug::Log("mapY = ");

	// 現在のmapYを表示する
	Debug::Log(mapY, true);
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
