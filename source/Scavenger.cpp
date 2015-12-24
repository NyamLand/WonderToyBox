
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
		POWER = 5,
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
	power = 3;	/*仮*/
	speed = 0.25f;
	scale = 0.05f;
	diffence = -1;
	stayTime = 0;
	absorb_length = DEFAULT_ABSORB_LENGTH;
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
		obj = new iex3DObj( "DATA/CHR/Knight/Knight_Dammy.IEM" );

	//	スケール設定
	obj->SetScale( 0.05f );
	obj->Update();

	if ( obj == nullptr )	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

//	描画
void	Scavenger::Render(iexShader* shader, LPSTR technique)
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
	sprintf_s(str, "掃\n除\n屋\n↓");
	DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

//	クイックアーツ
bool	Scavenger::QuickArts(void)

{
	power = QUICK;
	////平行移動
	ShiftMove();

	//その場回転
	//RollAngle();

	//無敵判定を切らないとそもそもコインを集められないので無敵切ってます。
	//問題なら言ってください
	unrivaled = false;

	if (absorb_length < 15.0f) absorb_length += 0.1f;		//吸い込む範囲を徐々に拡大
	Vector3 p_front = Vector3(sinf(this->angle.y), 0, cosf(this->angle.y));

	//	コイン情報取得
	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		bool	state = ( *it )->GetState();
		if ( state )
		{
			Vector3 toCoinVec = ( *it )->GetPos() - this->pos;
			float pVecLength = p_front.Length();
			float cVecLength = toCoinVec.Length();
			float dot = Vector3Dot( p_front, toCoinVec ) / ( pVecLength * cVecLength );
			dot = acos( dot );
			dot = dot * 180.0f / D3DX_PI;

			Vector3 vec = ( *it )->GetPos() - this->pos;
			float length = vec.Length();

			vec.Normalize();
			if ( dot < 45.0f && length < absorb_length ) 
			{
				( *it )->SetMove( -vec * 0.2f );
			}

			particle->Suck(this->pos, this->pos + GetFront() * absorb_length, GetRight(), absorb_length, 0.5f);
		}
	}

	if ( input->Get( KEY_D ) == 2 )
	{
		absorb_length = DEFAULT_ABSORB_LENGTH;
		return true;
	}

	return	false;
}

//	パワーアーツ
bool	Scavenger::PowerArts( void )
{	
	power = POWER;

	//無敵判定を切らないとそもそもコインを集められないので無敵切ってます。
	//問題なら言ってください
	unrivaled = false;
	absorb_length = 5.0f;
	Vector3 p_front = Vector3(sinf(this->angle.y), 0, cosf(this->angle.y));
	p_front.Normalize();
	float speed = 0.5f;
	

	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		bool	state = ( *it )->GetState();
		if ( state )
		{
			Vector3 toCoinVec = ( *it )->GetPos() - this->pos;
			float pVecLength = p_front.Length();
			float cVecLength = toCoinVec.Length();
			float dot = Vector3Dot( p_front, toCoinVec ) / ( pVecLength * cVecLength );
			dot = acosf( dot );
			dot = dot * 180.0f / D3DX_PI;

			Vector3 vec = ( *it )->GetPos() - this->pos;
			float length = vec.Length();

			vec.Normalize();
			if ( dot < 90.0f && length < absorb_length && stayTime == 0 )
			{
				( *it )->SetMove( -vec * 2.0f );
			}
		}
	}

	if (attackInfo.t < 1.0f)
	{
		//	後ろ
		particle->Dust(this->pos, GetFront()*-1, GetRight(), 0.5f);
	
		move = p_front * speed;
	}
	//	パラメータ加算
	attackInfo.t += 0.03f;

	if (attackInfo.t >= 1.0f)
	{
		stayTime++;
	}
	
	if (stayTime >= 2 * SECOND)
	{
		stayTime = 0;
		return	true;
	}
	return	false;
}

//	ハイパーアーツ
bool	Scavenger::HyperArts( void )
{
	power = HYPER;

	//無敵判定を切らないとそもそもコインを集められないので無敵切ってます。
	//問題なら言ってください
	unrivaled = false;
	absorb_length = 10.0f;
	stayTime++;

	m_Effect->StormSet(this->pos + Vector3(0.0f, 2.0f, 0.0f) , 2 * SECOND);
	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		bool	state = ( *it )->GetState();
		if ( state )
		{
			Vector3 vec = ( *it )->GetPos() - this->pos;
			
			vec.Normalize();
			float length = vec.Length();
			if ( length < absorb_length )
			{
				Vector3 parabola_move;
				Parabola(parabola_move, (*it)->GetPos(), pos, 0.7f, GRAVITY);
				(*it)->SetAbsorbedFlag(true);
				if (stayTime == 1) (*it)->SetMove(parabola_move);
			}
		}

	}

	if (stayTime > 2 * SECOND)
	{
		stayTime = 0;
		absorb_length = DEFAULT_ABSORB_LENGTH;
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
void	Scavenger::MotionManagement(int motion)
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
}

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

//	攻撃用パラメータ設定
void	Scavenger::SetAttackParam(int attackKind)
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