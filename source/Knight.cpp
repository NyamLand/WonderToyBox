
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"

#include	"Knight.h"

//*********************************************************************************
//
//	Knightクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------
	namespace KnightData
	{
		enum OFFENSIVE_POWER
		{
			QUICK0 = 1,
			QUICK1 = 2,
			QUICK2 = 4,
			POWER = 8,
			HYPER = 5,
		};
	}

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	Knight::Knight( void ) : BaseChara()
	{
		//	パラメータ初期化
		lance_r = 0.0f;
		power = 5;
		speed = 0.2f;
		scale = 0.1f;
		isGround = true;
	}

	//	デストラクタ
	Knight::~Knight( void )
	{

	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	描画
	void	Knight::Render( iexShader* shader, LPSTR technique )
	{
		BaseChara::Render( shader, technique );

		DrawCapsule( attackInfo.bottom, attackInfo.top, attackInfo.r, 0xFFFFFFFF );
		particle->BlueFlame( attackInfo.top, 0.3f );
		particle->BlueFlame( attackInfo.bottom, 0.3f );




		char	str[256];
		Vector3	stringPos;
		WorldToClient( pos, stringPos, matView* matProjection );
		stringPos.y -= 170.0f;
		sprintf_s( str, "な\nい\nと\n↓" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
		sprintf_s( str, "x = %f\ny = %f\nz = %f\n", pos.x, pos.y, pos.z );
		DrawString( str, 20, 300 );
		sprintf_s( str, "isGround = %d", isGround );
		DrawString( str, 20, 500 );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	クイックアーツ
	bool	Knight::QuickArts( void )
	{
		//相手を仰け反らせる時間(適当)
		leanFrame = 20;
		//	行列から前方取得
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Vector3	front = GetFront();
		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv1, ePrm_t::eSlow_Lv3, attackInfo.t );
		if ( t < 0.5f ){
			Lerp( attackInfo.bottom, startPos, finPos, t );
		}
		else{
			Lerp( attackInfo.bottom, finPos, startPos, t );
		}
		//	あたり判定のパラメータを与える
		attackInfo.r = 0.5f;
		attackInfo.top = attackInfo.bottom + front * 2.0f;
	
		//	パラメータ加算
		attackInfo.t += 0.015f;

		if ( attackInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	パワーアーツ
	bool	Knight::PowerArts( void )
	{
		//行列から前方取得
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos();
		static int step = 0;
		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attackInfo.t );
		Vector3 f = front * ( 2.0f * sinf( D3DX_PI * t ) );
		Vector3 r = -right * ( 2.0f * cosf( D3DX_PI * t ) );
		attackInfo.bottom = p_pos + f + r;
		attackInfo.top = attackInfo.bottom + f + r;

		switch ( step )
		{
		case 0:
			//	あたり判定のパラメータを与える
			attackInfo.r = 0.5f;
			step++;
			break;
		case 1:
			//	パラメータ加算
			attackInfo.t += 0.02f;
			break;
		}

		if ( attackInfo.t >= 1.0f ){
			step = 0;
			return	true;
		}
		return	false;
	}

	//	ハイパーアーツ
	bool	Knight::HyperArts( void )
	{
		//相手を仰け反らせる時間(適当)
		leanFrame = 35;
		SetMove( Vector3( 0.0f, 0.0f, 0.0f ) );
		static	int		num = 0;	//	回数
		//行列から前方取得
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		Vector3	p_pos = GetPos();
		static int step = 0;
		Vector3 f, r;

		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attackInfo.t );


		switch ( step )
		{
		case 0:
			//	あたり判定のパラメータを与える
			attackInfo.r = 0.5f;
			step++;
			break;

		case 1:
			//	パラメータ加算
			attackInfo.t += 0.02f;
			
			//	右から左へ薙ぎ払い
			f = front * ( 2.0f * sinf( D3DX_PI * t ) );
			r = right * ( 2.0f * cosf( D3DX_PI * t ) );
			p_pos += front * 0.1f + -right * 0.1f;
			attackInfo.bottom = p_pos + f + r;
			attackInfo.top = attackInfo.bottom + f + r;
			
			//	薙ぎ払い終えたら次へ
			if ( attackInfo.t >= 1.0f ){
				attackInfo.t = 0.0f;
				step++;
			}
			break;

		case 2:
			//	左から右へ薙ぎ払い
			f = front * ( 2.0f * sinf( D3DX_PI * t ) );
			r = -right * ( 2.0f * cosf( D3DX_PI * t ) );
			p_pos += front * 0.1f + right * 0.1f;
			attackInfo.bottom = p_pos + f + r;
			attackInfo.top = attackInfo.bottom + f + r;
			//	パラメータ加算
			attackInfo.t += 0.02f;
			//	薙ぎ払い終えたら次へ
			if ( attackInfo.t >= 1.0f ){
				attackInfo.t = 0.0f;
				step++;
			}
			break;

		case 3:
			//	回転切り
			attackInfo.bottom.x = p_pos.x + 2.0f * cosf( D3DX_PI / 180 * lance_r );
			attackInfo.bottom.z = p_pos.z + 2.0f * sinf( D3DX_PI / 180 * lance_r );
			attackInfo.top.x = p_pos.x + 4.0f * cosf( D3DX_PI / 180 * lance_r );
			attackInfo.top.z = p_pos.z + 4.0f * sinf( D3DX_PI / 180 * lance_r );
			lance_r += 10.0f;
			if ( lance_r >= 360 * 5 )
			{
				step = 0;
				lance_r = 0.0f;
				return true;
			}
			break;
		}

		SetPos( p_pos );

		return false;
	}

	//	モーション管理
	void	Knight::MotionManagement( int motion )
	{
		switch ( motion )
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
	void	Knight::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case MODE_STATE::QUICKARTS:
			attackInfo.type = COLLISION_TYPE::CAPSULEVSCAPSULE;
			if (attackInfo.t < 0.6f) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hitまでは仰け反りのみ
			if (attackInfo.t >= 0.6f && attackInfo.t < 0.8f ) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit目からは吹き飛ばしあり
			break;

		case MODE_STATE::POWERARTS:
			attackInfo.type = COLLISION_TYPE::CAPSULEVSCAPSULE;
			knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
			break;

		case MODE_STATE::HYPERARTS:
			attackInfo.type = COLLISION_TYPE::CAPSULEVSCAPSULE;
			if (attackInfo.t != 0) knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;	//2Hitまでは吹き飛ばしあり
			if (attackInfo.t == 0 && lance_r < 360 * 4) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;		//3~6hit目からは吹き飛ばしあり
			if (attackInfo.t == 0 && lance_r >= 360 * 4) knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;		//6hit目からは吹き飛ばしあり
			break;
		}
	}




