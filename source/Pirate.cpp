
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Pirate.h"
#include	"GameManager.h"
#include	"BulletManager.h"
#include	"Random.h"

//*********************************************************************************
//
//	Princessクラス
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
Pirate::Pirate(void) : BaseChara()
{
	//	パラメータ初期化
	power = 2;/*仮*/
	speed = 0.25f;
	scale = 0.02f;
	diffence = -1;
	isGround = true;
}

//	デストラクタ
Pirate::~Pirate(void)
{

}

//	初期化
bool	Pirate::Initialize(int playerNum, Vector3 pos)
{
	//	親の初期化
	BaseChara::Initialize(playerNum, pos);

	//	モデル読み込み
	if (obj == nullptr)
		obj = new iex3DObj("DATA/CHR/Y2009/Y2009.IEM");

	//	スケール設定
	obj->SetScale(0.02f);
	obj->Update();

	if (obj == nullptr)	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

//	描画
void	Pirate::Render(iexShader* shader, LPSTR technique)
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
	sprintf_s(str, "海\n賊\n↓");
	DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

//	クイックアーツ
bool	Pirate::QuickArts(void)
{
	static int time = 0;

	//	行列から情報取得
	Vector3	front = GetFront();
	front.Normalize();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));
	Vector3 vec = front;

	p_pos.y += 1.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new Pirate_Bullet01, p_pos, vec, bulletSpeed, playerNum);
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
bool	Pirate::PowerArts(void)
{
	power = POWER;
	Vector3	p_pos = GetPos();
	attackInfo.pos = Vector3(p_pos.x, p_pos.y + 1.5f, p_pos.z);
	SetMove(Vector3(0.0f, 0.0f, 0.0f));

	//	範囲拡大
	Lerp(attackInfo.r, 0.0f, 3.0f, attackInfo.t);

	//	パラメータ加算
	attackInfo.t += 0.02f;

	//	無敵状態
	if (attackInfo.t <= 0.5f)		unrivaled = true;
	else									unrivaled = false;

	if (attackInfo.t >= 1.0f)	return	true;
	return	false;
}

//	ハイパーアーツ
bool	Pirate::HyperArts(void)
{
	static int time = 0;

	//	行列から情報取得
	Vector3	front = GetFront();
	front.Normalize();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));
	Vector3 vec = front;

	p_pos.y += 1.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		m_BulletManager->Set(BULLET_TYPE::PIRATE_02, new Pirate_Bullet02, p_pos, vec, bulletSpeed, playerNum);
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



//	モーション管理
void	Pirate::MotionManagement(int motion)
{
	switch (motion)
	{
	case MOTION_NUM::STAND:
		obj->SetMotion(MOTION_DATA::STAND);
		break;

	case MOTION_NUM::POSTURE:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::JUMP:
		obj->SetMotion(MOTION_DATA::JUMP);
		break;

	case MOTION_NUM::GUARD:
		obj->SetMotion(MOTION_DATA::GUARD);
		break;

	case MOTION_NUM::LANDING:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::RUN:
		obj->SetMotion(MOTION_DATA::RUN);
		break;

	case MOTION_NUM::ATTACK1:
		obj->SetMotion(MOTION_DATA::ATTACK1);
		break;

	case MOTION_NUM::ATTACK2:
		obj->SetMotion(MOTION_DATA::ATTACK2);
		break;

	case MOTION_NUM::ATTACK3:
		obj->SetMotion(MOTION_DATA::ATTACK3);
		break;
	}
}

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

//	攻撃用パラメータ設定
void	Pirate::SetAttackParam(int attackKind)
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