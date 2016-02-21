
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Crown.h"

//**********************************************************************
//
//	Crown�N���X
//
//**********************************************************************

//------------------------------------------------------------------------------------
//	�O���[�o��
//------------------------------------------------------------------------------------

#define	CROWN_DIST	64

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Crown::Crown( void ) : orgCrown( nullptr )
	{
		FOR( 0, PLAYER_MAX )
		{
			state[value] = false;
			crownImage[value].obj = nullptr;
		}
	}

	//	�f�X�g���N�^
	Crown::~Crown( void )
	{
		Release();
	}

	//	������
	bool	Crown::Initialize( void )
	{
		//	�ǂݍ���
		Load();

		//	�\���̏�����
		FOR( 0, PLAYER_MAX )
		{
			ImageInitialize( crownImage[value], 0, 0, 80, 80, 0, 0, 512, 512 );
			crownImage[value].obj = orgCrown;
			if ( crownImage[value].obj == nullptr )	return	false;
		}

		return	true;
	}

	//	���
	void	Crown::Release( void )
	{
		SafeDelete( orgCrown );
	}

	//	�ǂݍ���
	void	Crown::Load( void )
	{
		orgCrown = new iex2DObj( "DATA/UI/1stCrown.png" );
	}

//------------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------------

	//	�X�V
	void	Crown::Update( void )
	{
		Vector3	p_Pos;
		Vector3	p_Up;
		Vector3	crownPos;
		Vector3	out;

		FOR( 0, PLAYER_MAX )
		{
			//	1�ʈȊO�͂Ƃ΂�
			if ( characterManager->GetRank( value ) > 1 )
			{
				state[value] = false;
				continue;
			}

			//	�\����Ԃ�
			state[value] = true;

			//	�\�����W�Z�o
			p_Pos = characterManager->GetPos( value );
			p_Up = characterManager->GetUp( value );
			crownPos = p_Pos + p_Up * 5.0f;
			WorldToClient( crownPos, out, matView *matProjection );

			//	�`��ʒu�ݒ�
			crownImage[value].x = static_cast<int>( out.x );
			crownImage[value].y = static_cast<int>( out.y - CROWN_DIST );
		}
	}

	//	�`��
	void	Crown::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			if ( state[value] )
			{
				RenderImage( crownImage[value], 
					crownImage[value].sx, crownImage[value].sy, 
					crownImage[value].sw, crownImage[value].sh, 
					IMAGE_MODE::NORMAL );
			}
		}
	}
