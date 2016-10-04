
#pragma once

//***********************************************************************
//
//	PowerUpItemƒNƒ‰ƒX
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class PowerUpItem : public	Item
{
private:
public:
	//	‰Šú‰»E‰ğ•ú
	PowerUpItem( void );
	~PowerUpItem( void );

	//	“®ìŠÖ”
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
