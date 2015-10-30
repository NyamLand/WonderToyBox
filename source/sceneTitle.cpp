
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
		sound->AllStop();
	}
	
	//	������
	bool	sceneTitle::Initialize( void )
	{
		//	���ݒ�
		iexLight::SetAmbient( 0x404040 );
		iexLight::SetFog( 800, 1000, 0 );

		//	���C�g�ݒ�
		Vector3 dir( 1.0f, -1.0f, -0.5f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 0.8f, 0.8f, 0.8f );

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

		//	UI������
		ui = new UI();
		ui->Initialize( UI_MODE::TITLE );
		
		//	�X�e�[�W
		stage = new iexMesh( "DATA/BG/title_map.IMO" );

		//	����������
		Random::Initialize();		

		//	�e���[�h������
		TitleInitialize();
		MenuInitialize();

		return	true;
	}

	//	�^�C�g��������
	void	sceneTitle::TitleInitialize( void )
	{
		//	�摜�ǂݍ���
		titleInfo.curtainL.obj = new iex2DObj( "DATA/curtain1.png" );
		titleInfo.curtainR.obj = new iex2DObj( "DATA/curtain2.png" );
		titleInfo.titleImage.obj = new iex2DObj( "DATA/UI/title.png" );
		titleInfo.pressSpace.obj = new iex2DObj( "DATA/UI/pressspace.png" );

		//	�摜�\���̏�����
		ImageInitialize( titleInfo.pressSpace, 640, 560, 300, 100, 0, 0, 256, 128 );
		ImageInitialize( titleInfo.titleImage, 640, 300, 500, 500, 0, 0, 512, 512 );

		//	�J�[�e���p�\���̏�����
		{
			//	�p�����[�^������
			titleInfo.curtainL.t = 0.0f;
			titleInfo.curtainR.t = 0.0f;

			//	���_�ݒ�
			SetVertex( titleInfo.curtainL.tlv[0], 0, 0, 0, 0, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[1], 640, 0, 0, 1, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[2], 0, 720, 0, 0, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainL.tlv[3], 640, 720, 0, 1, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[0], 640, 0, 0, 0, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[1], 1280, 0, 0, 1, 0, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[2], 640, 720, 0, 0, 1, 0xFFFFFFFF );
			SetVertex( titleInfo.curtainR.tlv[3], 1280, 720, 0, 1, 1, 0xFFFFFFFF );
		}

		//	�ϐ�������
		titleInfo.titleImage.renderflag = true;
		titleInfo.step = 0;
	}

	//	���j���[������
	void	sceneTitle::MenuInitialize( void )
	{
		menuInfo.menu_num = 0;
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
		stage->Render();

		//	�p�[�e�B�N���`��
		particle->Render();

		switch (mode)
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

		//	�f�o�b�O
		mainView->Render();



		//char	str[256];
		Matrix	mat = mainView->GetMatrix();
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();
		//sprintf_s( str, "t.x = %f\nt.y = %f\nt.z = %f\n", eff_pos.x, eff_pos.y, eff_pos.z );
		//DrawString( str, 50, 500 );
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
				//	�J�����X�V
				mainView->Update( VIEW_MODE::SETUP );

				//	�_�ōX�V
				FlashingUpdate( titleInfo.pressSpace, D3DX_PI / 180 * 4.0f );

				//	SPACE�L�[�őI��
				if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
				{
					//	�^�C�g���摜���B��
					titleInfo.titleImage.renderflag = false;

					//	��ʐ���
					screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, speed );

					//	pressspace�g��
					static	float	wavespeed = 1.5f;
					SetWave( titleInfo.pressSpace, wavespeed );
					titleInfo.step++;
				}
				break;

			case 1:
				//	�J�����X�V
				mainView->Update( VIEW_MODE::TITLE );

				//	�g��X�V
				WaveUpdate( titleInfo.pressSpace );

				//	�_�ōX�V
				FlashingUpdate( titleInfo.pressSpace, D3DX_PI / 180.0f * 10.0f );

				//	�p�����[�^���Z
				titleInfo.curtainL.t += D3DX_PI / 180 * speed;
				titleInfo.curtainR.t += D3DX_PI / 180 * speed;

				//	�p�����[�^����ݒ�
				if ( titleInfo.curtainL.t >= 1.0f )	titleInfo.curtainL.t = 1.0f;
				if ( titleInfo.curtainR.t >= 1.0f )	titleInfo.curtainR.t = 1.0f;

				//	�e���_�ړ�
				curtainStateL = Lerp( titleInfo.curtainL.tlv[0].sx,    0, -640, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[1].sx,  640,    0, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[2].sx,    0, -640, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[3].sx,  640,    0, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[0].sx,  640, 1280, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[1].sx, 1280, 1920, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[2].sx,  640, 1280, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[3].sx, 1280, 1920, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );

				//	����ς݂Ŏ��̃X�e�b�v��
				if ( curtainStateL && curtainStateR )	titleInfo.step++;
				break;

			case 2:
				changeflag = false;
				curtainStateL = false;
				curtainStateR = false;
				static	float	screenSpeed = 1.0f;
				screen->SetScreenMode( SCREEN_MODE::WHITE_IN, screenSpeed );
				mode = TITLE_MODE::MENU;
				break;
			}
		}

		//	�`��
		void	sceneTitle::TitleRender( void )
		{
			//	�w�i�`��
			iexPolygon::Rect( 0, 0, 1280, 720, RS_COPY, 0xFFFFFFFF );

			//	���`��
			iexPolygon::Render2D( titleInfo.curtainL.tlv, 2, titleInfo.curtainL.obj, RS_COPY );
			iexPolygon::Render2D( titleInfo.curtainR.tlv, 2, titleInfo.curtainL.obj, RS_COPY );

			//	�^�C�g���摜�`��
			RenderImage( titleInfo.titleImage, 0, 0, 512, 512, IMAGE_MODE::NORMAL );

			//	pressSpace�`��
			RenderImage( titleInfo.pressSpace, 0, 0, 256, 128, IMAGE_MODE::FLASH );
			RenderImage( titleInfo.pressSpace, 0, 0, 256, 128, IMAGE_MODE::WAVE );
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
							screen->SetScreenMode( SCREEN_MODE::WIPE_OUT, 1.0f );
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

			if (screen->GetScreenState())
			{
				MainFrame->ChangeScene(new sceneLoad(new sceneMenu()));
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
			ui->Update( mode );
			mainView->Update( VIEW_MODE::TITLE );
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
		void	sceneTitle::CreditRender( void )
		{
			DrawString( "��������m�����̏Љ��", 50, 50 );
			DrawString( "[SPACE]�F���j���[��", 300, 400, 0xFFFFFF00 );
		}