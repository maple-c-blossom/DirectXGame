#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include "Bullet.h"


using namespace DirectX;
using namespace std;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	uint32_t textreHandle_ = 0;
	uint32_t soundHandle_ = 0;
	uint32_t voiceHandle_ = 0;
	Sprite* sprite_ = nullptr;
	Model* model_ = nullptr;
	WorldTransform worldTransform_[2];
	WorldTransform Rales[100];
	Bullet bullet[20];

	XMFLOAT3 FrontVec = {0,0,1};
	ViewProjection viewProjection_;
	int32_t value_ = 0;
	float viewAngle = 0.0f;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
