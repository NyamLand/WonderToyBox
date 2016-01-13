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
#include	"Pirate_Bullet02.h"
#include	"Stage.h"

Pirate_Bullet02::Pirate_Bullet02()
{
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::PIRATE_02	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::PIRATE_02	];
	scale		=	Vector3(0.05f, 0.05f, 0.05f);
	leanpower	=	0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		isPlayerCheck[i] = false;
	}
}

void	Pirate_Bullet02::Update(void)
{
	//	動作
	Move();

	//	if (judgeTimer > 0)	judgeTimer--;
	//	else							activate = true;

	limitTimer--;

	liveTime++; 

	StageCollisionCheck();
	
	if (PlayerCollisionCheck() || liveTime >= 300)
	{
		enable = false;
		explosion = true;
	}

	if (explosion) Explode();
	move *= 0.99f;	//徐々に減速

	pos += move;

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Pirate_Bullet02::Move(void)
{	
	// 反射( ステージ )	
	static float rate = 0.4f;
	stage->GetReflect(pos, move, rate);
	move.y += GRAVITY;
}