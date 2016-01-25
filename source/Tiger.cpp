
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Tiger.h"

//*********************************************************************************
//
//	Tigerクラス
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
			QUICK2 = 3,
			POWER = 0,
			HYPER = 10,
		};
	}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	Tiger::Tiger( void )
	{
		//	パラメータ初期化
		attackInfo.r = 0.5f;
		attackInfo.t = 0.0f;
		speed = 0.2f;
		scale = 0.02f;
		diffence = -1;
		isGround = true;
		attackCount = 0;
		attackFlag = false;
	}

	//	デストラクタ
	Tiger::~Tiger( void )
	{

	}

	//	初期化
	bool	Tiger::Initialize( int playerNum, Vector3 pos )
	{
		//	親の初期化
		BaseChara::Initialize( playerNum, pos );

		//	モデル読み込み
		if ( obj == nullptr )
			obj = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );

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
	void	Tiger::Render( iexShader* shader, LPSTR technique )
	{
		BaseChara::Render( shader, technique );

		//	デバッグ用
		if ( !debug )	return;
		DrawSphere( attackInfo.pos, attackInfo.r, 0xFFFFFFFF );
		particle->BlueFlame( Vector3( attackInfo.pos.x, attackInfo.pos.y, attackInfo.pos.z ), 0.3f ); 

		char	str[256];
		Vector3	stringPos;
		WorldToClient( pos, stringPos, matView* matProjection );
		stringPos.y -= 150.0f;
		sprintf_s( str, "と\nら\n↓" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
		sprintf_s( str, "attackPos.x = %f\nattackPos.y = %f\nattackPos.z = %f", attackInfo.pos.x, attackInfo.pos.y, attackInfo.pos.z );
		DrawString( str, 100, 500, 0xFF00FFFF );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	クイックアーツ
	bool	Tiger::QuickArts( void )
	{		
		power = QUICK;

		//相手を仰け反らせる時間(適当)
		leanFrame = 20;
		//	情報取得
		Vector3	front = GetFront();
		Vector3	up = GetUp();

		//	情報設定
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finLinePos = startPos + front * 5.0f;
		Vector3	finCurvePos = startPos + front * 3.0f + up * 5.0f;
		Vector3	controlPoint = startPos + front * 5.0f + up * 3.5f;
		float	t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv5, attackInfo.t );
		attackInfo.r = 0.5f;
		attackInfo.t += 0.03f;
		if ( attackInfo.t >= 1.0f )	attackInfo.t = 1.0f;

		switch ( attackCount )
		{
		case 0:
		case 1:
		case 2:
		case 3:
			//	線形補間
			Lerp( attackInfo.pos, startPos, finLinePos, t );

			//	動作中にボタン押すと次の攻撃へ
			if ( input->Get( KEY_D ) == 3 )
			{
				if ( !attackFlag )
				{
					if ( attackInfo.t >= 0.3f )
					{
						attackFlag = true;
					}
				}
			}
			break;

		default:
			BezierCurve( attackInfo.pos, startPos, controlPoint, finCurvePos, t );
			break;
		}
		
		if ( attackInfo.t >= 1.0f )
		{
			if ( attackFlag )
			{
				attackFlag = false;
				attackCount++;
				attackInfo.t = 0.0f;
			}
			else
			{
				attackInfo.t = 0.0f;
				attackCount = 0;
				attackInfo.pos = GetPos();
				attackFlag = false;
				return	true;
			}
		}
		return	false;
	}

	//	パワーアーツ
	bool	Tiger::PowerArts( void )
	{
		power = POWER;

		//	無敵
		SetParameterState(PARAMETER_STATE::UNRIVALED);

		//	情報取得
		Vector3	p_pos = GetPos();
		Vector3	front = GetFront();
		Vector3	moveParam = GetMove();
		
		//	動作設定
		switch ( attackCount )
		{
		case 0:			
			moveParam = -front * 0.3f;
			attackCount++;
			break;

		case 1:
			p_pos += moveParam;
			SetDrag( 0.9f );

			if ( moveParam.Length() <= 0.01f )
			{
				attackCount = 0;
				return	true;
			}
			break;
		}

		//	情報更新
		SetMove( moveParam );
		SetPos( p_pos );

		return	false;
	}

	//	ハイパーアーツ
	bool	Tiger::HyperArts( void )
	{
		power = HYPER;

		leanFrame = 60;
		//	情報取得
		static	int		timer = 0;
		Vector3	front = GetFront();
		Vector3	up = GetUp();
		Vector3	right = GetRight();

		//	情報設定
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Vector3	p_pos = GetPos();
		Vector3	startPos_right = p_pos + up * 1.5f + right * 0.5f;
		Vector3	startPos_left = p_pos + up * 1.5f + right * -0.5f;
		Vector3	finPos_right = startPos_right + front * 5.0f;
		Vector3	finPos_left = startPos_left + front * 5.0f;
		float	t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv5, attackInfo.t );
		attackInfo.r = 0.5f;
		attackInfo.t += 0.07f;	//	スピード設定
		if ( attackInfo.t >= 1.0f )	attackInfo.t = 1.0f;

		switch ( attackCount )
		{
		case 20:
			timer++;
			particle->BlueFlame( p_pos, 1.0f );
			
			if ( timer >= 60 )
			{
				attackCount++;
				attackInfo.t = 0.0f;
			}
			break;

		case 21:
			attackInfo.r = 1.0f;
			Lerp( attackInfo.pos, startPos_right, finPos_right, t );

			if ( attackInfo.t >= 1.0f )
			{
				attackInfo.t = 0.0f;
				timer = 0;
				attackCount = 0;
				attackInfo.pos = p_pos;
				attackFlag = false;
				return	true;
			}
			break;

		default:
			//	線形補間
			if ( attackCount % 2 == 0 )		Lerp( attackInfo.pos, startPos_right, finPos_right, t );
			else											Lerp( attackInfo.pos, startPos_left, finPos_left, t );

			//	ゆっくり回転
			if ( input->Get( KEY_AXISX ) < 0 )		angle.y -= 0.02f;
			if ( input->Get( KEY_AXISX ) > 0 )		angle.y += 0.02f;

			if ( attackInfo.t >= 1.0f )
			{
				attackFlag = false;
				attackCount++;
				attackInfo.t = 0.0f;
			}
			break;
		}

		return	false;
	}

	//	モーション管理
/*	void	Tiger::MotionManagement(int motion)
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

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

	//	攻撃用パラメータ設定
	void	Tiger::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			attackInfo.type = Collision::SPHEREVSCAPSULE;
			if (attackCount < 4)
			{
				knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;
			}
			if (attackCount >= 4)
			{
				knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
			}
			break;

		case MODE_STATE::POWERARTS:
			attackInfo.type = 0;
			knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
			break;

		case MODE_STATE::HYPERARTS:
			attackInfo.type = Collision::SPHEREVSCYRINDER;
			if (attackCount < 20) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;
			if (attackCount >= 20) knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			
			break;
		}
	}