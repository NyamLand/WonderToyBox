
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"ShieldItem.h"

//***********************************************************************
//
//	ShieldItem�N���X
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	ShieldItem::ShieldItem( void )
	{

	}

	//	�f�X�g���N�^
	ShieldItem::~ShieldItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------

	//	�q�b�g������
	void	ShieldItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->SetParameterInfo( playerNum, PARAMETER_STATE::UNRIVALEDITEM );
	}
