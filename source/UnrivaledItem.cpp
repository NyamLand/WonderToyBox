
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"UnrivaledItem.h"

//***********************************************************************
//
//	UnrivaledItem�N���X
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	UnrivaledItem::UnrivaledItem( void )
	{

	}

	//	�f�X�g���N�^
	UnrivaledItem::~UnrivaledItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------

	//	�q�b�g������
	void	UnrivaledItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->SetParameterInfo( playerNum, PARAMETER_STATE::UNRIVALED );
	}
