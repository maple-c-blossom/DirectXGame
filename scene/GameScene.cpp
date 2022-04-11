#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

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
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	soundHandle_ = audio_->LoadWave("se_sad03.wav");
	voiceHandle_ = audio_->PlayWave(soundHandle_, true);

}

void GameScene::Update() 
{ 
	if (input_->TriggerKey(DIK_SPACE)) 
	{
		audio_->StopWave(voiceHandle_);
	}

	value_++;
	string strDebug = string("value : ") + to_string(value_);
	debugText_->Print(strDebug, 50, 50, 1.0f);
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
	model_->Draw(worldTransform_, viewProjection_,textreHandle_);
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion 3Dオブジェクト描画

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	XMFLOAT2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);


	/// <summary>
	sprite_->Draw();

	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
