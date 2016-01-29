
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Thief.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"GameManager.h"
#include	"Sound.h"
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
	scale = 0.035f;
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

	////	デバッグ用
	//if (!debug)	return;
	//DrawSphere(attackInfo.pos, attackInfo.r, 0xFFFFFFFF);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z - attackInfo.r), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z + attackInfo.r), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x - attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);

	//char	str[256];
	//Vector3	stringPos;
	//WorldToClient(pos, stringPos, matView* matProjection);
	//stringPos.y -= 150.0f;
	//sprintf_s(str, "怪\n盗\n↓");
	//DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

//	クイックアーツ
bool	Thief::QuickArts(void)
{
	static int time = 0;
	static	bool	initflag = false;
	if ( !initflag )
	{
		sound->PlaySE( SE::KAITO_QUICK );
		initflag = true;
	}
	Move();
	//	行列から情報取得
	Vector3	up = GetUp();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//モーションアトデナオス
	SetMotion(4);
	//if (obj->GetFrame() >= 237) obj->SetMotion(237);

	//	情報設定
	Vector3	vec[3] =
	{
		up * 5.0f + right * 3.0f,
		up * 5.0f,
		up * 5.0f + right * -3.0f
	};
	p_pos.y += 3.0f;
	float	 bulletSpeed = 0.5f;
	int playerNum = GetPlayerNum();


	{	//撃った時に動きが一度止まるパターン
		//					//モーションアトデナオス(ちょうどいい感じのフレームが来たら弾発射)
		//if (time == 0 /* obj->GetFrame() == ○○ */ )
		//{
		//	for (int i = 0; i < 3; i++)
		//	{
		//		m_BulletManager->Set(BULLET_TYPE::THIEF_01 , new Thief_Bullet01, p_pos, vec[i], bulletSpeed, playerNum);
		//	}
		//}
		//time++;

		////モーションアトデナオス(終わりのモーションが来たら終了)
		//if (obj->GetFrame() == 237)
		//{
		//	time = 0;
		//	return true;
		//}
	}

	{	//撃った時に動きが止まらないパターン
		for (int i = 0; i < 3; i++)
		{
			m_BulletManager->Set(BULLET_TYPE::THIEF_01 , new Thief_Bullet01, p_pos, vec[i], bulletSpeed, playerNum);
		}
		attackInfo.Interval = 60;
		initflag = false;
		return true;

	}
	return	false;
}

//	パワーアーツ
bool	Thief::PowerArts(void)
{
	static int time = 0;
	static	bool initflag = false;
	if ( !initflag )
	{
		sound->PlaySE( SE::KAITO_POWER );
		initflag = true;
	}
	//モーションアトデナオス
	SetMotion(5);
	//if (obj->GetFrame() >= 277) obj->SetFrame(277);

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

	//モーションアトデナオス(ちょうどいい感じのフレームが来たら弾発射)
	if (time == 0/* obj->GetFrame() == ○○ */)
	{
		m_BulletManager->Set(BULLET_TYPE::THIEF_02, new Thief_Bullet02, p_pos, vec, bulletSpeed, playerNum);
	}
		time++;

		//モーションアトデナオス
		if (obj->GetFrame() == 277)
		{
			time = 0;
			initflag = false;
			return true;
		}
	return	false;
}

//	ハイパーアーツ
bool	Thief::HyperArts(void)
{
	static	bool	initflag = false;
	if ( !initflag )
	{
		sound->PlaySE( SE::KAITO_HYPER );
		initflag = true;
	}


	//モーションアトデナオス
	SetMotion(6);


	power = HYPER;
	SetParameterState(PARAMETER_STATE::UNRIVALED);
	move = Vector3(0, 0 - GRAVITY, 0);	//撃ってる間は静止させる


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
		{ 0, angle.y + GetAngle(vec[0], front), 0 },
		{ 0, angle.y + GetAngle(vec[1], front), 0 },
		{ 0, angle.y, 0 },
		{ 0, angle.y - GetAngle(vec[3], front), 0 },
		{ 0, angle.y - GetAngle(vec[4], front), 0 }
	};
	float	 bulletSpeed = 0.5f;
	int playerNum = GetPlayerNum();

	//モーションアトデナオス(ちょうどいい感じのフレームが来たら弾発射)
	if (obj->GetFrame() == 339)
	{
		for (int i = 0; i < 5; i++)
		{
			m_BulletManager->Set(BULLET_TYPE::THIEF_03, new Thief_Bullet03, p_pos, vec[i], b_angle[i],  bulletSpeed, playerNum);
		}
	}

	//モーションアトデナオス(終わりのモーションが来たら終了)
	if (obj->GetFrame() == 399)
	{
		initflag = false;
		return true;
	}
	return	false;
}


//	モーション管理
/*void	Thief::MotionManagement(int motion)
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
}*/

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

//	攻撃用パラメータ設定
void	Thief::SetAttackParam(int attackKind)
{
	switch (attackKind)
	{
	case MODE_STATE::QUICKARTS:
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		if (attackInfo.t < 0.6) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hitまでは仰け反りのみ
		if (attackInfo.t >= 0.6) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit目からは吹き飛ばしあり
		break;

	case MODE_STATE::POWERARTS:
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
		break;

	case MODE_STATE::HYPERARTS:
		attackInfo.type = Collision::SPHEREVSCYRINDER;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}