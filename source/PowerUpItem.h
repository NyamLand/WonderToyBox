
#pragma once

//***********************************************************************
//
//	PowerUpItemクラス
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class PowerUpItem : public	Item
{
private:
public:
	//	初期化・解放
	PowerUpItem( void );
	~PowerUpItem( void );

	//	動作関数
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
