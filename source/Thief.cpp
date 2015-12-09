
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Thief.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"GameManager.h"
#include	"BulletManager.h"

//*********************************************************************************
//
//	Thiefクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------

namespace
{
	enum OFFENSIVE_POWER
	{
		QUICK = 1,
		POWER = 5,
		HYPER = 15,
	};
}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

//	コンストラクタ
Thief::Thief(void) : BaseChara()
{
	//	パラメータ初期化
	speed = 0.25f;
	scale = 0.05f;
	diffence = -1;
	stayTime = 0;
	absorb_length = DEFAULT_ABSORB_LENGTH;
	isGround = true;
}

//	デストラクタ
Thief::~Thief(void)
{

}

//	初期化
bool	Thief::Initialize(int playerNum, Vector3 pos)
{
	//	親の初期化
	BaseChara::Initialize(playerNum, pos);

	//	モデル読み込み
	if (obj == nullptr)
		obj = new iex3DObj("DATA/CHR/Thief/Thief.IEM");

	if (obj == nullptr)	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

//	描画
void	Thief::Render(iexShader* shader, LPSTR technique)
{
	BaseChara::Render(shader, technique);

	//	デバッグ用
	if (!debug)	return;
	DrawSphere(attackInfo.pos, attackInfo.r, 0xFFFFFFFF);
	particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z - attackInfo.r), 0.3f);
	particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z + attackInfo.r), 0.3f);
	particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);
	particle->BlueFlame(Vector3(attackInfo.pos.x - attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);

	char	str[256];
	Vector3	stringPos;
	WorldToClient(pos, stringPos, matView* matProjection);
	stringPos.y -= 150.0f;
	sprintf_s(str, "怪\n盗\n↓");
	DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

//	クイックアーツ
bool	Thief::QuickArts(void)
{
	static int time = 0;

	//	行列から情報取得
	Vector3	up = GetUp();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//	情報設定
	Vector3	vec[3] =
	{
		up * 5.0f + right * 3.0f,
		up * 5.0f,
		up * 5.0f + right * -3.0f
	};
	p_pos.y += 3.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			m_BulletManager->Set(BULLET_TYPE::THIEF_01 , new Thief_Bullet01, p_pos, vec[i], bulletSpeed, playerNum);
		}
	}
	time++;

	//一秒間硬直
	if (time >= 1 * SECOND)
	{
		time = 0;
		return true;
	}
	return	false;
}

//	パワーアーツ
bool	Thief::PowerArts(void)
{
	static int time = 0;

	//	行列から情報取得
	Vector3	front = GetFront();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//	情報設定
	p_pos.y += 1.0f;
	Vector3	vec = front * 1.0f;

	float	 bulletSpeed = 0.8f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		m_BulletManager->Set(BULLET_TYPE::THIEF_02, new Thief_Bullet02, p_pos, vec, bulletSpeed, playerNum);
	}
		time++;

		if (time >= 1 * SECOND)
		{
			time = 0;
			return true;
		}
	return	false;
}

//	ハイパーアーツ
bool	Thief::HyperArts(void)
{
	power = HYPER;
	unrivaled = true;
	move = Vector3(0, 0 - GRAVITY, 0);	//撃ってる間は静止させる

	static int time = 0;

	//	行列から情報取得
	Vector3	front = GetFront();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//	情報設定
	Vector3	vec[5] =
	{
		front * 5.0f + right * 8.0f,
		front * 5.0f + right * 3.0f,
		front * 5.0f,
		front * 5.0f + right * -3.0f,
		front * 5.0f + right * -8.0f
	};
	Vector3 b_angle[5] =
	{
		{ 0, angle + GetAngle(vec[0], front), 0 },
		{ 0, angle + GetAngle(vec[1], front), 0 },
		{ 0, angle, 0 },
		{ 0, angle - GetAngle(vec[3], front), 0 },
		{ 0, angle - GetAngle(vec[4], front), 0 }
	};
	float	 bulletSpeed = 0.5f;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			m_BulletManager->Set(BULLET_TYPE::THIEF_03, new Thief_Bullet03, p_pos, vec[i], b_angle[i],  bulletSpeed, playerNum);
		}
	}
	time++;

	if (time >= 150)
	{
		time = 0;
		return true;
	}
	return	false;
}


//	モーション管理
void	Thief::MotionManagement(int motion)
{
	switch (motion)
	{
	//case MOTION_NUM::STAND:
	//	obj->SetMotion( MOTION_DATA::STAND );
	//	break;

	case MOTION_NUM::POSTURE:
		obj->SetMotion( MOTION_DATA::POSTURE );
		break;

	case MOTION_NUM::JUMP:
		obj->SetMotion(MOTION_DATA::JUMP);
		break;

	case MOTION_NUM::GUARD:
		obj->SetMotion(MOTION_DATA::GUARD);
		break;

	//case MOTION_NUM::LANDING:
	//	obj->SetMotion(MOTION_DATA::POSTURE);
	//	break;

	case MOTION_NUM::RUN:
		obj->SetMotion(MOTION_DATA::RUN);
		break;

	case MOTION_NUM::ATTACK1:
		obj->SetMotion(MOTION_DATA::QUICK);
		break;

	case MOTION_NUM::ATTACK2:
		obj->SetMotion(MOTION_DATA::POWER);
		break;

	case MOTION_NUM::ATTACK3:
		obj->SetMotion(MOTION_DATA::HYPER);
		break;
	}
}

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

//	攻撃用パラメータ設定
void	Thief::SetAttackParam(int attackKind)
{
	switch (attackKind)
	{
	case MODE_STATE::QUICKARTS:
		knockBackInfo.isUp = false;
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		if (attackInfo.t < 0.6) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hitまでは仰け反りのみ
		if (attackInfo.t >= 0.6) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit目からは吹き飛ばしあり
		break;

	case MODE_STATE::POWERARTS:
		knockBackInfo.isUp = true;
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
		break;

	case MODE_STATE::HYPERARTS:
		knockBackInfo.isUp = false;
		attackInfo.type = Collision::SPHEREVSCYRINDER;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}