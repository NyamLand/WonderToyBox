
#pragma once

//***********************************************************************
//
//	SpeedUpItem�N���X
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class SpeedUpItem : public	Item
{
private:
public:
	//	�������E���
	SpeedUpItem( void );
	~SpeedUpItem( void );

	//	����֐�
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
