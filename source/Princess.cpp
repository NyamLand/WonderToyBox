
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Princess.h"

//*********************************************************************************
//
//	Princessクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

	//	コンストラクタ
	Princess::Princess( void )
	{
		//	パラメータ初期化
		attack_r = 0.0f;
		attack_t = 0.0f;
		speed = 0.2f;
		scale = 0.02f;
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
		motionData.STAND = MotionNum::STAND;
		motionData.POSTURE = MotionNum::POSTURE;
		motionData.RUN = MotionNum::RUN;
		motionData.ATTACK1 = MotionNum::ATTACK1;
		motionData.JUMP = MotionNum::JUMP;
		motionData.ATTACK2 = MotionNum::ATTACK2;
		motionData.ATTACK3 = MotionNum::ATTACK3;
		motionData.GUARD = MotionNum::GUARD;
		motionData.POSTURE = MotionNum::POSTURE;
	}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

	//	更新
	void	Princess::Update( void )
	{
		ModeManagement();
		BaseObj::Update();
	}

	//	描画
	void	Princess::Render( iexShader* shader, LPSTR technique )
	{
		BaseObj::Render( shader, technique );

		//	デバッグ用
		DrawSphere( attackPos, attack_r, 0xFFFFFFFF );
	}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

	//	モード管理
	void	Princess::ModeManagement( void )
	{
		switch ( mode )
		{
		case MOVE:
			Move();
			break;

		case ATTACK:
		case	POWERARTS:
		case HYPERARTS:
		case QUICKARTS:
			move = Vector3( 0.0f, 0.0f, 0.0f );
			Attack( mode );
			break;

		case JUMP:
			Jump();
			break;

		case GUARD:
			break;
		}
	}

	//	モードMove
	void	Princess::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )	mode = POWERARTS;
		if ( input->Get( KEY_C ) == 3 )	mode = HYPERARTS;
		if ( input->Get( KEY_D ) == 3 )	mode = JUMP;
	}

	//	クイックアーツ
	bool	Princess::QuickArts( void )
	{
		//	行列から前方取得
		Matrix	mat = obj->TransMatrix;
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();

		Vector3	startPos = Vector3( pos.x, pos.y + 1.5f, pos.z );
		Vector3	finPos = startPos + front * 5.0f;

		//	当たり判定位置移動&範囲拡大
		float t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, attack_t );
		Lerp( attackPos, startPos, finPos, t );
		attack_r = 3.0f * t;

		//	パラメータ加算
		attack_t += 0.015f;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	パワーアーツ
	bool	Princess::PowerArts( void )
	{
		attackPos = Vector3( pos.x, pos.y + 1.5f, pos.z );

		//	範囲拡大
		Lerp( attack_r, 0.0f, 3.0f, attack_t );

		//	パラメータ加算
		attack_t += 0.02f;

		if ( attack_t >= 1.0f )	return	true;
		return	false;
	}

	//	ハイパーアーツ
	bool	Princess::HyperArts( void )
	{
		static	int		num = 0;	//	回数
		attackPos = Vector3( pos.x, pos.y + 1.5f, pos.z );

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

	//	モードAttack
	void	Princess::Attack( int attackKind )
	{
		SetMotion( motionData.ATTACK1 );
		int		frame = obj->GetFrame();

		bool	isEnd = false;

		switch ( attackKind )
		{
		case QUICKARTS:
			isEnd = QuickArts();
			if ( !isEnd )	attackParam = 1;
			break;

		case POWERARTS:
			isEnd = PowerArts();
			if ( !isEnd )	attackParam = 2;
			break;

		case HYPERARTS:
			isEnd = HyperArts();
			if ( !isEnd )	attackParam = 3;
			break;
		}

		//	モーション終了時に
		if ( isEnd )
		{
			mode = MOVE;
			attack_t = 0.0f;
			attack_r = 0.0f;
		}
	}

	//	モードJump
	void	Princess::Jump( void )
	{
		CommonJump();
	}