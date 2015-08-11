
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
//	グローバル
//--------------------------------------------------------------------------

namespace ECCMANData
{
	enum MotionNum
	{
		POSTURE,				//	構え
		RUN,						//	走り
		ATTACK1,				//	攻撃１段階目
		JUMP,
		LANDING,				//	着地
		ATTACK2,				//	攻撃２段階目
		ATTACK3,				//	攻撃３段階目
		GUARD,					//	ガード
	};
}

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
		motionData.POSTURE = ECCMANData::POSTURE;
		motionData.RUN = ECCMANData::RUN;
		motionData.ATTACK1 = ECCMANData::ATTACK1;
		motionData.JUMP = ECCMANData::JUMP;
		motionData.LANDING = ECCMANData::LANDING;
		motionData.ATTACK2 = ECCMANData::ATTACK2;
		motionData.ATTACK3 = ECCMANData::ATTACK3;
		motionData.GUARD = ECCMANData::GUARD;
		motionData.POSTURE = ECCMANData::POSTURE;
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
		case PlayerData::MOVE:
			Move();
			break;

		case PlayerData::ATTACK:
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
	void	ECCMAN::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )	mode = PlayerData::ATTACK;
		if ( input->Get( KEY_B ) == 3 )	mode = PlayerData::JUMP;
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
		if ( frame >= 410 )		mode = PlayerData::MOVE;
	}

	//	モードJump
	void	ECCMAN::Jump( void )
	{
		CommonJump();
	}
	