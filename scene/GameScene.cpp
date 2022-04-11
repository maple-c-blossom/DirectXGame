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

	viewProjection_.eye = {0, 0, -50};

	viewProjection_.target = {10, 0, 0};

	viewProjection_.up = {1.0f, 0.0f, 0.0f};

	viewProjection_.Initialize();

	random_device seed_gen;
	mt19937_64 engin(seed_gen());
	uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (int i = 0; i < _countof(worldTransform_); i++) 
	{
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform_[i].rotation_ = {rotDist(engin), rotDist(engin), rotDist(engin)};
		worldTransform_[i].translation_ = {posDist(engin), posDist(engin), posDist(engin)};

		worldTransform_[i].Initialize();
	}

}

void GameScene::Update() 
{ 

	#pragma region 視点移動

	XMFLOAT3 move = {0, 0, 0};
	const float kEyeSpeed = 0.2f;
	if (input_->PushKey(DIK_W)) {
		move = {0, 0, kEyeSpeed};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, 0, -kEyeSpeed};
	}
	viewProjection_.eye.x += move.x;
	viewProjection_.eye.y += move.y;
	viewProjection_.eye.z += move.z;

	//注視点
	XMFLOAT3 moveTarget = {0, 0, 0};
	const float kTargetSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		moveTarget = {0, 0, -kTargetSpeed};
	} else if (input_->PushKey(DIK_RIGHT)) {
		moveTarget = {0, 0, kTargetSpeed};
	}

	viewProjection_.target.x += moveTarget.x;
	viewProjection_.target.y += moveTarget.y;
	viewProjection_.target.z += moveTarget.z;

	//上方向
	const float kUpRotSpeed = 0.05f;
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed ;
		viewAngle = fmodf(viewAngle, XM_2PI);
	}

	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	viewProjection_.UpdateMatrix();
	#pragma endregion


	for (int i = 0; i < _countof(worldTransform_); i++) {
	
		worldTransform_[i].UpdateMatrix();
	}

	debugText_->SetPos(50, 70);

	debugText_->Printf(
	  "eye : (%f,%f,%f)\n", viewProjection_.eye.x, viewProjection_.eye.y,
	  viewProjection_.eye.z);

	debugText_->SetPos(50, 90);

	debugText_->Printf(
	  "target : (%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(50, 110);

	debugText_->Printf(
	  "up : (%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y,
	  viewProjection_.up.z);


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
