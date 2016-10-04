
#pragma once

//***********************************************************************
//
//	MagnetItemƒNƒ‰ƒX
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class MagnetItem	:	public	Item
{
private:
public:
	//	‰Šú‰»E‰ğ•ú
	MagnetItem( void );
	~MagnetItem( void );

	//	“®ìŠÖ”
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
