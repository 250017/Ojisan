#include "Block.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"

namespace
{
	using std::vector;

	/*using std::vector;
	int model_t = -1;
	vector< vector<int>> mapData =
	{
		{1,0,1,1,1,1,1,0,0,0},
		{1,1,0,1,1,1,1,1,1,1},
		{1,0,1,1,1,1,1,1,1,1},
		{1,0,1,1,0,0,0,0,1,1},
		{1,1,1,1,0,0,0,0,1,1},
		{1,1,1,0,0,0,0,0,1,1},
		{1,1,1,1,1,0,0,0,0,0},
		{1,1,1,1,1,0,1,1,1,0},
		{1,1,0,0,0,0,1,0,0,0},
		{1,0,0,1,1,1,1,0,0,0},
	};*/
}

Block::Block(GameObject* parent)
	:GameObject(parent, "Block"), hModel_(-1), mapWidth_(-1), mapHeight_(-1)
{
	
}

//初期化
void Block::Initialize()
{


	CsvReader csvData;
	csvData.Load("map.csv"); //scvファイルを読み込む
	mapWidth_ = csvData.GetWidth();
	mapHeight_ = csvData.GetHeight();

	mapData_ = vector<vector<int>>(mapHeight_, vector<int>(mapWidth_, 0));
	for (int x = 0; x < mapWidth_; x++)
	{
		for (int y = 0; y < mapHeight_; y++)
		{
			mapData_[y][x] = csvData.GetValue(x, y); //csvの値をmapData_に格納
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 11; j++)
		{
			foods_[i][j] = nullptr;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 11; j++)
		{
			//if (abs(i) % 2 == 1 && abs(j) % 2 == 1) {
			//	transform_.position_ = XMFLOAT3(2 + 4 * i, 0, 2 + 4 * j);
			//	Model::SetTransform(hModel_, transform_);
			//	Model::Draw(hModel_);
			//}
			if (mapData_[i][j] == 2)
			{
				foods_[i][j] = Instantiate<Food>(this);
				foods_[i][j]->SetFoodType(FoodType::FOODTYPE_NORMAL);
				foods_[i][j]->SetPosition(XMFLOAT3(-40 + (j * 4), 0, 36 - (i % 10) * 4));
				foods_[i][j]->SetScale(XMFLOAT3(1, 1, 1));
			}
			else if (mapData_[i][j] == 3)
			{

			}
		}
	}



	hModel_ = Model::Load("Block.fbx");
	assert(hModel_ >= 0);

	
}

//更新
void Block::Update()
{
}

//描画
void Block::Draw()
{
	int x = 10;
	int z = 2;
	Model::SetAnimFrame;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 11; j++)
		{
			if (mapData_[i][j] == 1) {
				transform_.position_ = XMFLOAT3(2 + 4 * (j - 5), 0, -2 + 4 * -(i - 5));
				transform_.scale_ = XMFLOAT3(0.99f, 0.99f, 0.99f);
				Model::SetTransform(hModel_, transform_);
				Model::Draw(hModel_);
			}
		}
	}
	
	
	

	Transform tr;
}

//解放
void Block::Release()
{
}

Food* Block::GetFood(int mapZ, int mapX)
{
	return foods_[mapZ][mapX];
}

int Block::RemoveFood(int mapZ, int mapX)
{
	int score_ = foods_[mapZ][mapX]->GetScore();
	foods_[mapZ][mapX]->KillMe();
	foods_[mapZ][mapX] = nullptr;
	mapData_[mapZ][mapX] = 0;
	return score_;
}
