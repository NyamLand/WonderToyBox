
#pragma once

//***********************************************************************
//
//	SpeedUpItemクラス
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class SpeedUpItem : public	Item
{
private:
public:
	//	初期化・解放
	SpeedUpItem( void );
	~SpeedUpItem( void );

	//	動作関数
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
