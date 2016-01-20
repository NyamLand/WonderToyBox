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
#include	"Pirate_Bullet03.h"

Pirate_Bullet03::Pirate_Bullet03()
{
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::PIRATE_03	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::PIRATE_03	];
	scale		=	Vector3(0.01f, 0.01f, 0.01f);
	leanpower	=	0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		isPlayerCheck[i] = false;
	}
}

void	Pirate_Bullet03::Update(void)
{

	//	if (judgeTimer > 0)	judgeTimer--;
	//	else							activate = true;

	//limitTimer--;

	liveTime++;

	//

	if (PlayerCollisionCheck() ||/* Collision::CheckWall(pos, move)*/StageCollisionCheck())
	{
		enable = false;
		explosion = true;
	}

	if (explosion) Explode();


	//	“®ì
	Move();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Pirate_Bullet03::Move(void)
{
	move.y += GRAVITY;
	pos += move;
}