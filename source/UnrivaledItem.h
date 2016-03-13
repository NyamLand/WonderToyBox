
#pragma once

//***********************************************************************
//
//	UnrivaledItemƒNƒ‰ƒX
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class UnrivaledItem  : public	Item
{
private:
public:
	//	‰Šú‰»E‰ğ•ú
	UnrivaledItem( void );
	~UnrivaledItem( void );

	//	“®ìŠÖ”
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
