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

Pirate_Bullet02::Pirate_Bullet02() : Pirate_Bullet01()
{
	activate	=	true;
	state		=	true;
	scale		=	Vector3(0.05f, 0.05f, 0.05f);
	leanpower	=	0;
	//judgeTimer	=	BULLET_JUDGETIMER	[	BULLET_TYPE::PIRATE_01	];
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::PIRATE_02	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::PIRATE_02	];
}

void	Pirate_Bullet02::Update(void)
{
	//	“®ì
	Move();

	//	if (judgeTimer > 0)	judgeTimer--;
	//	else							activate = true;

	limitTimer--;


	if (PlayerCollisionCheck() || StageCollisionCheck())
	{
		enable = false;
		explosion = true;
	}

	if (explosion) Explode();
	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Pirate_Bullet02::Move(void)
{
	move.y += GRAVITY;
	pos += move;

}