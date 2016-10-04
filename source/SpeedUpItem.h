
#pragma once

//***********************************************************************
//
//	SpeedUpItemƒNƒ‰ƒX
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class SpeedUpItem : public	Item
{
private:
public:
	//	‰Šú‰»E‰ğ•ú
	SpeedUpItem( void );
	~SpeedUpItem( void );

	//	“®ìŠÖ”
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
