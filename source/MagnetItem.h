
#pragma once

//***********************************************************************
//
//	MagnetItem�N���X
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class MagnetItem	:	public	Item
{
private:
public:
	//	�������E���
	MagnetItem( void );
	~MagnetItem( void );

	//	����֐�
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
