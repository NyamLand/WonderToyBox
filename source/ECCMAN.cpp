
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
		isGround = true;

		//	モーション番号振り分け
		SetMotionNum();
	}

	//	デストラクタ
	ECCMAN::~ECCMAN( void )
	{

	}

	//	モーション番号振り分け
	void	ECCMAN::SetMotionNum( void )
	{
		SetMotionData( motionData, MotionType::POSTURE, POSTURE );
		SetMotionData( motionData, MotionType::RUN, RUN );
		SetMotionData( motionData, MotionType::ATTACK1, ATTACK1 );
		SetMotionData( motionData, MotionType::JUMP, JUMP );
		SetMotionData( motionData, MotionType::LANDING, LANDING );
		SetMotionData( motionData, MotionType::ATTACK2, ATTACK2 );
		SetMotionData( motionData, MotionType::ATTACK3, ATTACK3 );
		SetMotionData( motionData, MotionType::GUARD, GUARD );
	}


//--------------------------------------------------------------------------
//	更新
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//	描画
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------