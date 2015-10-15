
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Random.h"
#include	"BaseChara.h"
#include	"CharacterManager.h"
#include	"UI.h"
#include	"sceneMain.h"
#include	"sceneLoad.h"
#include	"sceneTitle.h"

#include	"sceneMenu.h"

//*******************************************************************************
//
//	sceneMenu�N���X
//
//*******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneMenu::sceneMenu( void )
	{
	
	}

	//	�f�X�g���N�^
	sceneMenu::~sceneMenu( void )
	{
		Release();
	}

	//	������
	bool	sceneMenu::Initialize( void )
	{
		//	camera
		m_Camera = new Camera();

		3;

		//	random
		Random::Initialize();

		//	screen
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::WHITE_IN, 0.5f );

		//	�摜�ǂݍ���

		//	�摜�\���̏�����
		ImageInitialize( wipeImage, 640, 360, 1280, 720, 0, 0, 2048, 2048 );
		return	true;
	}

	//	���
	void	sceneMenu::Release( void )
	{

	}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

	//	�X�V
	void	sceneMenu::Update( void )
	{

	}

	//	�`��
	void	sceneMenu::Render( void )
	{
		//	camera
		m_Camera->Activate();
		m_Camera->Clear();
	}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	�L�����N�^�[�I��
	void	sceneMenu::SelectCharacter( void )
	{

	}

	//	�X�e�[�W�I��
	void	sceneMenu::SelectStage( void )
	{

	}

	//	�ŏI�m�F
	void	sceneMenu::SelectCheck( void )
	{

	}

	//	���C����
	void	sceneMenu::MoveMain( void )
	{
		MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
		return;
	}

//-------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------