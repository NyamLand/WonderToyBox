
#pragma once

//***********************************************************************
//
//	LifeItemƒNƒ‰ƒX
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class LifeItem : public	Item
{
private:
public:
	//	‰Šú‰»E‰ğ•ú
	LifeItem( void );
	~LifeItem( void );

	//	“®ìŠÖ”
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
