
#pragma once

//***********************************************************************
//
//	MagnetItemクラス
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class MagnetItem	:	public	Item
{
private:
public:
	//	初期化・解放
	MagnetItem( void );
	~MagnetItem( void );

	//	動作関数
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
