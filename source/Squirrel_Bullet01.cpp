#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"CoinManager.h"
#include	"BaseBullet.h"
#include	"Squirrel_Bullet01.h"

Squirrel_Bullet01::Squirrel_Bullet01()
{

}

bool Squirrel_Bullet01::Initialize()
{
	judgeTimer = 10;
	limitTimer = 0;
	activate = false;
	state = true;
	scale = 0.05f;
	leanpower = 0;
	return true;
}

void	Squirrel_Bullet01::Update(void)
{
	//	動作
	Move();

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	limitTimer++;

	StageCollisionCheck();
	PlayerCollisionCheck();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Squirrel_Bullet01::Move(void)
{
	pos += move;
	// 反射( ステージ )	
	static float rate = 0.4f;
	Collision::GetReflect(pos, move, rate);
}