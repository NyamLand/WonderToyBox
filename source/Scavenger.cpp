
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"GameManager.h"
#include	"Camera.h"
#include	"Effect.h"
#include	"Scavenger.h"
#include	"Sound.h"
#include	"CharacterManager.h"

//*********************************************************************************
//
//	Scavengerクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------
#define	MIN_INPUT_STATE	300	//	スティック判定最小値

namespace
{
	enum OFFENSIVE_POWER
	{
		QUICK = 1,
		POWER = 1,
		HYPER = 15,
	};
}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

//	コンストラクタ
Scavenger::Scavenger(void) : BaseChara()
{
	//	パラメータ初期化
	power = 0;	/*仮*/
	speed = 0.25f;
	scale = 0.02f;
	diffence = -1;
	stayTime = 0;
	fireBallState = true;
	fireBallStep = 0;
	fireBallInterval = SECOND / 2;
	
	absorb_length = SCAVENGER::DEFAULT_ABSORB_LENGTH;
	isGround = true;
}

//	デストラクタ
Scavenger::~Scavenger(void)
{

}

//	初期化
bool	Scavenger::Initialize(int playerNum, Vector3 pos)
{
	//	親の初期化
	BaseChara::Initialize( playerNum, pos );

	//	モデル読み込み
	if ( obj == nullptr )
		obj = new iex3DObj( "DATA/CHR/majo/majo.IEM" );

	//	スケール設定
	obj->SetScale( 0.02f );
	obj->Update();

	if ( obj == nullptr )	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

//	更新
void	Scavenger::Update( void )
{
	BaseChara::Update();
	fireBallInterval++;
}

//	描画
void	Scavenger::Render(iexShader* shader, LPSTR technique)
{
	BaseChara::Render(shader, technique);

	////	デバッグ用
	//if (!debug)	return;
	DrawCapsule(attackInfo.top, attackInfo.bottom, attackInfo.r, 0xFFFFFFFF);
	DrawSphere( attackInfo.pos, attackInfo.r );
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z - attackInfo.r), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z + attackInfo.r), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x - attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);

	//char	str[256];
	//Vector3	stringPos;
	//WorldToClient(pos, stringPos, matView* matProjection);
	//stringPos.y -= 150.0f;
	//sprintf_s(str, "掃\n除\n屋\n↓");
	//DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

//	クイックアーツ
bool	Scavenger::QuickArts(void)
{
	//if ( fireBallInterval < SECOND / 2 )	return true;
	power = QUICK;
	//	情報取得
	Matrix	mat = obj->TransMatrix;
	Vector3	front = Vector3(mat._31, mat._32, mat._33);
	front.Normalize();
	Vector3	up = Vector3(mat._21, mat._22, mat._23);
	up.Normalize();
	static	Vector3	p_pos = Vector3(0.0f, 0.0f, 0.0f);
	static	bool			initflag = false;
	bool			isEnd = false;
	bool			isHit = false;

	if ( !initflag )
	{
		p_pos = pos + front * 2.0f + up * 2.0f;
		fireBallInterval = 0;
		attackInfo.t = 0.0f;
		attackInfo.r = 1.0f;
		sound->PlaySE( SE::MAJO_QUICK_START );
		initflag = true;
	}

	switch (input->Get(KEY_D))
	{
	case 1:
		if (fireBallState)
		{
			//	ボタンを押してる間進行
			p_pos += front * 0.5f;
			attackInfo.pos = p_pos;
			attackInfo.r = 1.0f;

			//	他のプレイヤーに当たったら発火
			FOR(0, PLAYER_MAX)
			{
				if (value == playerNum)	continue;
				Vector3	p2_pos = characterManager->GetPos(value);
				if (Collision::CapsuleVSSphere(p2_pos, p2_pos + Vector3(0.0f, 3.0f, 0.0f), 2.0f, attackInfo.pos, attackInfo.r))
				{
					sound->PlaySE( SE::MAJO_QUICK_BOMB );
					fireBallState = false;
				}
			}
			particle->FireBall(attackInfo.pos, attackInfo.r * 0.15f, Vector3(0.7f, 0.1f, 0.1f));
		}
		break;

	case 2:
		fireBallState = false;
		sound->PlaySE( SE::MAJO_QUICK_BOMB );
		break;
	}

	if ( !fireBallState )
	{
		power = QUICK;
		attackInfo.t += 0.03f;
		if ( attackInfo.t >= 1.0f )	attackInfo.t = 1.0f;
		isEnd = Lerp( attackInfo.r, 1.0f, 5.0f, attackInfo.t );
		particle->BombFireBall( attackInfo.pos, attackInfo.r * 0.04f, Vector3( 0.5f, 0.1f, 0.1f ) );

		if ( isEnd )
		{
			initflag = false;
			fireBallState = true;
			return	true;
		}
	}
	//power = QUICK;

	//if (obj->GetFrame() < SCAVENGER::MOTION_FRAME::QUICKARTS_SUCK)	SetMotion(SCAVENGER::MOTION_DATA::QUICK_START);
	////if (obj->GetFrame() >= SCAVENGER::MOTION_FRAME::SUCK)	SetMotion(SCAVENGER::MOTION_DATA::QUICK);

	//////平行移動
	//ShiftMove();
	////その場回転
	////RollAngle();

	//SetUnrivaled(false);

	//if (absorb_length < 15.0f) absorb_length += 0.1f;		//吸い込む範囲を徐々に拡大
	//Vector3 p_front = Vector3(sinf(this->angle.y), 0, cosf(this->angle.y));

	////	コイン情報取得
	//list<Coin*>	coinList = coinManager->GetList();
	//FOR_LIST( coinList.begin(), coinList.end() )
	//{
	//	bool	state = ( *it )->GetState();
	//	if ( state )
	//	{
	//		Vector3 toCoinVec = ( *it )->GetPos() - this->pos;
	//		float pVecLength = p_front.Length();
	//		float cVecLength = toCoinVec.Length();
	//		float dot = Vector3Dot( p_front, toCoinVec ) / ( pVecLength * cVecLength );
	//		dot = acos( dot );
	//		dot = dot * 180.0f / D3DX_PI;

	//		Vector3 vec = ( *it )->GetPos() - this->pos;
	//		float length = vec.Length();

	//		vec.Normalize();
	//		if ( dot < 45.0f && length < absorb_length ) 
	//		{
	//			( *it )->SetMove( -vec * 0.6f );
	//		}

	//		particle->Suck(this->pos, this->pos + GetFront() * absorb_length, GetRight(), absorb_length, 0.5f);
	//	}
	//}

	//if ( input->Get( KEY_D ) == 2 )
	//{
	//	SetMotion(SCAVENGER::MOTION_DATA::QUICK_END);
	//	absorb_length = DEFAULT_ABSORB_LENGTH;
	//}


	//if(obj->GetFrame() == SCAVENGER::MOTION_FRAME::QUICKARTS_END) return true;

	return	false;
}

