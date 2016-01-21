
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

#define	INTERVAL	300		//	�����ւ��̊Ԋu
#define	INTERPOLATION_SPEED		0.01f	//	��ԑ��x

//	static�錾
	bool	sceneLoad::threadState;
	int	x = 0, y = 0;


//----------------------------------------------------------------------------------
//	�������E���	
//----------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	sceneLoad::sceneLoad( Scene*	nextScene ) : newScene( nextScene ), timer( 0 )
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
		SafeDelete( nowLoading );	
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
		ImageInitialize( bgImage[1], x, y, w, h, 0, 0, 1280, 720 );

		//	NowLoading������
		nowLoading = new iex2DObj( "DATA/UI/Loading.png" );
		
		//	�ʃX���b�h�쐬
		_beginthread( Thread, 0, ( void* )newScene );
		
		//	�ϐ�������
		timer = 0;
		t = 0.0f;
		reverseFlag = false;
		isEnd = false;
		changeSceneFlag = false;
		loadingTimer = 0;
		renderCount = 0;
		x = 0;
		y = 0;
		


		return	true;
	}

//----------------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------------
	
	//	�X�V
	void	sceneLoad::Update( void )
	{
		//	�w�i�摜�����ւ�
		MoveBG();

		//	NowLoading�X�V
		MoveNowLoading();

		//	�ǂݍ��݊�����L�[���͎�t
		if ( threadState ){
			if ( KEY_Get( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
			{
				changeSceneFlag = true;
			}
		}

		//	�V�[���؂�ւ�
		if ( changeSceneFlag )
		{
			MainFrame->ChangeScene( newScene, false );
			return;
		}

		if (KEY(KEY_RIGHT) == 1)	x += 10;
		if (KEY(KEY_LEFT) == 1)		x -= 10;
		if (KEY(KEY_UP) == 1)		y -= 10;
		if (KEY(KEY_DOWN) == 1)	y += 10;
	}

	//	�`��
	void	sceneLoad::Render( void )
	{
		//	��ʃN���A
		mainView->Activate();
		mainView->Clear();

		//	�w�i�`��
		if ( reverseFlag )
		{
			RenderImage( bgImage[0], bgImage[0].sx, bgImage[0].sy, bgImage[0].sw, bgImage[0].sh, IMAGE_MODE::NORMAL );
			RenderImage( bgImage[1], bgImage[1].sx, bgImage[1].sy, bgImage[1].sw, bgImage[1].sh, IMAGE_MODE::NORMAL );
		}
		else
		{
			RenderImage( bgImage[1], bgImage[1].sx, bgImage[1].sy, bgImage[1].sw, bgImage[1].sh, IMAGE_MODE::NORMAL );
			RenderImage( bgImage[0], bgImage[0].sx, bgImage[0].sy, bgImage[0].sw, bgImage[0].sh, IMAGE_MODE::NORMAL );
		}

		RenderNowLoading();

		char	str[256];
		sprintf_s( str, "x = %d\ny = %d\n", x, y );
		DrawString( str, 50, 300 );
	}

	//	NowLoading�`��
	void	sceneLoad::RenderNowLoading( void )
	{
		//	���Ԃ��Ȃ��̂ō��W���ł�
		int	plusSw[4] = { 0, 25, 55, 97 };

		nowLoading->Render( 1230, 790, 300 + plusSw[renderCount], 150, 0, 0, 415 + plusSw[renderCount], 128 );
	}

//----------------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------------

	//	NowLoading����
	void	sceneLoad::MoveNowLoading( void )
	{
		loadingTimer++;

		if ( loadingTimer % 10 == 0 )
		{
			renderCount++;
			if ( renderCount > 3 )	renderCount = 0;
		}
	}

	//	�w�i�ړ�
	void	sceneLoad::MoveBG( void )
	{
		static	int	centerX = static_cast<int>( iexSystem::ScreenWidth * 0.5f );

		//	�^�C�}�[���Z
		timer++;

		//	��莞�Ԃ������獷���ւ��X�^�[�g
		if ( timer >= INTERVAL )
		{
			//	�����̉摜�����̈ʒu�ɔz�u
			bgImage[!reverseFlag].x = centerX;

			//	��ԃp�����[�^���Z
			t += INTERPOLATION_SPEED;
			if ( t >= 1.0f )		t = 1.0f;

			//	���
			isEnd = CubicFunctionInterpolation( bgImage[reverseFlag].x, centerX, -centerX, t );

			//	��ԏI���ナ�Z�b�g
			if ( isEnd )
			{
				timer = 0;
				t = 0.0f;
				reverseFlag = !reverseFlag;
			}
		}
	}
	
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