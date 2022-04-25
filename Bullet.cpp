#include "Bullet.h"

void Bullet::Shoot(XMFLOAT3 charaFrontVec,WorldTransform charaworld)
{
	liveFlag = true;
	frontVec = charaFrontVec;
	worldTransform_.translation_ = charaworld.translation_;
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
}

void Bullet::Update()
{

	if (liveFlag)
	{
		worldTransform_.translation_.x += frontVec.x * speed.x;
		worldTransform_.translation_.y += frontVec.y * speed.y;
		worldTransform_.translation_.z += frontVec.z * speed.z;
		count++;
	}

	if (count >= 200)
	{
		liveFlag = false;
		count = 0;
	}
}
