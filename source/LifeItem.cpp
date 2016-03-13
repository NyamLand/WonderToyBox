
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"LifeItem.h"

//***********************************************************************
//
//	LifeItemクラス
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------

	//	コンストラクタ
	LifeItem::LifeItem( void )
	{

	}

	//	デストラクタ
	LifeItem::~LifeItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

	//	ヒット時動作
	void	LifeItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->AddLife( playerNum );
	}
