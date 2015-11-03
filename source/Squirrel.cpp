#define	_USE_MATH_DEFINES
#include	<math.h>

#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Bullet.h"
#include	"BulletManager.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"Squirrel.h"


//*********************************************************************************
//
//	Squirrelクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------

	namespace
	{
		//	定数
		enum MotionNum
		{
			STAND		= 1,		//	立ち
			POSTURE,				//	構え
			RUN			= 4,		//	走り
			ATTACK1,				//	攻撃１段階目
			ATTACK2,				//	攻撃２段階目
			ATTACK3,				//	攻撃３段階目
			JUMP,
			GUARD,
		};

		enum OFFENSIVE_POWER
		{
			QUICK = 2,
			POWER = 4,
			HYPER = 25,
		};
	}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	Squirrel::Squirrel( void ) : BaseChara()
	{
		//	パラメータ初期化
		attackInfo.r	= 0.0f;
		attackInfo.t	= 0.0f;
		speed		= 0.3f;
		scale			= 0.02f;
		diffence		= -5;
		isGround	= true;
	}

	//	デストラクタ
	Squirrel::~Squirrel( void )
	{

	}

	//	初期化
	bool	Squirrel::Initialize( int playerNum, Vector3 pos )
	{
		//	親の初期化
		BaseChara::Initialize( playerNum, pos );

		//	モデル読み込み
		if ( obj == nullptr )
			obj = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );

		//	スケール設定
		obj->SetScale( 0.02f );
		obj->Update();

		if ( obj == nullptr )	return	false;
		return	true;
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	描画
	void	Squirrel::Render( iexShader* shader, LPSTR technique )
	{
		BaseChara::Render( shader, technique );

		//	デバッグ用
		if ( !debug ) 	return;
		char	str[256];
		DrawSphere( attackInfo.pos, attackInfo.r, 0xFFFFFFFF );
	
		Vector3	stringPos;
		Vector3	p_pos = GetPos();
		WorldToClient( p_pos, stringPos, matView* matProjection );
		stringPos.y -= 100.0f;
		sprintf_s( str, "り\nす\n↓" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
		sprintf_s(str, "x = %f\ny = %f\nz = %f\n", p_pos.x, p_pos.y, p_pos.z);
		DrawString( str, 20, 400 );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	クイックアーツ
	bool	Squirrel::QuickArts( void )
	{
		power = QUICK;

		static int time = 0;
		SetMove( Vector3( 0.0f, move.y, 0.0f ) );
		
		//	情報取得
		Vector3	front = GetFront();
		Vector3	p_pos = GetPos();

		//	情報設定
		Vector3	vec = front * 5.0f;
		static float bulletSpeed = 0.5f;

		int leanpower = 0;
		if ( time == 0 )m_BulletManager->Set( p_pos, vec, bulletSpeed ,leanpower);
		time++;

		if ( time >= 60 )
		{
			time = 0;
			return true;
		}
		return	false;
	}

	//	パワーアーツ
	bool	Squirrel::PowerArts( void )
	{
		power = POWER;

		static int time = 0;

		//	行列から情報取得
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos();
		SetMove( Vector3( 0.0f, move.y, 0.0f ) );

		//	情報設定
		Vector3	vec[3] =
		{
			front * 5.0f + right * 5.0f,
			front * 5.0f,
			front * 5.0f + right * -5.0f
		};
		static float	 bulletSpeed = 0.5f;
		int leanpower = 30;


		if ( time == 0 )
		{
			for ( int i = 0; i < 3; i++ )
			{
				m_BulletManager->Set( p_pos, vec[i], bulletSpeed ,leanpower);
			}
		}
		time++;

		if ( time >= 60 )
		{
			time = 0;
			return true;
		}
		return	false;
	}

	//	ハイパーアーツ
	bool	Squirrel::HyperArts( void )
	{
		power = HYPER;

		static int step = 0;		//	回数
		static int time = 0;
	
		//	情報取得
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Matrix	mat = GetMatrix();
		Vector3	front = GetFront();
		Vector3	up = GetUp();
		Vector3	p_pos = GetPos();
		Vector3	moveParam = GetMove();
		float	angleParam = GetAngle();

		//	情報設定
		Vector3	vec = front * 5.0f + up * -15.0f;
		static	float	bulletScale = 1.0f;
		static	float	bulletSpeed = 0.5f;

		switch ( step )
		{
		case 0:
			moveParam.y += 0.5f;
			p_pos += moveParam;
			time++;
			if ( time >= 20 )
			{
				time = 0;
				step++;
			}
			break;

		case 1:
			angleParam += 0.1f;
			if ( time % 16 == 0 ) m_BulletManager->Set( p_pos, vec, bulletScale, bulletSpeed );
			time++;
			if ( time > 16 * 4 - 1 ) step++;
			break;

		case 2:
			step = 0;
			time = 0;
			return true;
			break;
		}

		//	情報更新
		SetPos( p_pos );
		SetAngle( angleParam );

		return	false;
	}

	//	モーション管理
	void	Squirrel::MotionManagement(int motion)
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
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::GUARD:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::LANDING:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::RUN:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::ATTACK1:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::ATTACK2:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;

		case MOTION_NUM::ATTACK3:
			obj->SetMotion(MOTION_DATA::POSTURE);
			break;
		}
	}

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

	//	攻撃用パラメータ設定
	void	Squirrel::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			attackInfo.type = COLLISION_TYPE::SPHEREVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::WEAK;
			break;

		case MODE_STATE::POWERARTS:
			attackInfo.type = COLLISION_TYPE::SPHEREVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
			break;

		case MODE_STATE::HYPERARTS:
			attackInfo.type =	COLLISION_TYPE::SPHEREVSCYRINDER;
			knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			break;
		}
	}