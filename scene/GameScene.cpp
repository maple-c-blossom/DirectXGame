#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include "Bullet.h"

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

	viewProjection_.eye = {0, 0.5f, -100};

	viewProjection_.target = {0, 0, 0};

	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	viewProjection_.Initialize();


	worldTransform_[0].Initialize();

	worldTransform_[1].translation_ = {0.0f, 0.0f, 5.0f};
	worldTransform_[1].scale_ = { 0.5f,0.5f,0.5f };
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[1].Initialize();

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 100; i++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					Rales[j][i].translation_ = { 0,-2.5f,0 };
				}
				else
				{
					Rales[j][i].translation_ = { 0,2.5f,0 };
				}
			}
			else
			{
				Rales[j][i].translation_.x = Rales[j][i - 1].translation_.x + 3.0f;
				Rales[j][i].translation_.y = Rales[j][i - 1].translation_.y;
			}
			Rales[j][i].scale_ = { 1.5f,1.5f,1.5f };
			Rales[j][i].Initialize();
		}


	}
	for (int i = 0; i < _countof(bullet); i++)
	{
		bullet[i].liveFlag = false;
		bullet[i].worldTransform_.Initialize();
	}

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
		FrontVec = { 0,0,1 };
		for (int i = 0; i < _countof(bullet); i++)
		{
			bullet[i].liveFlag = false;
		}

	}

	if (input_->PushKey(DIK_LEFT))
	{
		worldTransform_[0].rotation_.y -= 0.05f;
		FrontVec.x = sinf(worldTransform_[0].rotation_.y);
		FrontVec.z = cosf(worldTransform_[0].rotation_.y);
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		worldTransform_[0].rotation_.y += 0.05f;
		FrontVec.x = sinf(worldTransform_[0].rotation_.y);
		FrontVec.z = cosf(worldTransform_[0].rotation_.y);
	}


	


	if (input_->PushKey(DIK_UP)) 
	{
		move.x = FrontVec.x * 1.0f;
		move.y = FrontVec.y * 1.0f;
		move.z = FrontVec.z * 1.0f;
	}
	else if (input_->PushKey(DIK_DOWN)) 
	{
		move.x = FrontVec.x * -1.0f;
		move.y = FrontVec.y * -1.0f;
		move.z = FrontVec.z * -1.0f;
	}


	#pragma endregion

	worldTransform_[0].translation_.x += move.x;
	worldTransform_[0].translation_.y += move.y;
	worldTransform_[0].translation_.z += move.z;


	viewProjection_.eye.x = worldTransform_[0].translation_.x + (20 * -FrontVec.x);
	viewProjection_.eye.y = worldTransform_[0].translation_.y;
	viewProjection_.eye.z = worldTransform_[0].translation_.z + (20 * -FrontVec.z);

	viewProjection_.target.x = worldTransform_[0].translation_.x;
	viewProjection_.target.y = worldTransform_[0].translation_.y;
	viewProjection_.target.z = worldTransform_[0].translation_.z;




	viewProjection_.UpdateMatrix();

	for (int i = 0; i < _countof(worldTransform_); i++) {
	
		worldTransform_[i].UpdateMatrix();
	}

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 100; i++)
		{
			Rales[j][i].UpdateMatrix();
		}
	}


	for (int i = 0; i < _countof(bullet); i++)
	{
		if (input_->TriggerKey(DIK_SPACE))
		{
			if (bullet[i].liveFlag == false)
			{
				bullet[i].Shoot(FrontVec,worldTransform_[0]);
				break;
			}
		}
		else
		{
			break;
		}
	}

	for (int i = 0; i < _countof(bullet); i++)
	{
		if (bullet[i].liveFlag)
		{
			bullet[i].Update();
			bullet[i].worldTransform_.UpdateMatrix();
		}
	}


	debugText_->SetPos(50, 70);

	debugText_->Printf(
		"ObjFront3D : (%f,%f,%f)  Bullet[0](%f,%f,%f)\n", worldTransform_->translation_.x, worldTransform_->translation_.y, worldTransform_->translation_.z,
		bullet[0].worldTransform_.translation_.x, bullet[0].worldTransform_.translation_.y, bullet[0].worldTransform_.translation_.z);


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

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 100; i++)
		{
			model_->Draw(Rales[j][i], viewProjection_, textreHandle_);
		}

	}
	for (int i = 0; i < _countof(bullet); i++)
	{
		if (bullet[i].liveFlag)
		{
			model_->Draw(bullet[i].worldTransform_, viewProjection_, textreHandle_);
		}
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
