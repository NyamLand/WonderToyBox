
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
		SafeDelete( load );
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
		load = new iex2DObj( "DATA/UI/Load/Lord-back.png" );
		load_anykey.obj = new iex2DObj( "DATA/UI/pressspace.png" );
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.86f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.93f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.18f );
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
		load->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );

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