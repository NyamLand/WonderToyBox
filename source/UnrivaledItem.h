
#pragma once

//***********************************************************************
//
//	UnrivaledItem�N���X
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class UnrivaledItem  : public	Item
{
private:
public:
	//	�������E���
	UnrivaledItem( void );
	~UnrivaledItem( void );

	//	����֐�
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
