
#pragma once

//***********************************************************************
//
//	ShieldItem�N���X
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class ShieldItem : public	Item
{
private:
public:
	//	�������E���
	ShieldItem( void );
	~ShieldItem( void );

	//	����֐�
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
