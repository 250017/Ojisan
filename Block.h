#pragma once
#include "Engine/GameObject.h"
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

private:
    int hModel_;
    std::vector<std::vector<int>> mapData_;
    int mapWidth_;
    int mapHeight_;
};

