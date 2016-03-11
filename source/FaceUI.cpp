#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"NumberUI.h"
#include	"FaceUI.h"

//**********************************************************************
//
//	FaceUI�N���X
//
//**********************************************************************

//------------------------------------------------------------------------------------
//	�O���[�o��
//------------------------------------------------------------------------------------

#define FACEIMAGE_SCALE 256

//------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	FaceUI::FaceUI( void )
	{
	}

	//	�f�X�g���N�^
	FaceUI::~FaceUI(void)
	{
		SafeDelete( face );
	}

	void	FaceUI::Initialize( void )
	{
		FOR( 0, PLAYER_MAX ){
			ImageInitialize( faceImage[value], posXTable[value],
				static_cast<int>( iexSystem::ScreenHeight * 0.1f ),
				NUMBER_SCALE + NUMBER_SCALE,		//	����2�������̉���
				NUMBER_SCALE + NUMBER_SCALE,		//	����2�������̏c��
				0, 0, FACEIMAGE_SCALE, FACEIMAGE_SCALE );
		}
	}

	void	FaceUI::TableInitialize( void )
	{
		//	���W�e�[�u��������
		posXTable[0] = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		posXTable[1] = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
		posXTable[2] = static_cast<int>( iexSystem::ScreenWidth * 0.7f );
		posXTable[3] = static_cast<int>( iexSystem::ScreenWidth * 0.9f );

	}
//------------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------------
	//	�X�V
	void	FaceUI::Update( void )
	{
		FOR( 0, PLAYER_MAX ){
			FaceImageUpdate( value, gameManager->GetCharacterType( value ) );
		}
	}


	//	��摜�\���̍X�V
	void	FaceUI::FaceImageUpdate( int num, int mode )
	{
		faceImage[num].alpha = 0.5f;
		faceImage[num].sy = 256 * mode;
	}

	//	�`��
	void	FaceUI::Render( void )
	{
		FOR( 0, PLAYER_MAX ){
			RenderImage( faceImage[value], faceImage[value].sx, faceImage[value].sy, faceImage[value].sw, faceImage[value].sh, IMAGE_MODE::ADOPTPARAM );
		}
	}