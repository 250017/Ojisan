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

	//壁を2D化
	transform_.rotate_.x = 270.0f;

	mapData_ = vector<vector<int>>(mapHeight_, vector<int>(mapWidth_, 0));
	for (int y = 0; y < mapHeight_; y++)
	{
		for (int x = 0; x < mapWidth_; x++)
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

	for (int y = 0; y < mapHeight_ -1; y++) {
		for (int x = 0; x < mapWidth_; x++)
		{
			//xf (abs(x) % 2 == 1 && abs(j) % 2 == 1) {
			//	transform_.posxtxon_ = XMFLOAT3(2 + 4 * x, 0, 2 + 4 * j);
			//	Model::SetTransform(hModel_, transform_);
			//	Model::Draw(hModel_);
			//}
			if (mapData_[y][x] == 2)
			{
				foods_[y][x] =Instantiate<Food>(this);
				foods_[y][x]->SetFoodType(FoodType::FOODTYPE_NORMAL);
				foods_[y][x]->SetPosition(XMFLOAT3(-40 + (x * 4), 36 - (y * 10) * 4, 0));
				foods_[y][x]->SetScale(XMFLOAT3(1, 1, 1));
			}
			else if (mapData_[y][x] == 3)
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

	for (int y = 0; y < mapHeight_; y++) {
		for (int x = 0; x < mapWidth_; x++)
		{
			if (mapData_[y][x] == 1) {
				transform_.position_ = XMFLOAT3(2 + 4 * (x - 5), - 2 + 4 * -(y - 5), 0);
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
