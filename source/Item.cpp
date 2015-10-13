
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"Particle.h"
#include	"CharacterManager.h"
#include	"GameManager.h"

#include	"Item.h"

//******************************************************************************
//
//	Item�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Item::Item( void )
	{

	}

	//	�f�X�g���N�^
	Item::~Item(void)
	{

	}

	//	������
	bool	Item::Initialize( void )
	{
		Coin::Initialize();
		type = 0;
		scale = 1.0f;
		return	true;
	}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	����
	void	Item::Move( void )
	{
		//	�d�͉��Z
		move.y += GRAVITY;

		//	��]
		angle += 0.05f;
	}

	//	�q�b�g������
	void	Item::Hitduringtheoperation( const Vector3& pos, const int& Num )
	{
		state = false;

		switch ( type )
		{
		case ITEM_TYPE::ATTACK_UP:
			characterManager->SetParameterInfo( Num, PARAMETER_STATE::ATTACKUP );
			break;
		}
		float	effectScale = 0.2f;
		particle->Spark(pos, effectScale);
	
		sound->PlaySE( SE::COIN_SE );
	}

//-------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------

	//	���f���n��
	void	Item::SetMesh( iexMesh* obj )
	{
		this->obj = obj;
	}

	//	��Ԑݒ�
	void	Item::SetState( bool state )
	{
		this->state = state;
	}

	//	�^�C�v�ݒ�
	void	Item::SetType( const int& type )
	{
		this->type = type;
	}

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------

	//	�^�C�v�擾
	int		Item::GetType( void )
	{
		int		out = type;
		return	out;
	}

	bool	Item::GetState( void )
	{
		bool		out = state;
		return	out;
	}