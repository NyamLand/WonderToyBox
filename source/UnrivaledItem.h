
#pragma once

//***********************************************************************
//
//	UnrivaledItemクラス
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class UnrivaledItem  : public	Item
{
private:
public:
	//	初期化・解放
	UnrivaledItem( void );
	~UnrivaledItem( void );

	//	動作関数
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
