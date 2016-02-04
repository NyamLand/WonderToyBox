
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

namespace OFFENSIVE_POWER
{
	enum 
	{
		QUICK = 0,
		POWER = 0,
		HYPER = 1,
	};
}

namespace DROP_POWER
{
	enum 
	{
		QUICK = 0,
		POWER = 0,
		HYPER = 20,
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
	artsTimer = 0;
	absorb_length = DEFAULT_ABSORB_LENGTH;
	isGround = true;
	armRenderflag = false;
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
	arm = new iexMesh("DATA/Effect/k_arm/k_arm4.imo");
	hand = new iexMesh("DATA/Effect/kq_hand(仮)/hand.IMO");

	arm->SetScale(0.1f);

	if (obj == nullptr)	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

//	描画
void	Thief::Render(iexShader* shader, LPSTR technique)
{
	//SetArmTransform();
	BaseChara::Render(shader, technique);

	if (armRenderflag) arm->Render(shader, technique);

	////	デバッグ用
	//if (!debug)	return;
	//DrawCapsule(attackInfo.bottom, attackInfo.top, attackInfo.r, 0xFFFFFFFF);
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
	if ( !initflag )
	{
		artsTimer = 0;
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
	if (artsTimer == 0/* obj->GetFrame() == ○○ */)
	{
		m_BulletManager->Set(BULLET_TYPE::THIEF_02, new Thief_Bullet02, p_pos, vec, bulletSpeed, playerNum);
	}
	artsTimer++;

		//モーションアトデナオス
		if (obj->GetFrame() == 277)
		{
			artsTimer = 0;
			initflag = false;
			return true;
		}
	return	false;
}

//	ハイパーアーツ
bool	Thief::HyperArts(void)
{
	if ( !initflag )
	{
		sound->PlaySE( SE::HYPER_ATTACK );
		sound->PlaySE( SE::KAITO_HYPER );
		initflag = true;
	}


	//モーションアトデナオス
	SetMotion(6);

	attackInfo.power = OFFENSIVE_POWER::HYPER;
	attackInfo.dropPower = DROP_POWER::HYPER;
	attackInfo.coinDropType = DROP_TYPE::SUCTION;

	SetParameterState(PARAMETER_STATE::UNRIVALED);
	move = Vector3(0, 0 - GRAVITY, 0);	//撃ってる間は静止させる


	//	行列から情報取得
	Vector3	front = GetFront();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//	情報設定
	SetArmTransform();

	//モーションアトデナオス(ちょうどいい感じのフレームが来たら攻撃開始)
	if (obj->GetFrame() >= 339 && obj->GetFrame() < 399)
	{
		float t = GetBezier(ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attackInfo.t);
		Vector3 f = front * (2.0f * sinf(D3DX_PI * t));
		Vector3 r = -right * (2.0f * cosf(D3DX_PI * t));
		attackInfo.bottom = p_pos + f + r;

		switch (HyperStep)
		{
		case 0:
			//	あたり判定のパラメータを与える
			attackInfo.top = attackInfo.bottom + r * HyperRate;
			attackInfo.r = 2.5f;
			HyperRate += 1.0;
			if (HyperRate > 20.0f) HyperStep++;
			break;
		case 1:
			//	パラメータ加算
			attackInfo.top = attackInfo.bottom + f * HyperRate + r * HyperRate;
			attackInfo.t += 0.03f;
			if (attackInfo.t >= 1.0f)
			{
				HyperStep++;
			}
			break;
		case 2:
			//attackInfo.top = attackInfo.bottom + r * rate;
			attackInfo.t = 0.0f;
			HyperRate -= 1.0f;
			break;
		}
		armRenderflag = true;
	}

	Vector3 v1, v2;
	v1 = front;
	v2 = attackInfo.top - attackInfo.bottom;
	v1.Normalize();
	v2.Normalize();
	float armAngle = GetAngle(v1, v2);
	Vector3 cross;
	Vector3Cross(cross, v1, v2);
	if (cross.y < 0) armAngle = -armAngle;

	arm->SetPos(pos);
	arm->SetAngle(angle.y + armAngle);
	arm->SetScale(Vector3(0.03f, 0.03f, HyperRate * 0.01f));
	arm->Update();

	//モーションアトデナオス(終わりのモーションが来たら終了)
	if (/*obj->GetFrame() == 399*/ HyperRate < 0)
	{
		HyperStep = 0;
		HyperRate = 0;
		armRenderflag = false;
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
void	Thief::SetArmTransform(void)
{

}

void	Thief::SetHandTransform(void)
{

}

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
		attackInfo.type = Collision::CAPSULEVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}