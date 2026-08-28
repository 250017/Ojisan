#pragma once
#include "Engine/GameObject.h"
#include "Food.h"
#include <vector>

class Block :
    public GameObject
{
public:
    //コンストラクタ
    Block(GameObject* parent);

    //初期化
    void Initialize() override;
    std::vector<std::vector<int>> GetMapData() { return mapData_; }

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    //フードをもらう
    Food* GetFood(int mapZ, int mapX);

    //フードを消す
    int RemoveFood(int mapZ, int mapX);

private:
    int hModel_;
    std::vector<std::vector<int>> mapData_;
    int mapWidth_;
    int mapHeight_;
    Food* foods_[10][11];
};

