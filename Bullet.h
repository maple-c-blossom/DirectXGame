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


using namespace DirectX;
using namespace std;
class Bullet
{
public:
	WorldTransform worldTransform_;
	XMFLOAT3 speed = {1.0f,1.0f,1.0f};
	XMFLOAT3 frontVec = { 0,0,0 };
	bool liveFlag = false;
	int count = 0;
	void Shoot(XMFLOAT3 charaFrontVec, WorldTransform charaworld);
	void Update();
};

