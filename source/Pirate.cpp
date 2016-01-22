
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Pirate.h"
#include	"CharacterManager.h"
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
	scale = 0.045f;
	diffence = -1;
	isGround = true;
}

//	デストラクタ
Pirate::~Pirate( void )
{
	SafeDelete( cannon );
}

//	初期化
bool	Pirate::Initialize(int playerNum, Vector3 pos)
{
	//	親の初期化
	BaseChara::Initialize(playerNum, pos);

	//	モデル読み込み
	if (obj == nullptr)
		obj = new iex3DObj("DATA/CHR/Pirate/Pirate.IEM");

	//	スケール設定
	obj->SetScale(0.02f);
	obj->Update();
	cannon = new iexMesh( "DATA/CHR/Pirate/PirateCannon.IMO" );
	//cannon->SetScale( 0.02f );



	if (obj == nullptr)	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

//	描画
void	Pirate::Render(iexShader* shader, LPSTR technique)
{
	SetCannonPos();
	BaseChara::Render(shader, technique);
	cannon->Render();
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

	//弾を発射するときの情報を決定
	p_pos.y += 1.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	//発射する弾の種類を決定
	int pattern;
	int rnd = Random::GetInt(0, 99);
	if (0 <= rnd && rnd < 70)	pattern = QuickArts_DATA::NORMAL_SHOT;
	if (70 <= rnd && rnd < 100)	pattern = QuickArts_DATA::TIMER_SHOT;

	if (time == 0)
	{
		switch (pattern)
		{
		case QuickArts_DATA::NORMAL_SHOT:
			m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new Pirate_Bullet01, p_pos, vec, bulletSpeed, playerNum);
			break;
		case QuickArts_DATA::TIMER_SHOT:
			m_BulletManager->Set(BULLET_TYPE::PIRATE_02, new Pirate_Bullet02, p_pos, vec, bulletSpeed, playerNum);
			break;
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
bool	Pirate::PowerArts(void)
{
	float run_speed = 0.5f;
	SetUnrivaled(false);

	//	行列から前方取得
	SetMove(Vector3(0.0f, 0.0f, 0.0f));
	Vector3	front = GetFront();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos() + front * 2.0f;

	//	あたり判定のパラメータを与える
	attackInfo.r = 2.0f;
	attackInfo.bottom = p_pos;
	attackInfo.top = attackInfo.bottom + Vector3(0.0f,3.0f,0.0f);

	attackInfo.bottom = p_pos - right * 2.0f;
	attackInfo.top = attackInfo.bottom + right * 4.0f;


	static	float adjustSpeed = 0.02f;
	AngleAdjust(adjustSpeed);

	//	パラメータ加算
	attackInfo.t += 0.015f;

	if (attackInfo.t < 1.0f)move = front * run_speed;

	if (attackInfo.t >= 1.5f)	return	true;
	return	false;
}

//	ハイパーアーツ
bool	Pirate::HyperArts(void)
{
	//大砲の砲弾発射位置決定（画面上側）
	Vector3 b_pos;	
	b_pos = Vector3(
		Random::GetFloat(-20.0f, -20.0f),
		30.0f,
		Random::GetFloat(-20.0f, 20.0f)
		);

	//自分以外のプレイヤー三人を着弾地点にする
	Vector3 target[3];
	for (int i = 0,n = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//自分は除外
		target[n] = characterManager->GetPos(i);
		n++;
	}

	float bulletSpeed = 0.5f;
	Vector3 vec[3];
	for (int i = 0; i < 3; i++)
	{
		Parabola(vec[i], b_pos, target[i], bulletSpeed, GRAVITY);
		m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new Pirate_Bullet03, b_pos, vec[i], bulletSpeed, playerNum);
	}

	return true;
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

//	大砲位置設定
void	Pirate::SetCannonPos( void )
{
	Matrix	mat;
	mat = *obj->GetBone( 8 ) * obj->TransMatrix;
	cannon->TransMatrix = mat;
	particle->BlueFlame( Vector3( mat._41, mat._42, mat._43 ), 0.1f );
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
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		break;

	case MODE_STATE::POWERARTS:
		attackInfo.type = Collision::CAPSULEVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
		break;

	case MODE_STATE::HYPERARTS:
		attackInfo.type = Collision::SPHEREVSCYRINDER;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}