
#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	< process.h >
#include	"Camera.h"
#include	"sceneLoad.h"
using namespace std;

//*******************************************************************************
//
//	sceneLoad�N���X
//
//*******************************************************************************

//----------------------------------------------------------------------------------
//	�O���[�o��
//----------------------------------------------------------------------------------
	
	//	static�錾
	bool	sceneLoad::threadState;

//----------------------------------------------------------------------------------
//	�������E���	
//----------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	sceneLoad::sceneLoad( Scene*	nextScene ) : newScene( nextScene )
	{
		//	�t���O������
		threadState = false;
		loadflg = false;
	}

	//	�f�X�g���N�^
	sceneLoad::~sceneLoad( void )
	{
		SafeDelete( bgImage[0].obj );
		SafeDelete( bgImage[1].obj );
		SafeDelete( load_anykey.obj );	
	}

	//	������
	bool	sceneLoad::Initialize( void )
	{
		//    ���ݒ�
		iexLight::SetAmbient( 0x808080 );
		iexLight::SetFog( 800, 1000, 0 );

		//	�J�����ݒ�
		mainView = new Camera();

		//	�w�i������
		bgImage[0].obj = new iex2DObj( "DATA/UI/Load/Lord-back01.png" );
		bgImage[1].obj = new iex2DObj( "DATA/UI/Load/Lord-back02.png" );
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth );
		int h = static_cast<int>( iexSystem::ScreenHeight );
		ImageInitialize( bgImage[0], x, y, w, h, 0, 0, 1280, 720 );

		x = static_cast<int>( iexSystem::ScreenWidth * 1.5f );
		ImageInitialize( bgImage[1], x, y, w, h, 0, 0, 1280, 720 );

		//	PressSpace�摜�\���̐ݒ�
		load_anykey.obj = new iex2DObj( "DATA/UI/pressspace.png" );
		x = static_cast<int>( iexSystem::ScreenWidth * 0.86f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.93f );
		w = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.18f );
		ImageInitialize( load_anykey, x, y, w, h, 0, 0, 256, 128 );
		
		//	�ʃX���b�h�쐬
		_beginthread( Thread, 0, ( void* )newScene );
		
		return	true;
	}

//----------------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------------
	
	//	�X�V
	void	sceneLoad::Update( void )
	{

		//	�_�ōX�V
		FlashingUpdate( load_anykey, D3DX_PI / 180 * 4.0f );

		//	�V�[���؂芷��
		if ( threadState ){
			if ( KEY_Get( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 ){

				//	pressspace�g��
				static	float	wavespeed = 1.5f;
				SetWave( load_anykey, wavespeed );
				MainFrame->ChangeScene( newScene, false );
				return;
			}
		}
	}

	//	�`��
	void	sceneLoad::Render( void )
	{
		//	��ʃN���A
		mainView->Activate();
		mainView->Clear();

		//	�w�i�`��
		RenderImage( bgImage[0], bgImage[0].sx, bgImage[0].sy, bgImage[0].sw, bgImage[0].sh, IMAGE_MODE::NORMAL );

		//	pressSpace�`��
		if ( threadState )
		{
			RenderImage( load_anykey, 0, 0, 256, 128, IMAGE_MODE::FLASH );
			RenderImage( load_anykey, 0, 0, 256, 128, IMAGE_MODE::WAVE );
		}
	}

//----------------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------------
	
	//	�X���b�h
	void	sceneLoad::Thread( void* arg )
	{
		//	���񂳂��鏈��
		Scene* scene = ( Scene* )arg;
		scene->Initialize();

		//	�X���b�h�I��
		threadState = true;
		_endthread();
	}