
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"Random.h"
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
#define	IMAGE_MAX		4		//	�摜�ő吔

//	static�錾
bool	sceneLoad::threadState;

//----------------------------------------------------------------------------------
//	�������E���	
//----------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	sceneLoad::sceneLoad( Scene* nextScene ) : newScene( nextScene ), nowLoading( nullptr ),
		t( 0.0f ),
		loadflg(false), isEnd(false), changeSceneFlag(false),
		timer(0), reverseFlag(0), loadingTimer(0), renderCount(0), order(0), num(0)
	{
		//	�摜�֘A
		FOR( 0, IMAGE_MAX )	bgImage[value].obj = nullptr;
		pressAnyKeyImage.obj = nullptr;
		moveImage.obj = nullptr;
		backImage.obj = nullptr;

		//	static�ϐ�������
		threadState = false;
	}

	//	�f�X�g���N�^
	sceneLoad::~sceneLoad( void )
	{
		FOR( 0, IMAGE_MAX )
		{
			SafeDelete( bgImage[value].obj );
		}

		SafeDelete( nowLoading );	
	}

	//	������
	bool	sceneLoad::Initialize( void )
	{
		//    ���ݒ�
		iexLight::SetAmbient( 0x808080 );
		iexLight::SetFog( 800, 1000, 0 );
		
		//	����������
		Random::Initialize();

		//	�J�����ݒ�
		mainView = new Camera();

		//	�摜�ǂݍ���
		Load();

		//	�w�i������
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth );
		int h = static_cast<int>( iexSystem::ScreenHeight );
		FOR( 0, IMAGE_MAX )
		{
			ImageInitialize( bgImage[value], x, y, w, h, 0, 0, 1280, 720 );
		}

		order = Random::GetInt( 0, IMAGE_MAX - 1 );
		int randOrder = Random::GetInt( 0, IMAGE_MAX - 1 );
		while ( randOrder == order )
		{
			randOrder = Random::GetInt( 0, IMAGE_MAX - 1 );
		}
		moveImage = bgImage[randOrder];
		backImage = bgImage[order];

		//	NowLoading, pressAnyKey������
		pressAnyKeyImage.obj = nowLoading;
		ImageInitialize( pressAnyKeyImage,  1380, 865, 350, 150, 0, 128, 512, 128 );
		
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
		return	true;
	}

	//	�ǂݍ���
	void	sceneLoad::Load( void )
	{
		bgImage[0].obj = new iex2DObj( "DATA/UI/Load/Lord-back01.png" );
		bgImage[1].obj = new iex2DObj( "DATA/UI/Load/Lord-back02.png" );
		bgImage[2].obj = new iex2DObj( "DATA/UI/Load/Lord-back03.png" );
		bgImage[3].obj = new iex2DObj( "DATA/UI/Load/Lord-back04.png" );
		nowLoading = new iex2DObj( "DATA/UI/Loading.png" );
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
			if ( KEY_Get( KEY_SPACE ) == 3 || KEY( KEY_B ) == 3 )
			{
				changeSceneFlag = true;
			}
			FlashingUpdate( pressAnyKeyImage, 0.1f );
		}

		//	�V�[���؂�ւ�
		if ( changeSceneFlag )
		{
			MainFrame->ChangeScene( newScene, false );
			return;
		}
	}

	//	�`��
	void	sceneLoad::Render( void )
	{
		//	��ʃN���A
		mainView->Activate();
		mainView->Clear();

		//	�w�i�`��
		RenderImage( backImage, backImage.sx, backImage.sy, backImage.sw, backImage.sh, IMAGE_MODE::NORMAL );
		RenderImage( moveImage, moveImage.sx, moveImage.sy, moveImage.sw, moveImage.sh, IMAGE_MODE::NORMAL );

		//	No��Loading�APressAnyKey�`��
		if ( !threadState )	RenderNowLoading();
		else	RenderImage( pressAnyKeyImage, 
			pressAnyKeyImage.sx, pressAnyKeyImage.sy, 
			pressAnyKeyImage.sw, pressAnyKeyImage.sh, 
			IMAGE_MODE::FLASH );
	}

	//	NowLoading�`��
	void	sceneLoad::RenderNowLoading( void )
	{
		//	���Ԃ��Ȃ��̂ō��W���ł�
		int	plusSw[4] = { 0, 25, 55, 97 };
		int	plusW[4] = { 0, 18, 40, 70 };

		nowLoading->Render( 1230, 790, 300 + plusW[renderCount], 150, 0, 0, 415 + plusSw[renderCount], 128 );
	}

//----------------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------------

	//	NowLoading����
	void	sceneLoad::MoveNowLoading( void )
	{
		loadingTimer++;

		if ( loadingTimer % 30 == 0 )
		{
			renderCount++;
			if ( renderCount > 3 )	renderCount = 1;
		}
	}

	//	�w�i�ړ�
	void	sceneLoad::MoveBG( void )
	{
		isEnd = false;
		static	int	centerX = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int randOrder = Random::GetInt( 0, IMAGE_MAX - 1);

		//	�^�C�}�[���Z
		timer++;

		//	��莞�Ԃ������獷���ւ��X�^�[�g
		if ( timer >= INTERVAL )
		{
			//	�p�����[�^�X�V
			t += 0.01f;
			if ( t >= 1.0f )
			{
				t = 1.0f;
			}

			isEnd = Lerp( moveImage.x, centerX, -centerX, t );

			if ( isEnd )
			{
				moveImage = bgImage[order];

				while ( randOrder == order )
				{
					randOrder = Random::GetInt( 0, IMAGE_MAX - 1 );
				}
				backImage = bgImage[randOrder];
				order = randOrder;
				t = 0.0f;
				timer = 0;
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