
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"PowerUpItem.h"

//***********************************************************************
//
//	PowerUpItemクラス
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------

	//	コンストラクタ
	PowerUpItem::PowerUpItem( void )
	{

	}

	//	デストラクタ
	PowerUpItem::~PowerUpItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

	//	ヒット時動作
	void	PowerUpItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->SetParameterInfo( playerNum, PARAMETER_STATE::ATTACKUP );
	}
