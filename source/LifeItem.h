
#pragma once

//***********************************************************************
//
//	LifeItemクラス
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class LifeItem : public	Item
{
private:
public:
	//	初期化・解放
	LifeItem( void );
	~LifeItem( void );

	//	動作関数
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
