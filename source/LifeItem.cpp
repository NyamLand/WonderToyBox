
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"LifeItem.h"

//***********************************************************************
//
//	LifeItem�N���X
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	LifeItem::LifeItem( void )
	{

	}

	//	�f�X�g���N�^
	LifeItem::~LifeItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------

	//	�q�b�g������
	void	LifeItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->AddLife( playerNum );
	}
