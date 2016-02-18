
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
#include	"Curtain.h"
#include	"sceneMain.h"
#include	"sceneMenu.h"
#include	"sceneLoad.h"

#include	"sceneTitle.h"

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

namespace
{
	namespace TITLE_STEP
	{
		enum
		{
			WAIT_SCREEN,
			WAIT_INPUT,
			WAIT_OPEN,
			NEXT_MODE,
		};
	}
}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneTitle::sceneTitle( void )	: mode( TITLE_MODE::TITLE )
	{

	}
	
	//	�f�X�g���N�^
	sceneTitle::~sceneTitle( void )
	{
		Release();
	}
	
	//	������
	bool	sceneTitle::Initialize( void )
	{
		//	���ݒ�
		iexLight::SetAmbient( 0x404040 );
		iexLight::SetFog( 800, 1000, 0 );

		//	���C�g�ݒ�
		Vector3 dir( 0.0f, -1.0f, -0.0f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 1.0f, 1.0f, 1.0f );

		//	�ǂݍ���
		Load();

		//	�J�����ݒ�
		mainView = new Camera();

		//	�J�[�e��������
		curtain = new Curtain();

		//	�}�l�[�W���[������
		gameManager->Initialize();

		//	�p�[�e�B�N��������
		particle->Initialize();

		//	�X�N���[��������
		screen->SetScreenMode( SCREEN_MODE::FADE_IN, 1.0f );

		//	�X�e�[�W	
		stage->SetAngle( D3DX_PI );
		stage->SetScale( 0.1f );
		stage->Update();

		//	����������
		Random::Initialize();		

		//	�e���[�h������
		TitleInitialize();
		MenuInitialize();
		CreditInitialize();

		//	BGM�ݒ�Đ�
		sound->PlayBGM( BGM::TITLE_BGM );
		return	true;
	}

	//	�^�C�g��������
	void	sceneTitle::TitleInitialize( void )
	{
		//	�摜�\���̏�����
		ImageInitialize( titleInfo.pressSpace, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.82f ), static_cast<int>( iexSystem::ScreenWidth * 0.2f ), static_cast<int>( iexSystem::ScreenHeight * 0.15f ), 0, 0, 256, 128 );
		ImageInitialize( titleInfo.titleImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.42f ), static_cast<int>( iexSystem::ScreenWidth * 0.4f ), static_cast<int>( iexSystem::ScreenHeight * 0.7f ), 0, 0, 512, 512 );
		ImageInitialize( titleInfo.gameStartImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.80f ), static_cast<int>( iexSystem::ScreenWidth * 0.15f ), static_cast<int>( iexSystem::ScreenHeight * 0.07f ), 0, 0, 512, 128 );
		ImageInitialize( titleInfo.creditTextImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.87f ), static_cast<int>( iexSystem::ScreenWidth * 0.1f ), static_cast<int>( iexSystem::ScreenHeight * 0.07f ), 0, 128, 512, 128 );
		titleInfo.gameStartImage.obj = titleInfo.titleText;
		titleInfo.creditTextImage.obj = titleInfo.titleText;

		//	�J������񏉊���
		cameraInfo.state = 0;

		//	�ϐ�������
		titleInfo.titleImage.renderflag = true;
		titleInfo.step = 0;
		titleInfo.nextMode = 0;
		titleInfo.changeflag = false;
		titleInfo.curtainState = false;
		titleInfo.speed = 0.5f;
	}

	//	���j���[������
	void	sceneTitle::MenuInitialize( void )
	{
		menuInfo.menu_num = 0;
	}

	//	�N���W�b�g������
	void	sceneTitle::CreditInitialize( void )
	{		
		creditInfo.sy = 0;
		creditInfo.t = 0.0f;
	}

	//	�ǂݍ���
	bool	sceneTitle::Load( void )
	{
		//	�X�e�[�W
		PointerNew( stage, "DATA/BG/title_stageV2/title_stagev2.IMO" );

		//	�^�C�g���摜
		PointerNew( titleInfo.titleImage.obj, "DATA/UI/title.png" );
		PointerNew( titleInfo.pressSpace.obj, "DATA/UI/pressspace.png" );
		PointerNew( titleInfo.titleText, "DATA/UI/title/titleText.png" );

		//	�N���W�b�g�p�摜
		PointerNew( creditInfo.credit, "DATA/UI/title/credit.png" );

		return	true;
	}

	//	���
	void	sceneTitle::Release( void )
	{
		SafeDelete( curtain );
		SafeDelete( mainView );
		SafeDelete( titleInfo.titleImage.obj );
		SafeDelete( titleInfo.pressSpace.obj );
		SafeDelete( stage );
		SafeDelete( titleInfo.titleText );
		SafeDelete( creditInfo.credit );
		sound->AllStop();
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�X�V
	void	sceneTitle::Update( void )
	{
		curtain->Update();

		//	�e���[�h����
		switch ( mode )
		{
		case TITLE_MODE::TITLE:
			TitleUpdate();
			break;
		
		case TITLE_MODE::MENU:
			MenuUpdate();
			break;

		case TITLE_MODE::PLAY:
			MoveSceneUpdate();
			break;

		case TITLE_MODE::CREDIT:
			CreditUpdate();
			break;
		}

		//	�^�[�Q�b�g�ʒu�Ƀp�[�e�B�N���z�u
		for ( int i = 0; i < TITLE_TARGET::END; i++)	particle->BlueFlame( TITLE_MOVE_INFO::target[i] );
		particle->Update();
		
		//	�X�N���[���X�V
		screen->Update();
	}

	//	�`��
	void	sceneTitle::Render( void )
	{
		//	��ʃN���A
		mainView->Activate();
		mainView->Clear( 0xFF02B4FE );

		//	�X�e�[�W�`��
		stage->Render();

		//	�J�[�e���`��
		curtain->Render();

		//	�p�[�e�B�N���`��
		//particle->Render();

		switch ( mode )
		{
		case TITLE_MODE::TITLE:
			TitleRender();
			break;

		case TITLE_MODE::MENU:
			MenuRender();
			break;

		case TITLE_MODE::PLAY:
			MoveSceneRender();
			break;

		case TITLE_MODE::CREDIT:
			CreditRender();
			break;
		}

		//	�X�N���[���`��
		screen->Render();
	}

//******************************************************************
//�@�e��ʂ̃��\�b�h
//******************************************************************

	//--------------------------------------------------------
	//	�^�C�g��
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::TitleUpdate( void )
		{
			static	float flashingSpeed = D3DX_PI / 180 * 10.0f;
			switch ( titleInfo.step )
			{
			case TITLE_STEP::WAIT_SCREEN:
				//	�t�F�[�h�����I���܂ő҂�
				if ( screen->GetScreenState() )	titleInfo.step++;
				break;

			case TITLE_STEP::WAIT_INPUT:
				//	�J�����X�V
				mainView->Update( VIEW_MODE::SETUP );

				//	�_�ōX�V
				FlashingUpdate( titleInfo.pressSpace, D3DX_PI / 180 * 4.0f );

				//	�J�[�\���ړ�
				if ( input[0]->Get( KEY_UP ) == 3 )
				{
					titleInfo.nextMode--;
					if ( titleInfo.nextMode < 0 )	titleInfo.nextMode = 1;
				}
				if ( input[0]->Get( KEY_DOWN ) == 3 )
				{
					titleInfo.nextMode++;
					if ( titleInfo.nextMode >= 2 )	titleInfo.nextMode = 0;
				}

				//	�_�ōX�V
				switch ( titleInfo.nextMode )
				{
				case 0:
					FlashingUpdate( titleInfo.gameStartImage, D3DX_PI / 180 * 4.0f );
					titleInfo.creditTextImage.flashingAlpha = 1.0f;
					break;

				case 1:
					FlashingUpdate( titleInfo.creditTextImage, D3DX_PI / 180 * 4.0f );
					titleInfo.gameStartImage.flashingAlpha = 1.0f;
					break;
				}
				
				//	SPACE�L�[�őI��
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					//	�^�C�g���摜���B��
					titleInfo.titleImage.renderflag = false;

					//	���艹�炷
					sound->PlaySE( SE::DECIDE_SE );

					//	��ʐ���
					screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, 1.0f );

					//	�J�[�e�����[�h�ݒ�
					curtain->SetMode( Curtain::OPEN );

					//	pressspace�g��
					static	float	wavespeed = 1.5f;
					switch ( titleInfo.nextMode )
					{
					case 0:	SetWave( titleInfo.gameStartImage, wavespeed );
						break;
					case 1:	SetWave( titleInfo.creditTextImage, wavespeed );
						break;
					}

					//	���̃X�e�b�v
					titleInfo.step++;
				}
				break;

			case TITLE_STEP::WAIT_OPEN:
				//	�J�����X�V
				mainView->Update( VIEW_MODE::TITLE );

				//	�g��X�V
				WaveUpdate( titleInfo.gameStartImage );
				WaveUpdate( titleInfo.creditTextImage );

				//	�_�ōX�V
				switch ( titleInfo.nextMode )
				{
				case 0:
					FlashingUpdate( titleInfo.gameStartImage, flashingSpeed );
					titleInfo.creditTextImage.flashingAlpha = 1.0f;
					break;

				case 1:
					FlashingUpdate( titleInfo.creditTextImage, flashingSpeed );
					titleInfo.gameStartImage.flashingAlpha = 1.0f;
					break;
				}

				//	����ς݂Ŏ��̃X�e�b�v��
				if ( curtain->GetIsEnd() )	titleInfo.step++;
				break;

			case TITLE_STEP::NEXT_MODE:
				titleInfo.changeflag = false;
				titleInfo.curtainState = false;
				static	float	screenSpeed = 1.0f;
				screen->SetScreenMode( SCREEN_MODE::WHITE_IN, screenSpeed );

				switch ( titleInfo.nextMode )
				{
				case 0:
					mode = TITLE_MODE::MENU;
					break;

				case 1:
					mode = TITLE_MODE::CREDIT;
					break;
				}
				break;
			}
		}

		//	�`��
		void	sceneTitle::TitleRender( void )
		{
			//	�w�i�`��
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight , RS_COPY, 0xFFFFFFFF );

			//	���`��
			curtain->Render();

			//	�^�C�g���摜�`��
			RenderImage( titleInfo.titleImage, 0, 0, 512, 512, IMAGE_MODE::NORMAL );

			//	�e�L�X�g�`��
			RenderImage( titleInfo.gameStartImage, 0, 0, 512, 128, IMAGE_MODE::FLASH );
			RenderImage( titleInfo.creditTextImage, 0, 128, 512, 128, IMAGE_MODE::FLASH );
			RenderImage( titleInfo.pressSpace, 0, 0, 256, 128, IMAGE_MODE::WAVE );
			RenderImage( titleInfo.gameStartImage, 0, 0, 512, 128, IMAGE_MODE::WAVE );
			RenderImage( titleInfo.creditTextImage, 0, 128, 512, 128, IMAGE_MODE::WAVE );
		}

	//--------------------------------------------------------
	//	���j���[
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::MenuUpdate( void )
		{
			//	�ړ����������
			if ( mainView->GetMoveState() )	//	�ړ����I�����Ă���
			{
				//	����
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					//	���艹
					sound->PlaySE( SE::DECIDE_SE );

					//	�J�����ړ���A�����_�ݒ�
					if ( menuInfo.menu_num == TITLE_TARGET::PLAY )
					{
						mode = TITLE_MODE::PLAY;
						if ( mainView->GetMoveState() )
						{
							mainView->SetNextPoint( TITLE_TARGET::MOVE_MENU_UP, 0.008f );
						}
					}
				}
			}

			//	�J�����X�V
			mainView->Update( VIEW_MODE::TITLE );
		}

		//	�`��
		void	sceneTitle::MenuRender( void )
		{

		}

	//--------------------------------------------------------
	//	�I����ʂ�
	//--------------------------------------------------------
		
		//�@�X�V
		void	sceneTitle::MoveSceneUpdate( void )
		{			
			//	�J�����X�V
			mainView->Update( VIEW_MODE::TITLE );

			switch ( cameraInfo.state )
			{
			case 0:
				if ( mainView->GetMoveState() )
				{
					mainView->SetNextPoint( TITLE_TARGET::MOVE_MENU_IN, 0.01f );
					screen->SetScreenMode( SCREEN_MODE::WIPE_OUT, 1.5f );
					cameraInfo.state++;
				}
				break;

			case 1:
				if ( screen->GetScreenState() )
				{
					MainFrame->ChangeScene( new sceneMenu() );
					return;
				}
				break;
			}
		}

		//�@�`��
		void	sceneTitle::MoveSceneRender( void )
		{

		}

	//--------------------------------------------------------
	//	�N���W�b�g
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::CreditUpdate( void )
		{
			//	�^�C�g���֖߂�
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3  || input[0]->Get( KEY_B ) == 3 )
			{
				mode = TITLE_MODE::TITLE;
				MainFrame->ChangeScene( new sceneTitle() );
				return;
			}
			
			//	�X�N���[���؂�ւ��C����
			if ( screen->GetScreenState() )
			{
				creditInfo.t += 0.003f;
				if ( creditInfo.t >= 1.0f )	creditInfo.t = 1.0f;
			}

			//	���
			Lerp( creditInfo.sy, 0, 800, creditInfo.t );
		}

		//	�`��
		void	sceneTitle::CreditRender( void )
		{
			creditInfo.credit->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, creditInfo.sy, 512, 512 );
		}