//	パワーアーツ
bool	Scavenger::PowerArts( void )
{	
	static	bool initflag = false;

	if ( !initflag )
	{
		sound->PlaySE( SE::MAJO_POWER );
		initflag = true;
	}

	power = POWER;
	//攻撃モーションでなければモーション設定
	if ( obj->GetFrame() < SCAVENGER::MOTION_FRAME::POWER_TO_WAIT )
	{
		SetMotion( SCAVENGER::MOTION_DATA::POWER_START );
	}

	float run_speed = 0.5f;
	SetUnrivaled(false);

	//	行列から前方取得
	SetMove(Vector3(0.0f, 0.0f, 0.0f));
	Vector3	front = GetFront();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos() + front * 2.0f;

	//	あたり判定のパラメータを与える
	attackInfo.r = 1.0f;
	attackInfo.bottom = p_pos;
	attackInfo.top = attackInfo.bottom + Vector3(0.0f, 3.0f, 0.0f);

	attackInfo.bottom = p_pos - right * 1.0f;
	attackInfo.top = attackInfo.bottom + right * 2.0f;

	//	パラメータ加算
	attackInfo.t += 0.03f;
	absorb_length = 5.0f;
	front.Normalize();
	

	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		bool	state = ( *it )->GetState();
		if ( state )
		{
			Vector3 toCoinVec = ( *it )->GetPos() - this->pos;
			float pVecLength = front.Length();
			float cVecLength = toCoinVec.Length();
			float dot = Vector3Dot( front, toCoinVec ) / ( pVecLength * cVecLength );
			dot = acosf( dot );
			dot = dot * 180.0f / D3DX_PI;

			Vector3 vec = ( *it )->GetPos() - this->pos;
			float length = vec.Length();

			vec.Normalize();
			if ( dot < 90.0f && length < absorb_length)
			{
				( *it )->SetMove( -vec * 2.0f );
			}
		}
	}
	
	if (attackInfo.t < 1.0f)
	{
		//攻撃中モーション固定
		if (obj->GetFrame() >= SCAVENGER::MOTION_FRAME::POWER_TO_WAIT) obj->SetFrame(SCAVENGER::MOTION_FRAME::POWER_TO_WAIT);
		//	後ろ
		particle->Semicircle(this->pos, GetFront(), GetRight(), GetUp(), 0.7f);
		move = front * run_speed;
	}

	if (attackInfo.t >= 1.0f)
	{
		SetMotion(SCAVENGER::MOTION_DATA::POWER_END);
	}

	//モーション終了時にMOVEへ戻す
	if ( obj->GetFrame() == SCAVENGER::MOTION_FRAME::POWERARTS_END )
	{
		initflag = false;
		return	true;
	}
	return	false;
}

