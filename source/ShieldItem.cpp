
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"ShieldItem.h"

//***********************************************************************
//
//	ShieldItemクラス
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------

	//	コンストラクタ
	ShieldItem::ShieldItem( void )
	{

	}

	//	デストラクタ
	ShieldItem::~ShieldItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

	//	ヒット時動作
	void	ShieldItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->SetParameterInfo( playerNum, PARAMETER_STATE::UNRIVALEDITEM );
	}
