
#pragma once

//***********************************************************************
//
//	PowerUpItem�N���X
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class PowerUpItem : public	Item
{
private:
public:
	//	�������E���
	PowerUpItem( void );
	~PowerUpItem( void );

	//	����֐�
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
