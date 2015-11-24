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
#include	"Thief_Bullet03.h"
#include	"BulletManager.h"

Thief_Bullet03::Thief_Bullet03() :growSpeed(0.0f), checkMax(false), checkMin(false)
{
}

bool Thief_Bullet03::Initialize()
{
	m_BulletManager->Set(BULLET_MODEL::THIEF_04, new Thief_Bullet04, pos, move, angle, 0.5f, playerNum);
	radius = 100.0f;
	judgeTimer = 10000;
	limitTimer = 10 * SECOND;
	activate = false;
	state = true;
	scale = Vector3(0.005f,0.005f,0.005f);
	leanpower = 0;
	return true;
}

void	Thief_Bullet03::Update(void)
{
	//	ìÆçÏ
	ControlScale();

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	limitTimer--;

	//PlayerCollisionCheck();

	
	if (checkMin == true) state = false;


	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Thief_Bullet03::ControlScale(void)
{
	Vector3 front = GetFront();
	front.Normalize();
	growSpeed += 0.05f;
	float addLength = sinf(growSpeed);
	move = front * addLength;


	if (!Collision::CheckWall(pos, move))
	{
		pos += move;;
		scale.z += addLength * 0.005f;
	}

	//growSpeed += 0.05f;
	//float addLength = sinf(growSpeed);
	//scale.z += addLength * 0.005f;
	

	//èkÇ›énÇﬂÇΩÇÁç≈ëÂÇ‹Ç≈êLÇ—ÇΩÇ‡ÇÃÇ∆Ç∑ÇÈ(èkÇ›énÇﬂÇƒÇ¢ÇÈ)
	if (addLength <= 0)
	{
		checkMax = true;
	}

	//àÍìxèkÇÒÇ≈Ç©ÇÁçƒìxêLÇ—ÇÈÇ∆ç≈è¨Ç‹Ç≈èkÇÒÇæÇ‡ÇÃÇ∆Ç∑ÇÈ
	if (addLength >= 0 && checkMax == true || scale.z <= 0.001f && checkMax == true)
	{
		checkMin = true;
	}

}