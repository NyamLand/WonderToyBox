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
#include	"ThiefBullet03.h"
#include	"BulletManager.h"
#include	"Stage.h"

ThiefBullet03::ThiefBullet03() :growSpeed(0.0f), checkMax(false), checkMin(false)
{
	scale		=	Vector3(0.01f,0.01f,0.005f);
//	judgeTimer	=	BULLET_JUDGETIMER	[	BULLET_TYPE::THIEF_03	];
	radius		=	BULLET_RADIUS		[	BULLET_TYPE::THIEF_03	];
	limitTimer	=	BULLET_LIMITTIMER	[	BULLET_TYPE::THIEF_03	];
}

bool ThiefBullet03::Initialize()
{
	m_BulletManager->Set(BULLET_TYPE::THIEF_04, new ThiefBullet04, pos, move, angle, 0.5f, playerNum);
	return true;
}

void	ThiefBullet03::Update(void)
{
	//	����
	ControlScale();

//	if (judgeTimer > 0)	judgeTimer--;
//	else							activate = true;

	limitTimer--;

	liveTime++;

	//PlayerCollisionCheck();

	
	if (checkMin == true) state = false;


	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	ThiefBullet03::ControlScale(void)
{
	Vector3 front = GetFront();
	front.Normalize();
	growSpeed += 0.05f;
	float addLength = sinf(growSpeed);
	move = front * addLength;


	if (!stage->CheckWall(pos,move))
	{
		pos += move;;
		scale.z += addLength * 0.005f;
	}

	//growSpeed += 0.05f;
	//float addLength = sinf(growSpeed);
	//scale.z += addLength * 0.005f;
	

	//�k�ݎn�߂���ő�܂ŐL�т����̂Ƃ���(�k�ݎn�߂Ă���)
	if (addLength <= 0)
	{
		checkMax = true;
	}

	//��x�k��ł���ēx�L�т�ƍŏ��܂ŏk�񂾂��̂Ƃ���
	if (addLength >= 0 && checkMax == true || scale.z <= 0.001f && checkMax == true)
	{
		checkMin = true;
	}

}