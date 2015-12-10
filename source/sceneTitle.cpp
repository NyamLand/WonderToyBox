
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
	//	�^�C�g�����j���[�p���
	namespace Title_Info
	{
		const		LPSTR	characterName[] =
		{
			"�i�C�g",
			"�v�����Z�X",
			"���X",
			"Y�˂�����"
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
		SafeDelete( mainView );
		SafeDelete( titleInfo.curtainL.obj );
		SafeDelete( titleInfo.curtainR.obj );
		SafeDelete( titleInfo.titleImage.obj );
		SafeDelete( titleInfo.pressSpace.obj );
		SafeDelete( stage );
		SafeDelete( titleInfo.titleText );
		SafeDelete( creditInfo.credit );
		sound->AllStop();
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

		//	�J�����ݒ�
		mainView = new Camera();

		//	�}�l�[�W���[������
		gameManager->Initialize();

		//	�p�[�e�B�N��������
		particle->Initialize();

		//	���o�^
		sound->Initialize();

		//	�X�N���[��������
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::FADE_IN, 1.0f );

		//	UI������
		ui = new UI();
		ui->Initialize( UI_MODE::TITLE );
		
		//	�X�e�[�W
		stage = new iexMesh( "DATA/BG/titlestage/title_map.IMO" );

		//	����������
		Random::Initialize();		

		//	�e���[�h������
		TitleInitialize();
		MenuInitialize();
		CreditInitialize();

		return	true;
	}

	//	�^�C�g��������
	void	sceneTitle::TitleInitialize( void )
	{
		//	�摜�ǂݍ���
		titleInfo.curtainL.obj = new iex2DObj( "DATA/UI/title/curtain1.png" );
		titleInfo.curtainR.obj = new iex2DObj( "DATA/UI/title/curtain2.png" );
		titleInfo.titleImage.obj = new iex2DObj( "DATA/UI/title.png" );
		titleInfo.pressSpace.obj = new iex2DObj( "DATA/UI/pressspace.png" );
		titleInfo.titleText = new iex2DObj( "DATA/UI/title/titleText.png" );

		//	�摜�\���̏�����
		ImageInitialize( titleInfo.pressSpace, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.82f ), static_cast<int>( iexSystem::ScreenWidth * 0.2f ), static_cast<int>( iexSystem::ScreenHeight * 0.15f ), 0, 0, 256, 128 );
		ImageInitialize( titleInfo.titleImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.42f ), static_cast<int>( iexSystem::ScreenWidth * 0.4f ), static_cast<int>( iexSystem::ScreenHeight * 0.7f ), 0, 0, 512, 512 );
		ImageInitialize( titleInfo.gameStartImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.80f ), static_cast<int>( iexSystem::ScreenWidth * 0.15f ), static_cast<int>( iexSystem::ScreenHeight * 0.07f ), 0, 0, 512, 128 );
		ImageInitialize( titleInfo.creditTextImage, iexSystem::ScreenWidth / 2, static_cast<int>( iexSystem::ScreenHeight * 0.87f ), static_cast<int>( iexSystem::ScreenWidth * 0.1f ), static_cast<int>( iexSystem::ScreenHeight * 0.07f ), 0, 128, 512, 128 );
		titleInfo.gameStartImage.obj = titleInfo.titleText;
		titleInfo.creditTextImage.obj = titleInfo.titleText;

		//	�J�[�e���p�\���̏�����
		{
			//	�p�����[�^������
			titleInfo.curtainL.t = 0.0f;
			titleInfo.curtainR.t = 0.0f;

			//	���_�ݒ�
			SetVertex( titleInfo.curtainL.tlv[0], 0, 0, 0, 0, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[1], static_cast<float>( iexSystem::ScreenWidth / 2 ), 0, 0, 1, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[2], 0, static_cast<float>( iexSystem::ScreenHeight ), 0, 0, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[3], static_cast<float>( iexSystem::ScreenWidth / 2 ), static_cast<float>( iexSystem::ScreenHeight ), 0, 1, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[0], static_cast<float>( iexSystem::ScreenWidth / 2 ), 0, 0, 0, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[1], static_cast<float>( iexSystem::ScreenWidth ), 0, 0, 1, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[2], static_cast<float>( iexSystem::ScreenWidth / 2 ), static_cast<float>( iexSystem::ScreenHeight ), 0, 0, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[3], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight ), 0, 1, 1, 0xFFFFFFFF );
		}

		//	�ϐ�������
		titleInfo.titleImage.renderflag = true;
		titleInfo.step = 0;
		titleInfo.nextMode = 0;
	}

	//	���j���[������
	void	sceneTitle::MenuInitialize( void )
	{
		menuInfo.menu_num = 0;
	}

	//	�N���W�b�g������
	void	sceneTitle::CreditInitialize( void )
	{
		creditInfo.credit = new iex2DObj( "DATA/UI/title/credit.png" );
		creditInfo.sy = 0;
		creditInfo.t = 0.0f;
	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�X�V
	void	sceneTitle::Update( void )
	{
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
			MoveSelectUpdate();
			break;

		case TITLE_MODE::OPTION:
			OptionUpdate();
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
		mainView->Clear();

		//	�X�e�[�W�`��
		stage->Render( shader3D, "full" );

		//	�p�[�e�B�N���`��
		particle->Render();

		switch ( mode )
		{
		case TITLE_MODE::TITLE:
			TitleRender();
			break;

		case TITLE_MODE::MENU:
			MenuRender();
			break;

		case TITLE_MODE::PLAY:
			MoveSelectRender();
			break;

		case TITLE_MODE::OPTION:
			OptionRender();
			break;

		case TITLE_MODE::CREDIT:
			CreditRender();
			break;
		}

		//	UI�`��
		ui->Render( mode );

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
			//	�p�����[�^
			static	bool	changeflag = false;
			static	bool	curtainStateL = false;
			static	bool	curtainStateR = false;
			static	float	speed = 0.5f;

			switch ( titleInfo.step )
			{
			case 0:
				//	�t�F�[�h�����I���܂ő҂�
				if ( screen->GetScreenState() )	titleInfo.step++;
				break;

			case 1:
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
				if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
				{
					//	�^�C�g���摜���B��
					titleInfo.titleImage.renderflag = false;

					//	��ʐ���
					screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, 1.0f );

					//	pressspace�g��
					static	float	wavespeed = 1.5f;
					
					switch ( titleInfo.nextMode )
					{
					case 0:
						SetWave( titleInfo.gameStartImage, wavespeed );
						break;

					case 1:
						SetWave( titleInfo.creditTextImage, wavespeed );
						break;
					}
					titleInfo.step++;
				}
				break;

			case 2:
				//	�J�����X�V
				mainView->Update( VIEW_MODE::TITLE );

				//	�g��X�V
				//WaveUpdate( titleInfo.pressSpace );
				WaveUpdate( titleInfo.gameStartImage );
				WaveUpdate( titleInfo.creditTextImage );

				//	�_�ōX�V
				switch (titleInfo.nextMode)
				{
				case 0:
					FlashingUpdate( titleInfo.gameStartImage, D3DX_PI / 180 * 10.0f );
					titleInfo.creditTextImage.flashingAlpha = 1.0f;
					break;

				case 1:
					FlashingUpdate( titleInfo.creditTextImage, D3DX_PI / 180 * 10.0f );
					titleInfo.gameStartImage.flashingAlpha = 1.0f;
					break;
				}
				//	�p�����[�^���Z
				titleInfo.curtainL.t += D3DX_PI / 180 * speed;
				titleInfo.curtainR.t += D3DX_PI / 180 * speed;

				//	�p�����[�^����ݒ�
				if ( titleInfo.curtainL.t >= 1.0f )	titleInfo.curtainL.t = 1.0f;
				if ( titleInfo.curtainR.t >= 1.0f )	titleInfo.curtainR.t = 1.0f;

				//	�e���_�ړ�
				curtainStateL = Lerp( titleInfo.curtainL.tlv[0].sx, 0, -( static_cast<int>( iexSystem::ScreenWidth / 2 ) ), GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[1].sx, static_cast<int>( iexSystem::ScreenWidth / 2 ), 0, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[2].sx, 0, -( static_cast<int>( iexSystem::ScreenWidth / 2 ) ), GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[3].sx, static_cast<int>( iexSystem::ScreenWidth / 2 ), 0, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[0].sx, static_cast<int>( iexSystem::ScreenWidth / 2 ), static_cast<int>( iexSystem::ScreenWidth ), GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[1].sx, static_cast<int>( iexSystem::ScreenWidth ), static_cast<int>( iexSystem::ScreenWidth * 1.5f ), GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[2].sx, static_cast<int>( iexSystem::ScreenWidth / 2 ), static_cast<int>( iexSystem::ScreenWidth ), GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[3].sx, static_cast<int>( iexSystem::ScreenWidth ), static_cast<int>( iexSystem::ScreenWidth * 1.5f ), GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );

				//	����ς݂Ŏ��̃X�e�b�v��
				if ( curtainStateL && curtainStateR )	titleInfo.step++;
				break;

			case 3:
				changeflag = false;
				curtainStateL = false;
				curtainStateR = false;
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
			iexPolygon::Rect(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight , RS_COPY, 0xFFFFFFFF);

			//	���`��
			iexPolygon::Render2D( titleInfo.curtainL.tlv, 2, titleInfo.curtainL.obj, RS_COPY );
			iexPolygon::Render2D( titleInfo.curtainR.tlv, 2, titleInfo.curtainL.obj, RS_COPY );

			//	�^�C�g���摜�`��
			RenderImage( titleInfo.titleImage, 0, 0, 512, 512, IMAGE_MODE::NORMAL );

			//	pressSpace�`��
			//RenderImage( titleInfo.pressSpace, 0, 0, 256, 128, IMAGE_MODE::FLASH );
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
				//	C,V�L�[�őI��
				if ( KEY( KEY_LEFT ) == 3 )
				{
					menuInfo.menu_num--;
					if ( menuInfo.menu_num < 0 )		menuInfo.menu_num = TITLE_TARGET::CREDIT;
					mainView->SetNextPoint( menuInfo.menu_num, 0.01f );
				}
				if ( KEY( KEY_RIGHT ) == 3 )
				{
					menuInfo.menu_num++;
					if ( menuInfo.menu_num > TITLE_TARGET::CREDIT )	menuInfo.menu_num = 0;
					mainView->SetNextPoint( menuInfo.menu_num, 0.01f );
				}

				//	����
				if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
				{
					switch ( menuInfo.menu_num )
					{
					case TITLE_TARGET::PLAY:
						mode = TITLE_MODE::PLAY;
						if (mainView->GetMoveState())
						{
							mainView->SetNextPoint(TITLE_TARGET::MOVE_MAIN, 0.005f);
							//sound->PlaySE(SE::DECIDE_SE);
							screen->SetScreenMode( SCREEN_MODE::WIPE_OUT, 1.5f );
						}
						break;

					case TITLE_TARGET::OPTION:
						mode = TITLE_MODE::OPTION;
						break;

					case TITLE_TARGET::CREDIT:
						mode = TITLE_MODE::CREDIT;
						break;
					}	
					sound->PlaySE( SE::DECIDE_SE );
				}
			}

			//	�J�����X�V
			mainView->Update( VIEW_MODE::TITLE );

			//	UI�X�V
			switch ( menuInfo.menu_num )
			{
			case TITLE_TARGET::PLAY:
				ui->Update(TITLE_MODE::MENU);
				break;

			case TITLE_TARGET::OPTION:
				ui->Update(TITLE_MODE::OPTION);
				break;

			case TITLE_TARGET::CREDIT:
				ui->Update(TITLE_MODE::CREDIT);
				break;
			}
		}

		//	�`��
		void	sceneTitle::MenuRender( void )
		{
			DrawString( "���j���[��ʂ���", 50, 50 );
			DrawString( "C�EV�łǂꂩ�I��ł�", 300, 400, 0xFFFFFF00 );

			switch ( menuInfo.menu_num )
			{
			case TITLE_TARGET::PLAY:
				DrawString( "�݂�Ȃł����ԃ��[�h�I", 50, 100 );
				DrawString( "[SPACE]�F�L�����I����", 300, 200, 0xFFFFFF00 );
				break;

			case TITLE_TARGET::OPTION:
				DrawString( "�I�v�V����", 50, 100 );
				DrawString( "[SPACE]�F�I�v�V������", 300, 200, 0xFFFFFF00 );
				break;

			case TITLE_TARGET::CREDIT:
				DrawString( "��������m�����I", 50, 100 );
				DrawString( "[SPACE]�F�N���W�b�g��", 300, 200, 0xFFFFFF00 );
				break;
			}
			ui->Render( 0 );
		}

	//--------------------------------------------------------
	//	�I����ʂ�
	//--------------------------------------------------------
		
		//�@�X�V
		void	sceneTitle::MoveSelectUpdate( void )
		{			
			//	�J�����X�V
			mainView->Update(VIEW_MODE::TITLE);

			//	UI�X�V
			ui->Update( TITLE_MODE::MOVE_MAIN );

			if ( screen->GetScreenState() )
			{
				MainFrame->ChangeScene(new sceneMenu());
				return;
			}
		}

		//�@�`��
		void	sceneTitle::MoveSelectRender( void )
		{

		}

	//--------------------------------------------------------
	//	�I�v�V����
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::OptionUpdate( void )
		{
			mainView->Update( VIEW_MODE::TITLE );
			ui->Update( mode );
			if ( !mainView->GetMoveState() )	return;
			if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
				mainView->SetNextPoint( TITLE_TARGET::PLAY, 0.01f );
			}
			mainView->SetNextPoint(menuInfo.menu_num, 0.01f);
		}

		//	�`��
		void	sceneTitle::OptionRender( void )
		{
			DrawString( "�I�v�V��������B�ݒ肢�����Ă�", 50, 50 );
			DrawString( "[SPACE]�F���j���[��", 300, 400, 0xFFFFFF00 );
		}

	//--------------------------------------------------------
	//	�N���W�b�g
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::CreditUpdate( void )
		{
			if ( input[0]->Get( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::TITLE;
				creditInfo.t = 0.0f;
			}
			
			if (screen->GetScreenState())
			{
				creditInfo.t += 0.005f;
				if ( creditInfo.t >= 1.0f )	creditInfo.t = 1.0f;
			}

			Lerp( creditInfo.sy, 0, 1536, creditInfo.t );
		}

		//	�`��
		void	sceneTitle::CreditRender( void )
		{
			creditInfo.credit->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, creditInfo.sy, 512, 512 );
		}