
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"ECCMAN.h"

//*******************************************************************************
//
//	Player_ECCMANクラス
//
//*******************************************************************************

//--------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------

	//	コンストラクタ
	ECCMAN::ECCMAN( void )
	{
		//	パラメータ初期化
		speed = 0.3f;
		scale = 0.02f;
		SetMotionData();
		isGround = true;
	}

	//	デストラクタ
	ECCMAN::~ECCMAN( void )
	{

	}

	//	モーションデータ登録
	void	ECCMAN::SetMotionData( void )
	{
		motionData.POSTURE = MotionNum::POSTURE;
		motionData.RUN = MotionNum::RUN;
		motionData.ATTACK1 = MotionNum::ATTACK1;
		motionData.JUMP = MotionNum::JUMP;
		motionData.LANDING = MotionNum::LANDING;
		motionData.ATTACK2 = MotionNum::ATTACK2;
		motionData.ATTACK3 = MotionNum::ATTACK3;
		motionData.GUARD = MotionNum::GUARD;
		motionData.POSTURE = MotionNum::POSTURE;
	}

//--------------------------------------------------------------------------
//	更新・描画
//--------------------------------------------------------------------------

	//	更新
	void	ECCMAN::Update( void )
	{
		ModeManagement();
		BaseObj::Update();
	}

	//	描画
	void	ECCMAN::Render( iexShader* shader, LPSTR technique )
	{
		BaseObj::Render( shader, technique );
	}

//--------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------

	//	モード管理
	void	ECCMAN::ModeManagement( void )
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
	void	ECCMAN::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = ATTACK;
		if ( input->Get( KEY_B ) == 3 )	mode = JUMP;
	}
	
	//	モードAttack
	void	ECCMAN::Attack( void )
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
	void	ECCMAN::Jump( void )
	{
		CommonJump();
	}
	