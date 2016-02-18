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
#include	"BulletManager.h"
#include	"SquirrelBullet01.h"

SquirrelBullet01::SquirrelBullet01()
{
	scale		=	Vector3(0.05f,0.05f,0.05f);
//	judgeTimer	=	BULLET_JUDGETIMER	[	BULLET_TYPE::SQUIRREL_01	]; 
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::SQUIRREL_01	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::SQUIRREL_01	];
}

bool SquirrelBullet01::Initialize()
{
	return true;
}

void	SquirrelBullet01::Update(void)
{
	//	動作
	Move();

//	if (judgeTimer > 0)	judgeTimer--;
//	else							activate = true;

	limitTimer++;

	liveTime++;

	StageCollisionCheck();
	PlayerCollisionCheck();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	SquirrelBullet01::Move(void)
{
	// 反射( ステージ )	
	static float rate = 0.4f;
	Collision::GetReflect(pos, move, rate);
	pos += move;
}