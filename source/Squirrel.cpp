#define	_USE_MATH_DEFINES
#include	<math.h>

#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"BaseObj.h"
#include	"Player.h"
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

	namespace SquirrelData
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
	Squirrel::Squirrel( void )
	{
		//	パラメータ初期化
		attack_r	= 0.0f;
		attack_t	= 0.0f;
		speed		= 0.3f;
		scale			= 0.02f;
		diffence		= -5;
		isGround	= true;
		SetMotionData();
	}

	//	デストラクタ
	Squirrel::~Squirrel( void )
	{

	}

	//	モーションデータ登録
	void	Squirrel::SetMotionData( void )
	{
		motionData.STAND		=	SquirrelData::STAND;
		motionData.POSTURE	=	SquirrelData::POSTURE;
		motionData.RUN			=	SquirrelData::RUN;
		motionData.ATTACK1		=	SquirrelData::ATTACK1;
		motionData.JUMP			=	SquirrelData::JUMP;
		motionData.ATTACK2		=	SquirrelData::ATTACK2;
		motionData.ATTACK3		=	SquirrelData::ATTACK3;
		motionData.GUARD		=	SquirrelData::GUARD;
		motionData.POSTURE	=	SquirrelData::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	更新
	void	Squirrel::Update( void )
	{
		ModeManagement();
		BaseObj::Update();
	}

	//	描画
	void	Squirrel::Render( iexShader* shader, LPSTR technique )
	{
		BaseObj::Render( shader, technique );

		//	デバッグ用
		if ( !debug ) 	return;
		char	str[256];
		DrawSphere( attackPos, attack_r, 0xFFFFFFFF );
	
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

	//	モード管理
	void	Squirrel::ModeManagement( void )
	{
		switch( mode )
		{
		case PlayerData::MOVE:
			Move();
			break;

		case PlayerData::ATTACK:
		case	PlayerData::POWERARTS:
		case PlayerData::HYPERARTS:
		case PlayerData::QUICKARTS:
			unrivaled = true;
			move = Vector3( 0.0f, 0.0f, 0.0f );
			Attack( mode );
			break;

		case PlayerData::JUMP:
			Jump();
			break;

		case PlayerData::GUARD:
			Guard();
			break;

		case PlayerData::DAMAGE_STRENGTH:
			CommonKnockBackStrength();
			break;

		case PlayerData::DAMAGE:
			Damage( mode );
			break;
		}
	}

	//	モードMove
	void	Squirrel::Move( void )
	{
		CommonMove();
		
		if ( input->Get( KEY_A ) == 3 )		mode = PlayerData::QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )		mode = PlayerData::POWERARTS;
		if ( input->Get( KEY_C ) == 3 )		mode = PlayerData::HYPERARTS;
		if ( input->Get( KEY_D ) == 3 )		mode = PlayerData::JUMP;
		if ( input->Get( KEY_B7 ) == 3 )	mode = PlayerData::GUARD;
		if ( input->Get( KEY_B10 ) == 3 )	mode = PlayerData::DAMAGE_STRENGTH;
	}

	//	クイックアーツ
	bool	Squirrel::QuickArts( void )
	{
		static int time = 0;
		
		//	情報取得
		Matrix	mat = GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		Vector3	p_pos = GetPos();
		front.Normalize();

		//	情報設定
		Vector3	vec = front * 5.0f;
		static float bulletSpeed = 0.5f;

		if ( time == 0 )m_BulletManager->Set( p_pos, vec, bulletSpeed );
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
		static int time = 0;

		//	行列から情報取得
		Matrix	mat = GetMatrix();
		Vector3	right = Vector3( mat._11, mat._12, mat._13 );
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();
		right.Normalize();
		Vector3	p_pos = GetPos();

		//	情報設定
		Vector3	vec[3] =
		{
			front * 5.0f + right * 5.0f,
			front * 5.0f,
			front * 5.0f + right * -5.0f
		};
		static float	 bulletSpeed = 0.5f;


		if ( time == 0 )
		{
			for ( int i = 0; i < 3; i++ )
			{
				m_BulletManager->Set( p_pos, vec[i], bulletSpeed );
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
		static int step = 0;		//	回数
		static int time = 0;
	
		//	情報取得
		Matrix	mat = obj->TransMatrix;
		Vector3	up = Vector3( mat._21, mat._22, mat._23 );
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		Vector3	p_pos = GetPos();
		front.Normalize();

		//	情報設定
		Vector3	vec = front * 5.0f + up * -150.0f;
		static	float	bulletScale = 1.0f;
		static	float	bulletSpeed = 0.5f;

		switch ( step )
		{
		case 0:
			move.y += 0.5f;
			time++;
			if ( time >= 20 )
			{
				time = 0;
				step++;
			}
			break;

		case 1:
			angle += 0.1f;
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

		return	false;
	}

	//	モードAttack
	void	Squirrel::Attack( int attackKind )
	{
		SetMotion( motionData.ATTACK1 );
		int		frame = obj->GetFrame();
		
		bool	isEnd = false;

		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			isEnd = QuickArts();
			if ( !isEnd ){
				attackParam = PlayerData::SPHEREVSCAPSULE;
				knockBackType = PlayerData::KNOCKBACK_WEAK;
			}
			break;

		case PlayerData::POWERARTS:
			isEnd = PowerArts();
			if ( !isEnd ){
				attackParam = PlayerData::SPHEREVSCAPSULE;
				knockBackType = PlayerData::KNOCKBACK_MIDDLE;
			}
			break;

		case PlayerData::HYPERARTS:
			isEnd = HyperArts();
			if ( !isEnd ){
				attackParam = PlayerData::SPHEREVSCYRINDER;
				knockBackType = PlayerData::KNOCKBACK_STRENGTH;
			}
			break;
		}

		//	モーション終了時に
		if ( isEnd )
		{
			attack_t = 0.0f;
			attack_r = 0.0f;
			attackParam = 0;
			knockBackType = 0;
			unrivaled = false;
			mode = PlayerData::MOVE;
		}
	}

	//	モードJump
	void	Squirrel::Jump( void )
	{
		CommonJump();
	}

	//	モードGuard
	void	Squirrel::Guard( void )
	{
		move.x = move.z = 0.0f;
		SetMotion( SquirrelData::STAND );
		CommonGuard();
	}

	//	モードDamage
	void	Squirrel::Damage( int type )
	{
		CommonKnockBack();
	}