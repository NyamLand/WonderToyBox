          
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Player.h"
#include	"Princess.h"

//*********************************************************************************
//
//	Princessクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------

	namespace PrincessData
	{
		//	定数
		enum MotionNum
		{
			STAND = 1,			//	立ち
			POSTURE,				//	構え
			RUN = 4,				//	走り
			ATTACK1,				//	攻撃１段階目
			ATTACK2,				//	攻撃２段階目
			ATTACK3,				//	攻撃３段階目
			JUMP,
			GUARD,
		};

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
	Princess::Princess( void )
	{
		//	パラメータ初期化
		attack_r = 0.0f;
		attack_t = 0.0f;
		speed = 0.25f;
		scale = 0.02f;
		diffence = -1;
		SetMotionData();
		isGround = true;
	}

	//	デストラクタ
	Princess::~Princess( void )
	{

	}

	//	モーションデータ登録
	void	Princess::SetMotionData( void )
	{
		motionData.STAND		=		PrincessData::STAND;
		motionData.POSTURE	=		PrincessData::POSTURE;
		motionData.RUN			=		PrincessData::RUN;
		motionData.ATTACK1		=		PrincessData::ATTACK1;
		motionData.JUMP			=		PrincessData::JUMP;
		motionData.ATTACK2		=		PrincessData::ATTACK2;
		motionData.ATTACK3		=		PrincessData::ATTACK3;
		motionData.GUARD		=		PrincessData::GUARD;
		motionData.POSTURE	=		PrincessData::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	描画
	void	Princess::Render( iexShader* shader, LPSTR technique )
	{
		CommonRender( shader, technique );

		//	デバッグ用
		if ( !debug )	return;
		DrawSphere( attackPos, attack_r, 0xFFFFFFFF );
		
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
		//	行列から前方取得
		Matrix	mat = GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();
		move = Vector3( 0.0f, 0.0f, 0.0f );

		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, attack_t );
		Lerp( attackPos, startPos, finPos, t );
		attack_r = 3.0f * t;

		//	パラメータ加算
		attack_t += 0.015f;

		//	無敵状態
		if ( attack_t <= 0.5f )	unrivaled = true;
		else								unrivaled = false;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	パワーアーツ
	bool	Princess::PowerArts( void )
	{
		Vector3	p_pos = GetPos();
		attackPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		move = Vector3( 0.0f, 0.0f, 0.0f );

		//	範囲拡大
		Lerp( attack_r, 0.0f, 3.0f, attack_t );

		//	パラメータ加算
		attack_t += 0.02f;

		//	無敵状態
		if (attack_t <= 0.5f)		unrivaled = true;
		else								unrivaled = false;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	ハイパーアーツ
	bool	Princess::HyperArts( void )
	{
		static	int		num = 0;	//	回数
		move = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos = GetPos();
		attackPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );

		//	範囲拡大
		float t = GetBezier( ePrm_t::eSlow_Lv4, ePrm_t::eRapid_Lv1, attack_t );
		Lerp( attack_r, 0.0f, 50.0f, t );

		//	パラメータ加算
		attack_t += 0.02f;

		if ( attack_t >= 1.0f )
		{
			switch ( num )
			{
			case 0:
				num++;
				attack_t  = 0.0f;
				break;

			case 1:
				num = 0;
				return	true;
				break;
			}
		}
		return	false;
	}

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

	//	攻撃用パラメータ設定
	void	Princess::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			attackParam = PlayerData::SPHEREVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_WEAK;
			break;

		case PlayerData::POWERARTS:
			attackParam = PlayerData::SPHEREVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_MIDDLE;
			break;

		case PlayerData::HYPERARTS:
			attackParam = PlayerData::SPHEREVSCYRINDER;
			knockBackType = PlayerData::KNOCKBACK_STRENGTH;
			break;
		}
	}