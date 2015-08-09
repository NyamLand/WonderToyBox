
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Y2009.h"

//*******************************************************************************
//
//	Player_Y2009クラス
//
//*******************************************************************************

//--------------------------------------------------------------------------
//	グローバル
//--------------------------------------------------------------------------

namespace Y2009Data
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
}

//--------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------

	//	コンストラクタ
	Y2009::Y2009( void )
	{
		//	パラメータ初期化
		speed = 0.2f;
		scale = 0.02f;
		SetMotionData();
		isGround = true;
	}
	
	//	デストラクタ
	Y2009::~Y2009( void )
	{
	
	}

	//	モーションデータ登録
	void	Y2009::SetMotionData( void )
	{
		motionData.STAND		=	Y2009Data::STAND;
		motionData.POSTURE	=	Y2009Data::POSTURE;
		motionData.RUN			=	Y2009Data::RUN;
		motionData.ATTACK1		=	Y2009Data::ATTACK1;
		motionData.JUMP			=	Y2009Data::JUMP;
		motionData.ATTACK2		=	Y2009Data::ATTACK2;
		motionData.ATTACK3		=	Y2009Data::ATTACK3;
		motionData.GUARD		=	Y2009Data::GUARD;
		motionData.POSTURE	=	Y2009Data::POSTURE;
	}

//--------------------------------------------------------------------------
//	更新・描画
//--------------------------------------------------------------------------

	//	更新
	void	Y2009::Update( void )
	{
		ModeManagement();
		BaseObj::Update();
	}

	//	描画
	void	Y2009::Render( iexShader* shader, LPSTR technique )
	{
		BaseObj::Render( shader, technique );
	}

//--------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------

	//	モード管理
	void	Y2009::ModeManagement( void )
	{
		switch ( mode )
		{
		case PlayerData::MOVE:
			Move();
			break;

		case PlayerData::ATTACK:
		case PlayerData::QUICKARTS:
		case PlayerData::POWERARTS:
		case PlayerData::HYPERARTS:
			Attack();
			break;

		case PlayerData::JUMP:
			Jump();
			break;

		case PlayerData::GUARD:
			break;
		}
	}

	//	モードMove
	void	Y2009::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = PlayerData::QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )	mode = PlayerData::POWERARTS;
		if ( input->Get( KEY_C ) == 3 )	mode = PlayerData::HYPERARTS;
		if ( input->Get( KEY_D ) == 3 )	mode = PlayerData::JUMP;
	}

	//	モードAttack
	void	Y2009::Attack( void )
	{
		SetMotion( motionData.ATTACK1 );
		int		frame = obj->GetFrame();

		//	攻撃判定
		if ( frame >= 377 && frame <= 385 )		attackParam = 1;
		else attackParam = 0;

		//	モーション終了時に
		if ( frame >= 410 )		mode = PlayerData::MOVE;
	}

	//	モードJump
	void	Y2009::Jump( void )
	{
		CommonJump();
	}

	