
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"SpeedUpItem.h"

//***********************************************************************
//
//	SpeedUpItem�N���X
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	SpeedUpItem::SpeedUpItem( void )
	{

	}

	//	�f�X�g���N�^
	SpeedUpItem::~SpeedUpItem( void )
	{

	}

//------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------

	//	�q�b�g������
	void	SpeedUpItem::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		Item::Hitduringtheoperation( pos, playerNum );
		characterManager->SetParameterInfo( playerNum, PARAMETER_STATE::SPEEDUP );
	}
