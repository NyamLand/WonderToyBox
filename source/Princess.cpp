          
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Princess.h"
#include	"CoinManager.h"
#include	"Sound.h"
#include	"GameManager.h"

//*********************************************************************************
//
//	Princessクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------

#define	POWER_ARTS_RADIUS	7.5f

	namespace
	{
		enum OFFENSIVE_POWER
		{
			QUICK = 1,
			POWER = 0,
			HYPER = 0,
		};
	}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	Princess::Princess( void ) : BaseChara()
	{
		//	パラメータ初期化
		power = 2;/*仮*/
		speed = 0.25f;
		scale = 0.06f;
		diffence = -1;
		isGround = true;
	}

	//	デストラクタ
	Princess::~Princess( void )
	{

	}

	//	初期化
	bool	Princess::Initialize( int playerNum, Vector3 pos )
	{
		//	親の初期化
		BaseChara::Initialize( playerNum, pos );

		//	モデル読み込み
		if( obj == nullptr )
			obj = new iex3DObj( "DATA/CHR/プリンセス/prinsess1.IEM" );

		//	スケール設定
		obj->SetScale( scale );
		obj->Update();

		if ( obj == nullptr )	return	false;
		return	true;
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	描画
	void	Princess::Render( iexShader* shader, LPSTR technique )
	{
		BaseChara::Render( shader, technique );

		DrawCapsule( attackInfo.bottom, attackInfo.top, attackInfo.r, 0xFFFFFFFF );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	クイックアーツ
	bool	Princess::QuickArts( void )
	{
		power = 1;

		static	bool	initflag = false;
		if ( !initflag )
		{
			sound->PlaySE( SE::PRINCESS_QUICK );
			initflag = true;
		}

		//SetMotion(PRINCESS::MOTIO_DATA::QUICK);

		//	行列から前方取得
		Vector3	front = GetFront();
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );

		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, attackInfo.t );
		Lerp( attackInfo.pos, startPos, finPos, t );
		attackInfo.r = 6.0f * t;

		//	パラメータ加算
		attackInfo.t += 0.015f;

		//	エフェクト
		particle->Flower( attackInfo.pos, attackInfo.r, Vector3( 1.0f, 0.4f, 0.4f ) );

		//相手を混乱状態に
		attackInfo.addParam = PARAMETER_STATE::CONFUSION;
		//	無敵状態
		if (attackInfo.t <= 0.5f) SetParameterState(PARAMETER_STATE::UNRIVALED);
		else					SetUnrivaled(false);

		if ( attackInfo.t >= 1.0f )
		{
			initflag = false;
			return	true;
		}
		return	false;
	}

	//	パワーアーツ
	bool	Princess::PowerArts( void )
	{
		static	bool	initflag = false;
		if ( !initflag )
		{
			sound->PlaySE( SE::PRINCESS_POWER );
			initflag = true;
		}
		power = 0;
		Vector3	p_pos = GetPos();
		attackInfo.pos = Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z );
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );

		//	範囲拡大
		Lerp( attackInfo.r, 0.0f, POWER_ARTS_RADIUS, attackInfo.t );

		//	パラメータ加算
		attackInfo.t += 0.02f;

		//	エフェクト
		particle->FlowerDisseminate( attackInfo.pos, attackInfo.r, 0.7f, Vector3( 1.0f, 0.4f, 0.4f ) );

		//範囲内にあるコインを取得
		list<Coin*>	coinList = coinManager->GetList();
		FOR_LIST(coinList.begin(), coinList.end())
		{
			bool	state = (*it)->GetState();
			if ( state )
			{
				if ( Collision::DistCheck( attackInfo.pos, (*it)->GetPos(), attackInfo.r ) )
				{
					(*it)->SetState( false );
					gameManager->AddCoin( playerNum );
				}
			}
		}

		//	無敵状態
		if (attackInfo.t <= 0.5f)		SetParameterState(PARAMETER_STATE::UNRIVALED);
		else							SetUnrivaled(false);

		if ( attackInfo.t >= 1.0f )
		{
			initflag = false;
			return	true;
		}
		return	false;
	}

	//	ハイパーアーツ
	bool	Princess::HyperArts( void )
	{
		power = HYPER;

		static	bool	initflag = false;
		if ( !initflag )
		{
			sound->PlaySE( SE::HYPER_ATTACK );
			sound->PlaySE( SE::PRINCESS_HYPER );
			initflag = true;
		}

		static	int		num = 0;	//	回数
		SetMove( Vector3( 0.0f, 0.0f ,0.0f ) );
		Vector3	p_pos = GetPos();
		attackInfo.top = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		attackInfo.bottom = Vector3( p_pos.x, p_pos.y - 1.5f, p_pos.z );
		attackInfo.pos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );

		//	範囲拡大
		float t = GetBezier( ePrm_t::eSlow_Lv4, ePrm_t::eRapid_Lv1, attackInfo.t );
		Lerp( attackInfo.r, 0.0f, 50.0f, t );

		//	エフェクト
		particle->FlowerDisseminate( attackInfo.pos, attackInfo.r, 2.0f, Vector3( 1.0f, 0.4f, 0.4f ) );
		//相手を混乱状態に
		attackInfo.addParam = PARAMETER_STATE::CONFUSION;
		//	パラメータ加算
		attackInfo.t += 0.02f;

		if ( attackInfo.t >= 1.0f )
		{
			switch ( num )
			{
			case 0:
				num++;
				attackInfo.t  = 0.0f;
				break;

			case 1:
				num = 0;
				initflag = false;
				return	true;
				break;
			}
		}
		return	false;
	}

	//	モーション管理
/*	void	Princess::MotionManagement( int motion )
	{
		switch ( motion )
		{
		case MOTION_NUM::STAND:
			obj->SetMotion( MOTION_DATA::STAND );
			break;

		case MOTION_NUM::POSTURE:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::RUN:
			obj->SetMotion( MOTION_DATA::RUN );
			break;

		case MOTION_NUM::JUMP:
			obj->SetMotion( MOTION_DATA::JUMP );
			break;

		case MOTION_NUM::GUARD:
			obj->SetMotion( MOTION_DATA::GUARD );
			break;

		case MOTION_NUM::FALLTOGROUND:
			obj->SetMotion( MOTION_DATA::FALLTOGROUND );
			break;

		case MOTION_NUM::ATTACK1:
			obj->SetMotion( MOTION_DATA::ATTACK1 );
			break;

		case MOTION_NUM::ATTACK2:
			obj->SetMotion( MOTION_DATA::ATTACK2 );
			break;

		case MOTION_NUM::GETUP:
			SetMotion( MOTION_DATA::GETUP );
			break;

		case MOTION_NUM::KNOCKBACK:
			SetMotion( MOTION_DATA::KNOCKBACK );
			break;
		}
	}*/

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

	//	攻撃用パラメータ設定
	void	Princess::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			attackInfo.type = Collision::SPHEREVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::WEAK;
			break;

		case MODE_STATE::POWERARTS:
			//プレイヤーとの判定なし
			break;

		case MODE_STATE::HYPERARTS:
			attackInfo.type = Collision::CAPSULEVSCYRINDER;
			knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			break;
		}
	}