
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

//------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------

	//	コンストラクタ
	Y2009::Y2009( void )
	{
		//	パラメータ初期化
		speed = 0.2f;
		scale = 0.02f;
		isGround = true;

		//	モーション番号登録
		SetMotionNum();
	}

	//	デストラクタ
	Y2009::~Y2009( void )
	{

	}
	
	//	モーション番号登録
	void	Y2009::SetMotionNum( void )
	{
		SetMotionData( motionData, MotionType::POSTURE, POSTURE );
		SetMotionData( motionData, MotionType::RUN, RUN );
		SetMotionData( motionData, MotionType::ATTACK1, ATTACK1 );
		SetMotionData( motionData, MotionType::JUMP, JUMP );
		SetMotionData( motionData, MotionType::ATTACK2, ATTACK2 );
		SetMotionData( motionData, MotionType::ATTACK3, ATTACK3 );
		SetMotionData( motionData, MotionType::GUARD, GUARD );
	}


//------------------------------------------------------------------------------
//	更新
//------------------------------------------------------------------------------
	
	//	更新

//------------------------------------------------------------------------------
//	描画
//------------------------------------------------------------------------------

	//	描画

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------

	//	移動

	//	攻撃

	