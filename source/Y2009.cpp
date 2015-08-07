
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
		motionData.STAND		=	MotionNum::STAND;
		motionData.POSTURE	=	MotionNum::POSTURE;
		motionData.RUN			=	MotionNum::RUN;
		motionData.ATTACK1		=	MotionNum::ATTACK1;
		motionData.JUMP			=	MotionNum::JUMP;
		motionData.ATTACK2		=	MotionNum::ATTACK2;
		motionData.ATTACK3		=	MotionNum::ATTACK3;
		motionData.GUARD		=	MotionNum::GUARD;
		motionData.POSTURE	=	MotionNum::POSTURE;
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
		case MOVE:
			Move();
			break;

		case ATTACK:
			Attack();
			break;

		case JUMP:
			Jump();
			break;

		case GUARD:
			break;
		}
	}

	//	モードMove
	void	Y2009::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = ATTACK;
		if ( input->Get( KEY_B ) == 3 )	mode = JUMP;
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
		if ( frame >= 410 )		mode = MOVE;
	}

	//	モードJump
	void	Y2009::Jump( void )
	{
		CommonJump();
	}

	