//	ハイパーアーツ
bool	Scavenger::HyperArts( void )
{
	power = HYPER;

	static	bool	initflag = false;
	if ( !initflag )
	{
		sound->PlaySE( SE::MAJO_HYPER );
		initflag = true;
	}

	if (obj->GetFrame() <= SCAVENGER::MOTION_FRAME::HYPER_BEGIN) SetMotion(SCAVENGER::MOTION_DATA::HYPER_START);

	SetUnrivaled(false);
	absorb_length = 20.0f;

	m_Effect->StormSet(this->pos + Vector3(0.0f, 2.0f, 0.0f) , 2 * SECOND);
	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		bool	state = ( *it )->GetState();
		if ( state )
		{
			Vector3 vec = ( *it )->GetPos() - this->pos;
			
			//vec.Normalize();
			float length = vec.Length();
			if ( length < absorb_length )
			{
				(*it)->SetAbsorbedFlag(true);
				//初めの一フレームだけ力を与える
				if (obj->GetFrame() == SCAVENGER::MOTION_FRAME::HYPER_BEGIN)
				{
					Vector3 parabola_move;
					Parabola(parabola_move, (*it)->GetPos(), pos, 1.0f, GRAVITY * SCAVENGER::SUCK_POWER);
					(*it)->SetMove(parabola_move);
				}
			}
		}

	}
	//	パラメータ加算
	attackInfo.t += 0.01f;


	if (attackInfo.t < 1.0f)
	{
		if (obj->GetFrame() >= SCAVENGER::MOTION_FRAME::HYPER_ABSORB) obj->SetFrame(SCAVENGER::MOTION_FRAME::HYPER_ABSORB);
	}

	if (attackInfo.t >= 1.0f)
	{
		absorb_length = SCAVENGER::DEFAULT_ABSORB_LENGTH;
	}

	if (obj->GetFrame() == SCAVENGER::MOTION_FRAME::HYPER_FINISH)
	{
		initflag = false;
		return true;
	}

	return	false;
}
void	Scavenger::ShiftMove(void)
{
	//	左スティックの入力チェック
	float	axisX = (float)input->Get(KEY_AXISX);
	float	axisY = (float)input->Get(KEY_AXISY);
	float	length = sqrtf(axisX * axisX + axisY * axisY);

	//	カメラの前方方向を求める
	Vector3	vEye(mainView->GetTarget() - mainView->GetPos());
	float	cameraAngle = atan2f(vEye.x, vEye.z);

	//	入力方向を求める
	float inputAngle = atan2f(axisX, axisY);

	//	目標の角度を求める
	float	targetAngle = cameraAngle + inputAngle;

	Vector3 direction = Vector3(sinf(targetAngle), 0.0f, cosf(targetAngle));

	moveVec = atan2f(direction.x, direction.z);

	if (length > MIN_INPUT_STATE)
	{
		if (!slip.state)
		{
			move.x = sinf(moveVec) * speed / 4;
			move.z = -cosf(moveVec) * speed / 4;
		}
		else
		{
			if (move.Length() < speed)
			{
				move.x += sinf(moveVec) * slipInfo.speed / 4;
				move.z += cosf(moveVec) * slipInfo.speed / 4;
			}
		}
	}
	else
	{
		SetDrag(0.8f);
	}
}


void	Scavenger::RollAngle(void)
{
	//	入力方向を求める
	float	axisX = (float)input->Get(KEY_AXISX);

	if (axisX > 0)
	{
		angle.y += 0.02f;
	}
	if (axisX < 0)
	{
		angle.y -= 0.02f;
	}
}
//	モーション管理
/*void	Scavenger::MotionManagement(int motion)
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
		obj->SetMotion( MOTION_DATA::JUMP );
		break;

	case MOTION_NUM::GUARD:
		obj->SetMotion( MOTION_DATA::GUARD );
		break;

	case MOTION_NUM::LANDING:
		obj->SetMotion( MOTION_DATA::STAND );
		break;

	case MOTION_NUM::RUN:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::ATTACK1:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::ATTACK2:
		obj->SetMotion( MOTION_DATA::RUN );
		break;

	case MOTION_NUM::ATTACK3:
		obj->SetMotion( MOTION_DATA::ATTACK3 );
		break;
	}
}*/

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

//	攻撃用パラメータ設定
void	Scavenger::SetAttackParam(int attackKind)
{
	switch (attackKind)
	{
	case MODE_STATE::QUICKARTS:
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		if (attackInfo.t < 0.6) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hitまでは仰け反りのみ
		if (attackInfo.t >= 0.6) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit目からは吹き飛ばしあり
		break;

	case MODE_STATE::POWERARTS:
		attackInfo.type = Collision::CAPSULEVSCAPSULE;//Collision::SPHEREVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
		break;

	case MODE_STATE::HYPERARTS:
		attackInfo.type = Collision::SPHEREVSCYRINDER;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}