
#pragma once

//***********************************************************************
//
//	LifeItem�N���X
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class LifeItem : public	Item
{
private:
public:
	//	�������E���
	LifeItem( void );
	~LifeItem( void );

	//	����֐�
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
