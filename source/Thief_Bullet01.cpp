#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"CoinManager.h"
#include	"CharacterManager.h"
#include	"BaseBullet.h"
#include	"Thief_Bullet01.h"

Thief_Bullet01::Thief_Bullet01()
{
}

bool Thief_Bullet01::Initialize()
{
	judgeTimer = 10;
	limitTimer = 1.5 * SECOND;
	activate = false;
	state = true;
	scale = 0.05f;
	leanpower = 0;
	return true;
}

void	Thief_Bullet01::Update(void)
{
	//	ìÆçÏ
	Move();
	pos += move;

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	limitTimer--;

	if (StageCollisionCheck()) state = false;
	PlayerCollisionCheck();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Thief_Bullet01::Move(void)
{
	Stalk();
}

void Thief_Bullet01::Stalk()
{
	float ToPlayerLength = 10000.0f;	//Ç∆ÇËÇ†Ç¶Ç∏í¥Ç≈Ç©Ç¢êîéö
	Vector3 ToPlayerVec;
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//é©ï™ÇÃë≈Ç¡ÇΩíeÇ»ÇÁèúäO
		Vector3 playerPos = characterManager->GetPos(i);
		Vector3 vec = playerPos - pos;
		float len = vec.Length();
		if (len < ToPlayerLength)
		{
			ToPlayerLength = len;
			ToPlayerVec = characterManager->GetPos(i) - pos;
		}
	}
	ToPlayerVec.Normalize();

	switch (step)
	{
	case 0:
		time++;
		if (time >= 20) step++;
		break;
	case 1:
		move = ToPlayerVec * 0.5f;
		step++;
		break;
	case 2:
		move += ToPlayerVec * 0.01f;
	}
}