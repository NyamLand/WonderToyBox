
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Player.h"
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
	Knight::Knight( void )
	{
		//	パラメータ初期化
		attack_r = 0.0f;
		attack_t = 0.0f;
		lance_r = 0.0f;
		power = 2600;
		speed = 0.2f;
		scale = 0.02f;
		SetMotionData();
		isGround = true;
	}

	//	デストラクタ
	Knight::~Knight( void )
	{

	}

	//	モーションデータ登録
	void	Knight::SetMotionData( void )
	{
		motionData.STAND = KnightData::STAND;
		motionData.POSTURE = KnightData::POSTURE;
		motionData.RUN = KnightData::RUN;
		motionData.ATTACK1 = KnightData::ATTACK1;
		motionData.JUMP = KnightData::JUMP;
		motionData.ATTACK2 = KnightData::ATTACK2;
		motionData.ATTACK3 = KnightData::ATTACK3;
		motionData.GUARD = KnightData::GUARD;
		motionData.POSTURE = KnightData::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	描画
	void	Knight::Render( iexShader* shader, LPSTR technique )
	{
		CommonRender( shader, technique );

		//	デバッグ用
		if ( !debug )	return;
		DrawCapsule( attackPos_bottom, attackPos_top, attack_r, 0xFFFFFFFF );

		char	str[256];
		Vector3	stringPos;
		WorldToClient( pos, stringPos, matView* matProjection );
		stringPos.y -= 170.0f;
		sprintf_s( str, "な\nい\nと\n↓" );
		DrawString( str, ( int )stringPos.x, ( int )stringPos.y );
		sprintf_s( str, "x = %f\ny = %f\nz = %f\n", pos.x, pos.y, pos.z );
		DrawString( str, 20, 300 );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	クイックアーツ
	bool	Knight::QuickArts( void )
	{
		//	行列から前方取得
		move = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	front = GetFront();
		Vector3	p_pos = GetPos();
		Vector3	startPos = Vector3( p_pos.x, p_pos.y + 1.5f, p_pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv1, ePrm_t::eSlow_Lv3, attack_t );
		if ( t < 0.5f ){
			Lerp( attackPos_bottom, startPos, finPos, t );
		}
		else{
			Lerp( attackPos_bottom, finPos, startPos, t );
		}
		//	あたり判定のパラメータを与える
		attack_r = 0.5f;
		attackPos_top = attackPos_bottom + front * 2.0f;
	
		//	パラメータ加算
		attack_t += 0.015f;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	パワーアーツ
	bool	Knight::PowerArts( void )
	{
		//行列から前方取得
		move = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		static int step = 0;
		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attack_t );

		switch ( step )
		{
		case 0:
			//	あたり判定のパラメータを与える
			attack_r = 0.5f;
			step++;
			break;
		case 1:
			Vector3	p_pos = GetPos();
			Vector3 f = front * ( 2.0f * sinf( D3DX_PI * t ) );
			Vector3 r = -right * ( 2.0f * cosf( D3DX_PI * t ) );
			attackPos_bottom = p_pos + f + r;
			attackPos_top = attackPos_bottom + f + r;
			//	パラメータ加算
			attack_t += 0.02f;
			break;
		}

		if ( attack_t >= 1.0f ){
			step = 0;
			return	true;
		}
		return	false;
	}

	//	ハイパーアーツ
	bool	Knight::HyperArts( void )
	{
		move = Vector3( 0.0f, 0.0f, 0.0f );
		static	int		num = 0;	//	回数
		//行列から前方取得
		Vector3	front = GetFront();
		Vector3	right = GetRight();
		static int step = 0;
		Vector3 f, r;

		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attack_t );

		Vector3	p_pos = GetPos();

		switch ( step )
		{
		case 0:
			//	あたり判定のパラメータを与える
			attack_r = 0.5f;
			step++;
			break;

		case 1:
			//	右から左へ薙ぎ払い
			f = front * ( 2.0f * sinf( PI * t ) );
			r = right * ( 2.0f * cosf( PI * t ) );
			SetPos(p_pos + front * 0.1f + -right * 0.1f);
			attackPos_bottom = p_pos + f + r;
			attackPos_top = attackPos_bottom + f + r;
			//	パラメータ加算
			attack_t += 0.02f;
			//	薙ぎ払い終えたら次へ
			if ( attack_t >= 1.0f ){
				attack_t = 0.0f;
				step++;
			}
			break;

		case 2:
			//	左から右へ薙ぎ払い
			f = front * ( 2.0f * sinf( PI * t ) );
			r = -right * ( 2.0f * cosf( PI * t ) );
			SetPos(p_pos + front * 0.1f + right * 0.1f);

			attackPos_bottom = p_pos + f + r;
			attackPos_top = attackPos_bottom + f + r;
			//	パラメータ加算
			attack_t += 0.02f;
			//	薙ぎ払い終えたら次へ
			if ( attack_t >= 1.0f ){
				attack_t = 0.0f;
				step++;
			}
			break;

		case 3:
			//	回転切り
			attackPos_bottom.x = p_pos.x + 2.0f * cosf( PI / 180 * lance_r );
			attackPos_bottom.z = p_pos.z + 2.0f * sinf( PI / 180 * lance_r );

			attackPos_top.x = p_pos.x + 4.0f * cosf( PI / 180 * lance_r );
			attackPos_top.z = p_pos.z + 4.0f * sinf( PI / 180 * lance_r );
			lance_r += 10.0f;
			if ( lance_r >= 360 * 5 )
			{
				step = 0;
				lance_r = 0.0f;
				return true;
			}
		}

		return false;
	}

//-----------------------------------------------------------------------------------
//	情報設定
//-----------------------------------------------------------------------------------

	//	攻撃用パラメータ設定
	void	Knight::SetAttackParam( int attackKind )
	{
		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			attackParam = PlayerData::COLLISION_TYPE::CAPSULEVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_WEAK;
			break;

		case PlayerData::POWERARTS:
			attackParam = PlayerData::COLLISION_TYPE::CAPSULEVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_MIDDLE;
			break;

		case PlayerData::HYPERARTS:
			attackParam = PlayerData::COLLISION_TYPE::CAPSULEVSCAPSULE;
			knockBackType = PlayerData::KNOCKBACK_STRENGTH;
			break;
		}
	}




