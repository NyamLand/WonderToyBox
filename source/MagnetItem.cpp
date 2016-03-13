
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"MagnetItem.h"

//***********************************************************************
//
//	MagnetItemクラス
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------

	//	コンストラクタ
	MagnetItem::MagnetItem( void )
	{
	
	}

	//	デストラクタ
	MagnetItem::~MagnetItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

	//	ヒット時動作
	void	MagnetItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->SetParameterInfo( playerNum, PARAMETER_STATE::MAGNET );
	}
