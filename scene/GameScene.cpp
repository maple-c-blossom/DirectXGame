#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textreHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(textreHandle_, {100,50});
	model_ = Model::Create();
	//soundHandle_ = audio_->LoadWave("se_sad03.wav");
	//voiceHandle_ = audio_->PlayWave(soundHandle_, true);

	viewProjection_.eye = {0, 10, -50};

	viewProjection_.target = {0, 0, 0};

	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	viewProjection_.Initialize();


	worldTransform_[0].Initialize();

	worldTransform_[1].translation_ = {0.0f, 4.5f, 0.0f};
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[1].Initialize();


}

void GameScene::Update() 
{ 

	#pragma region 視点移動

	XMFLOAT3 move = {0, 0, 0};

	if (input_->PushKey(DIK_R)) 
	{
		worldTransform_[0].translation_ = {0.0f, 0.0f, 0.0f};
		worldTransform_[0].rotation_ = { 0.0f,0.0f,0.0f };
		worldTransform_[0].Initialize();
		ObjFront3D = { 0,0,1 };
	}

	if (input_->PushKey(DIK_LEFT))
	{
		worldTransform_[0].rotation_.y += 0.05f;
		ObjFront3D.x = sinf(worldTransform_[0].rotation_.y);
		ObjFront3D.z = cosf(worldTransform_[0].rotation_.y);
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		worldTransform_[0].rotation_.y -= 0.05f;
		ObjFront3D.x = sinf(worldTransform_[0].rotation_.y);
		ObjFront3D.z = cosf(worldTransform_[0].rotation_.y);
	}


	


	if (input_->PushKey(DIK_UP)) 
	{
		move.x = ObjFront3D.x * 1.0f;
		move.y = ObjFront3D.y * 1.0f;
		move.z = ObjFront3D.z * 1.0f;
	}
	else if (input_->PushKey(DIK_DOWN)) 
	{
		move.x = ObjFront3D.x * -1.0f;
		move.y = ObjFront3D.y * -1.0f;
		move.z = ObjFront3D.z * -1.0f;
	}


	viewProjection_.UpdateMatrix();
	#pragma endregion

	worldTransform_[0].translation_.x += move.x;
	worldTransform_[0].translation_.y += move.y;
	worldTransform_[0].translation_.z += move.z;


	for (int i = 0; i < _countof(worldTransform_); i++) {
	
		worldTransform_[i].UpdateMatrix();
	}

	debugText_->SetPos(50, 70);

	debugText_->Printf(
		"ObjFront3D : (%f,%f,%f)\n", ObjFront3D.x, ObjFront3D.y, ObjFront3D.z);


}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	for (int i = 0; i < _countof(worldTransform_); i++) 
	{
		model_->Draw(worldTransform_[i], viewProjection_, textreHandle_);
	}
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion 3Dオブジェクト描画

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	//XMFLOAT2 position = sprite_->GetPosition();

	//position.x += 2.0f;
	//position.y += 1.0f;

	//sprite_->SetPosition(position);


	/// <summary>
	//sprite_->Draw();

	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
