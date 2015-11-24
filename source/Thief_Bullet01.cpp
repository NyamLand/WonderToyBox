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
	radius = 10.0f;
	limitTimer = 3 * SECOND;
	activate = false;
	state = true;
	scale = Vector3( 0.05f,0.05f,0.05f );
	leanpower = 0;
	angle.y = characterManager->GetAngle(playerNum);

	return true;
}

void	Thief_Bullet01::Update(void)
{
	//	動作
	Move();

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	limitTimer--;


	liveTime++;

	if (!StageCollisionCheck() && !Collision::CheckWall(pos,move))	PlayerCollisionCheck();	//壁にも地面にも触れていないときプレイヤーとあたり判定を取る


	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}


void	Thief_Bullet01::Move(void)
{
	ToPlayerLength = 10000.0f;	//とりあえず超でかい数字
	ToPlayerVec;
	frontDot, backDot, upDot, downDot;		//プレイヤー方向とそれぞれの方向の内積
	cross;
	front = GetFront();
	up = GetUp();



	switch (step)
	{
	case STEP::CREATED://トランプ出現
		if (liveTime >= 20) step = STEP::WAIT;
		break;
	case STEP::WAIT:
		Wait();
		break;
	case STEP::TARGETING:
		Targeting();
		break;
	case STEP::FIRING:	//発射
		Firing();
		break;
	case STEP::STALK:	//追跡
		Stalk();
		break;
	}

	if (StageCollisionCheck() || Collision::CheckWall(pos, move))	move = Vector3(0, 0, 0);	//壁が地面に触れると停止
	pos += move;
}

void Thief_Bullet01::Wait()
{
	move = Vector3(0, 0, 0);

	//Targetingで狙う場所を設定しておく
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//自分の打った弾なら除外
		Vector3 playerPos = characterManager->GetPos(i);
		Vector3 vec = playerPos - pos;
		float len = vec.Length();
		if (len < ToPlayerLength)
		{
			ToPlayerLength = len;
			ToPlayerVec = characterManager->GetPos(i) - pos;
			frontDot = Vector3Dot(front, ToPlayerVec);
			backDot = Vector3Dot(-front, ToPlayerVec);

			Vector3Cross(cross, front, ToPlayerVec);

		}
	}
	ToPlayerVec.Normalize();

	if (frontDot > backDot)
	{
		if (cross.y < 0) angle.y -= 0.1f;
		else angle.y += 0.1f;

		//高さを考慮しない左右のみの角度
		Vector3 tempFront = Vector3(front.x, 0.0f, front.z);		
		Vector3 tempToPlayerVec = Vector3(ToPlayerVec.x, 0.0f, ToPlayerVec.z);
		frontDot = GetDegreeAngle(tempFront, tempToPlayerVec);
		if (frontDot <= 10)
		{
			step = STEP::TARGETING;
		}
	}
	else
	{
		if (cross.y < 0) angle.y += 0.1f;
		else angle.y -= 0.1f;

		//高さを考慮しない左右のみの角度
		Vector3 tempBack = Vector3(-front.x, 0.0f, -front.z);
		Vector3 tempToPlayerVec = Vector3(ToPlayerVec.x, 0.0f, ToPlayerVec.z);
		backDot = GetDegreeAngle(tempBack, tempToPlayerVec);
		if (backDot <= 10)
		{
			step = STEP::TARGETING;
		}
	}


	//if (cross.y < 0)
	//{
	//	if (frontDot > backDot) angle.y -= 0.1f;
	//	else angle.y += 0.05f;
	//}
	//else
	//{
	//	if (frontDot > backDot) angle.y += 0.1f;
	//	else angle.y -= 0.1f;
	//}

	//if (liveTime >= 50) step = STEP::TARGETING;
}

void Thief_Bullet01::Targeting()
{
	upDot = GetDegreeAngle(up, ToPlayerVec);
	backDot = GetDegreeAngle(-up, ToPlayerVec);

	angle.x += 0.5f;

	if (liveTime >= 40)
	{
		step = STEP::FIRING;
	}
}

void Thief_Bullet01::Firing()
{
	move = ToPlayerVec * 1.0f;
	step = STEP::STALK;
}

void Thief_Bullet01::Stalk()
{
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//自分の打った弾なら除外
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

	move += ToPlayerVec * 0.02f;
}