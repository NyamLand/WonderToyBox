
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
	cannon = new iexMesh( "DATA/CHR/Pirate/pirate_cannon.IMO" );
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
	BaseChara::Render( shader, technique );
	if ( renderflag )		cannon->Render();
}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

//	クイックアーツ
bool	Pirate::QuickArts(void)
{
	static int time = 0;

	SetMotion(PIRATE::MOTION_DATA::QUICK);
	//	行列から情報取得
	Vector3	front = GetFront();
	front.Normalize();
	Vector3	c_pos = Vector3(cannon->TransMatrix._41, cannon->TransMatrix._42, cannon->TransMatrix._43); //弾を飛ばす位置を大砲の位置に設定
	SetMove(Vector3(0.0f, move.y, 0.0f));
	Vector3 vec = front;

	//弾を発射するときの情報を決定
	c_pos.y -= 1.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	//発射する弾の種類を決定
	int pattern;
	int rnd = Random::GetInt(0, 99);
	if (0 <= rnd && rnd < 70)	pattern = QuickArts_DATA::NORMAL_SHOT;
	if (70 <= rnd && rnd < 100)	pattern = QuickArts_DATA::TIMER_SHOT;

	if (obj->GetFrame() == PIRATE::MOTION_FRAME::SHOT)
	{
		switch (pattern)
		{
		case QuickArts_DATA::NORMAL_SHOT:
			m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new Pirate_Bullet01, c_pos, vec, bulletSpeed, playerNum);
			break;
		case QuickArts_DATA::TIMER_SHOT:
			m_BulletManager->Set(BULLET_TYPE::PIRATE_02, new Pirate_Bullet02, c_pos, vec, bulletSpeed, playerNum);
			break;
		}
	}
	time++;

	//一秒間硬直
	if (obj->GetFrame() == PIRATE::MOTION_FRAME::SHOT_STOP)
	{
		time = 0;
		return true;
	}
	return	false;
}

//	パワーアーツ
bool	Pirate::PowerArts(void)
{	
	if (attackInfo.t == 0) SetMotion(PIRATE::MOTION_DATA::POWER_START);
	
	if (attackInfo.t > 1.0) SetMotion(PIRATE::MOTION_DATA::POWER_END);

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

	if (attackInfo.t >= 1.3f)	return	true;
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

	//モーションアトデナオス
	SetMotion(6);

	//自分以外のプレイヤー三人を着弾地点にする
	Vector3 target[3];
	for (int i = 0, n = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//自分は除外
		target[n] = characterManager->GetPos(i);
		n++;
	}

	float bulletSpeed = 0.5f;
	Vector3 vec[3];


	//モーションアトデナオス
	if (obj->GetFrame() == 255)
	{
		for (int i = 0; i < 3; i++)
		{
			Parabola(vec[i], b_pos, target[i], bulletSpeed, GRAVITY);
			m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new Pirate_Bullet03, b_pos, vec[i], bulletSpeed, playerNum);
		}
		return true;
	}
		
	return false;
}

//	モーション管理
/*void	Pirate::MotionManagement(int motion)
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
}*/

//	大砲位置設定
void	Pirate::SetCannonPos( void )
{
	Matrix	mat;
	mat = *obj->GetBone( 7 ) * obj->TransMatrix;
	cannon->TransMatrix = mat;
	Matrix	cannonMat = cannon->TransMatrix;
	Vector3	up = Vector3( mat._21, mat._22, mat._23 );
	Vector3	cannonPos = Vector3( 0.0f, 0.0f, 0.0f );
	up.Normalize();
	cannonPos = Vector3( cannon->TransMatrix._41, cannon->TransMatrix._42, cannon->TransMatrix._43 );
	cannonPos += up * 0.5f;
	cannon->TransMatrix._41 = cannonPos.x;
	cannon->TransMatrix._42 = cannonPos.y;
	cannon->TransMatrix._43 = cannonPos.z;
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