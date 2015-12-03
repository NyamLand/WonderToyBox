          
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Princess.h"

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
	Princess::Princess( void ) : BaseChara()
	{
		//	パラメータ初期化
		power = 2;/*仮*/
		speed = 0.25f;
		scale = 0.04f;
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

		//	デバッグ用
		if ( !debug )	return;
		DrawSphere( attackInfo.pos, attackInfo.r, 0xFFFFFFFF );
		particle->BlueFlame( Vector3( attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z - attackInfo.r ), 0.3f );
		particle->BlueFlame( Vector3( attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z + attackInfo.r ), 0.3f );
		particle->BlueFlame( Vector3( attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z ), 0.3f );
		particle->BlueFlame( Vector3( attackInfo.pos.x - attackInfo.r, attackInfo.pos.y, attackInfo.pos.z ), 0.3f );
		
		char	str[256];
		Vector3	stringPos;
		WorldToClient( pos, stringPos, matView* matProjection );
		stringPos.y -= 150.0f;
		sprintf_s( str, "ひ\nめ\n↓" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	クイックアーツ
	bool	Princess::QuickArts( void )
	{
		power = QUICK;

		//相手を仰け反らせる時間(適当)
		leanFrame = 20;
		//	行列から前方取得
		Vector3	front = GetFront();
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );

		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, attackInfo.t );
		Lerp( attackInfo.pos, startPos, finPos, t );
		attackInfo.r = 3.0f * t;

		//	パラメータ加算
		attackInfo.t += 0.015f;

		//	無敵状態
		if ( attackInfo.t <= 0.5f )	unrivaled = true;
		else								unrivaled = false;

		if ( attackInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	パワーアーツ
	bool	Princess::PowerArts( void )
	{
		power = POWER;
		Vector3	p_pos = GetPos();
		attackInfo.pos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );

		//	範囲拡大
		Lerp( attackInfo.r, 0.0f, 3.0f, attackInfo.t );

		//	パラメータ加算
		attackInfo.t += 0.02f;

		//	無敵状態
		if ( attackInfo.t <= 0.5f )		unrivaled = true;
		else										unrivaled = false;

		if ( attackInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	ハイパーアーツ
	bool	Princess::HyperArts( void )
	{
		power = HYPER;

		static	int		num = 0;	//	回数
		SetMove( Vector3( 0.0f, 0.0f ,0.0f ) );
		Vector3	p_pos = GetPos();
		attackInfo.pos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );

		//	範囲拡大
		float t = GetBezier( ePrm_t::eSlow_Lv4, ePrm_t::eRapid_Lv1, attackInfo.t );
		Lerp( attackInfo.r, 0.0f, 50.0f, t );

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
				return	true;
				break;
			}
		}
		return	false;
	}

	//	モーション管理
	void	Princess::MotionManagement( int motion )
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
	}

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

	//	攻撃用パラメータ設定
	void	Princess::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			knockBackInfo.isUp = false;
			attackInfo.type = Collision::SPHEREVSCAPSULE;
			if(attackInfo.t < 0.6) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hitまでは仰け反りのみ
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