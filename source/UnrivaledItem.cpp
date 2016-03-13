
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"UnrivaledItem.h"

//***********************************************************************
//
//	UnrivaledItemクラス
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------

	//	コンストラクタ
	UnrivaledItem::UnrivaledItem( void )
	{

	}

	//	デストラクタ
	UnrivaledItem::~UnrivaledItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

	//	ヒット時動作
	void	UnrivaledItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->SetParameterInfo( playerNum, PARAMETER_STATE::UNRIVALED );
	}
