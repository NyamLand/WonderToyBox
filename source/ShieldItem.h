
#pragma once

//***********************************************************************
//
//	ShieldItemƒNƒ‰ƒX
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class ShieldItem : public	Item
{
private:
public:
	//	‰Šú‰»E‰ğ•ú
	ShieldItem( void );
	~ShieldItem( void );

	//	“®ìŠÖ”
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
