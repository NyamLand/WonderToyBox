
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"LifeUI.h"

//**********************************************************************
//
//	LifeUI�N���X
//
//**********************************************************************

//----------------------------------------------------------------------------
//	�O���[�o��
//----------------------------------------------------------------------------
#define	LIFE_MAX		5

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	LifeUI::LifeUI( void ) : orgLife( nullptr )
	{
		FOR( 0, PLAYER_MAX )
		{
			life[value] = 0;
			lifeImage[value].obj = nullptr;
		}
	}

	//	�f�X�g���N�^
	LifeUI::~LifeUI( void )
	{
		Release();
	}

	//	�ǂݍ���
	void	LifeUI::Load( void )
	{
		orgLife = new iex2DObj( "DATA/UI/NLife.png" );
	}

	//	������
	bool	LifeUI::Initialize( void )
	{
		//	�摜�ǂݍ���
		Load();

		//	�\���̏�����
		FOR( 0, PLAYER_MAX )
		{
			lifeImage[value].obj = orgLife;
			life[value] = gameManager->GetStartLife( value );
			ImageInitialize( lifeImage[value], 0, 0, 75, 75, 0, 0, 64, 64 );
			if( lifeImage[value].obj == nullptr )	return	false;	
		}

		return	true;
	}

	//	���
	void	LifeUI::Release( void )
	{
		SafeDelete( orgLife );
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	LifeUI::Update( void )
	{
		//	�ϐ�����
		Vector3	p_Pos, p_Up, lifePos, out;
		int			culLife;

		//	�S�l���ݒ�
		FOR( 0, PLAYER_MAX )
		{
			//	�\�����W�Z�o
			p_Pos = characterManager->GetPos( value );
			p_Up = characterManager->GetUp( value );
			lifePos = p_Pos + p_Up * 5.0f;
			WorldToClient( lifePos, out, matView * matProjection );

			//	���݂̗͎̑擾
			culLife = characterManager->GetLife( value );

			//	�`��ʒu����
			lifeImage[value].x = static_cast<int>( out.x );
			lifeImage[value].y = static_cast<int>( out.y );

			//	�ǂݍ��݈ʒu�ݒ�
			lifeImage[value].sx = lifeImage[value] .sw * ( ( LIFE_MAX - culLife )  % 4 );
			lifeImage[value].sy = lifeImage[value].sh * ( ( LIFE_MAX - culLife ) / 4 );
		}
	}

	//	�`��
	void	LifeUI::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( lifeImage[value], 
				lifeImage[value].sx, lifeImage[value].sy,
				lifeImage[value].sw, lifeImage[value].sh,
				IMAGE_MODE::NORMAL );
		}
	}
