
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"PowerUpItem.h"

//***********************************************************************
//
//	PowerUpItem�N���X
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	PowerUpItem::PowerUpItem( void )
	{

	}

	//	�f�X�g���N�^
	PowerUpItem::~PowerUpItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------

	//	�q�b�g������
	void	PowerUpItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->SetParameterInfo( playerNum, PARAMETER_STATE::ATTACKUP );
	}
