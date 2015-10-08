
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Random.h"
#include	"BaseChara.h"
#include	"CharacterManager.h"
#include	"sceneMain.h"
#include	"GameManager.h"
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

	//	�^�C�g�����[�h
	namespace TITLE_MODE
	{
		enum
		{
			TITLE,
			MENU,
			SELECT_PLAYERNUM,
			SELECT_CHARACTER,
			SELECT_STAGE,
			SELECT_CHECK,
			OPTION,
			CREDIT,
			MOVE_MAIN,
		};
	}

	//	���j���[���[�h
	namespace SELECT_MODE
	{
		enum
		{
			PLAYER_NUM,
			CHARACTER,
			STAGE,
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
		SafeDelete( m_Camera );
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
		m_Camera = new Camera();

		//	�}�l�[�W���[������
		gameManager->Initialize();

		//	�p�[�e�B�N��������
		particle->Initialize();

		//	���o�^
		sound->Initialize();

		//	�X�N���[��������
		screen->Initialize();
		
		//	�X�e�[�W
		stage = new iexMesh( "DATA/BG/title_map.IMO" );

		//	����������
		Random::Initialize();		

		//	�e���[�h������
		TitleInitialize();
		MenuInitialize();
		SelectInitialize();

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

	//	�I��������
	void	sceneTitle::SelectInitialize( void )
	{
		//	�L�����N�^�[��񏉊���
		for ( int i = 0; i < CHARACTER_TYPE::MAX; i++ )
		{
			characterInfo[i].name = characterName[i];
			characterInfo[i].select = false;
		}

		//	�I����񏉊���
		setInfo.playerNum = 1;
		setInfo.stageType = 0;
		setInfo.step_cs = 0;
		for ( int i = 0; i < PLAYER_MAX; i++ )	setInfo.characterType[i] = i;

		//	�p�����[�^������
		selectInfo.menu_num = 0;
		selectInfo.mode = SELECT_MODE::PLAYER_NUM;
		selectInfo.select_mode = 0;

		//	�v���C���[�I����񏉊���
		for ( int i = 0; i < PLAYER_MAX; i++ )  
		{
			selectInfo.playerselectInfo[i].isSelect = false;
			selectInfo.playerselectInfo[i].num = i;
		}
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

		case TITLE_MODE::SELECT_PLAYERNUM:
			SelectUpdate();
			break;

		case TITLE_MODE::SELECT_CHECK:
			SelectCheckUpdate();
			break;

		case TITLE_MODE::OPTION:
			OptionUpdate();
			break;

		case TITLE_MODE::CREDIT:
			CreditUpdate();
			break;

		case TITLE_MODE::MOVE_MAIN:
			if ( !sound->GetSEState( SE::DECIDE_SE ) )
			{
				MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
				return;
			}
			break;
		}

		//	�^�[�Q�b�g�ʒu�Ƀp�[�e�B�N���z�u
		for ( int i = 0; i < TITLE_TARGET::END; i++)	particle->BlueFlame( TITLE_MOVE_INFO::target[i] );
		particle->Update();
		
		//	�X�N���[���X�V
		screen->Update();	
		particle->BlueFlame( eff_pos, 1.0f );
	}

	//	�`��
	void	sceneTitle::Render( void )
	{
		//	��ʃN���A
		m_Camera->Activate();
		m_Camera->Clear();

		//	�X�e�[�W�`��
		stage->Render();

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

		case TITLE_MODE::SELECT_PLAYERNUM:
			SelectRender();
			break;

		case TITLE_MODE::SELECT_CHECK:
			SelectCheckRender();
			break;

		case TITLE_MODE::OPTION:
			OptionRender();
			break;

		case TITLE_MODE::CREDIT:
			CreditRender();
			break;

		case TITLE_MODE::MOVE_MAIN:
			break;
		}

		//	�X�N���[���`��
		screen->Render();

		//	�f�o�b�O
		m_Camera->Render();

		//char	str[256];
		Matrix	mat = m_Camera->GetMatrix();
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
			static	float	speed = 0.5f;
			static	bool	curtainStateL = false;
			static	bool	curtainStateR = false;

			switch ( titleInfo.step )
			{
			case 0:
				//	�J�����X�V
				m_Camera->Update( VIEW_MODE::SETUP );

				//	�_�ōX�V
				FlashingUpdate( titleInfo.pressSpace, D3DX_PI / 180 * 4.0f );

				//	SPACE�L�[�őI��
				if ( input[0]->Get( KEY_SPACE ) == 3 )
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
				m_Camera->Update( VIEW_MODE::TITLE );

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
				curtainStateL = Lerp( titleInfo.curtainL.tlv[0].sx, 0, -640, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[1].sx, 640, 0, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[2].sx, 0, -640, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateL = Lerp( titleInfo.curtainL.tlv[3].sx, 640, 0, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[0].sx, 640, 1280, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[1].sx, 1280, 1920, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.curtainL.t ) );
				curtainStateR = Lerp( titleInfo.curtainR.tlv[2].sx, 640, 1280, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, titleInfo.curtainL.t ) );
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
			if ( m_Camera->GetMoveState() )	//	�ړ����I�����Ă���
			{
				//	C,V�L�[�őI��
				if ( KEY( KEY_C ) == 3 )
				{
					menuInfo.menu_num--;
					if ( menuInfo.menu_num < 0 )		menuInfo.menu_num = TITLE_TARGET::CREDIT;
					m_Camera->SetNextPoint( menuInfo.menu_num, 0.01f );
				}
				if ( KEY( KEY_D ) == 3 )
				{
					menuInfo.menu_num++;
					if ( menuInfo.menu_num > TITLE_TARGET::CREDIT )	menuInfo.menu_num = 0;
					m_Camera->SetNextPoint( menuInfo.menu_num, 0.01f );
				}

				//	����
				if ( KEY( KEY_SPACE ) == 3 )
				{
					switch ( menuInfo.menu_num )
					{
					case TITLE_TARGET::PLAYERNUMBER:
						mode = TITLE_MODE::SELECT_PLAYERNUM;
						break;

					case TITLE_TARGET::OPTION:
						mode = TITLE_MODE::OPTION;
						break;

					case TITLE_TARGET::CREDIT:
						mode = TITLE_MODE::CREDIT;
						break;
					}	
					sound->PlaySE( SE::DECIDE_SE );
					m_Camera->SetNextPoint( menuInfo.menu_num, 0.01f );
				}
			}

			//	�J�����X�V
			m_Camera->Update( VIEW_MODE::TITLE );
		}

		//	�`��
		void	sceneTitle::MenuRender( void )
		{
			DrawString( "���j���[��ʂ���", 50, 50 );
			DrawString( "C�EV�łǂꂩ�I��ł�", 300, 400, 0xFFFFFF00 );

			switch ( menuInfo.menu_num )
			{
			case TITLE_TARGET::PLAYERNUMBER:
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

		}

	//--------------------------------------------------------
	//	�I��
	//--------------------------------------------------------

		//	�X�V
		void	sceneTitle::SelectUpdate( void )
		{
			switch ( selectInfo.mode )
			{
			case SELECT_MODE::PLAYER_NUM:
				SelectPlayerNumUpdate();
				break;

			case SELECT_MODE::CHARACTER:
				SelectCharacterUpdate();
				break;

			case SELECT_MODE::STAGE:
				SelectStageUpdate();
				break;
			}

			//	�J�����X�V
			m_Camera->Update( VIEW_MODE::TITLE );
		}

		//	�`��
		void	sceneTitle::SelectRender( void )
		{
			switch ( selectInfo.mode )
			{
			case SELECT_MODE::PLAYER_NUM:
				SelectPlayerNumRender();
				break;

			case SELECT_MODE::CHARACTER:
				SelectCharacterRender();
				break;

			case SELECT_MODE::STAGE:
				SelectStageRender();
				break;
			}
		}

	//--------------------------------------------------------
	//	�l���I��
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::SelectPlayerNumUpdate( void )
		{
			//	�ړ����͑I��s��
			if( !m_Camera->GetMoveState() )	return;

			//	�I��
			if ( KEY( KEY_RIGHT ) == 3 )		setInfo.playerNum++;
			if ( KEY( KEY_LEFT ) == 3 )		setInfo.playerNum--;

			//	����
			if ( setInfo.playerNum > PLAYER_MAX )		setInfo.playerNum = 1;
			if ( setInfo.playerNum < 1 )						setInfo.playerNum = PLAYER_MAX; 

			//	�I��
			if ( KEY( KEY_SPACE ) == 3 )
			{
				selectInfo.mode = SELECT_MODE::CHARACTER;
				m_Camera->SetNextPoint( TITLE_TARGET::SELECTCHARACTER, 0.01f );
				sound->PlaySE( SE::DECIDE_SE );
				return;
			}

			//	�L�����Z��
			if ( KEY( KEY_DOWN ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				m_Camera->SetNextPoint( TITLE_TARGET::PLAYERNUMBER, 0.01f );
			}
		}
		
		//    �`��
		void    sceneTitle::SelectPlayerNumRender( void )
		{
			DrawString("�l���I������", 50, 50);
			DrawString("[SPACE]�F�L�����I����", 300, 400);

			char    str[64];
			wsprintf(str, "�v���C���[�l���F%d\n", setInfo.playerNum);
			IEX_DrawText(str, 300, 300, 200, 20, 0xFFFFFFFF);
		}

	//--------------------------------------------------------
	//	�L�����I��
	//--------------------------------------------------------
		
		//	�X�V
		void	sceneTitle::SelectCharacterUpdate( void )
		{
			if ( !m_Camera->GetMoveState() )	return;
			switch ( selectInfo.select_mode )
			{
			case 0:
				SelectPlayer();
				break;

			case 1:
				SelectCPU();
				break;

			case 2:
				selectInfo.mode = SELECT_MODE::STAGE;
				selectInfo.select_mode = 0;
				m_Camera->SetNextPoint( TITLE_TARGET::SELECTSTAGE, 0.01f );
				break;
			}
		}
		
		//    �`��
		void	sceneTitle::SelectCharacterRender( void )
		{
			DrawString("�L�����I������", 300, 100);
			//DrawString( "[SPACE]�F�X�e�[�W�I����", 300, 400, 0xFFFFFF00 );

			char    str[64];
			for (int p = 0; p < setInfo.playerNum; p++)
			{
				LPSTR string = characterInfo[selectInfo.playerselectInfo[p].num].name;
				sprintf_s(str, "\n%d�o�̃L�����^�C�v�F", p + 1);
				strcat(str, string);
				DrawString(str, 300, 300 + p * 20, 0xFFFFFF00);
			}
			for (int p = setInfo.playerNum; p < PLAYER_MAX; p++)
			{
				LPSTR string = characterInfo[selectInfo.playerselectInfo[p].num].name;
				sprintf_s(str, "\n%d�o�̃L�����^�C�v�F", p + 1);
				strcat(str, string);
				DrawString(str, 300, 300 + p * 20);
			}

			switch (selectInfo.select_mode)
			{
			case 0:
				DrawString("�v���C���[���g���L������I��ł�", 300, 280, 0xFFFFFF00);
				break;

			case 1:
				DrawString("�b�o�t���g���L������I��ł�", 300, 280);
				break;
			}

		}

		//	�v���C���[�I��
		void	sceneTitle::SelectPlayer( void )
		{
			for ( int p = 0; p < setInfo.playerNum; p++ )
			{
				if ( input[p]->Get( KEY_DOWN ) == 3 )
				{
					//	�S�����I���Ȃ�߂�
					bool	isSelect[PLAYER_MAX] = 
					{
						selectInfo.playerselectInfo[0].isSelect,
						selectInfo.playerselectInfo[1].isSelect,
						selectInfo.playerselectInfo[2].isSelect,
						selectInfo.playerselectInfo[3].isSelect,
					};

					//	�S�����I���Ȃ�O�̃��j���[��
					if ( !isSelect[0] && !isSelect[1] && !isSelect[2] && !isSelect[3] )
					{
						selectInfo.mode = TITLE_TARGET::PLAYERNUMBER;
						return;
					}

					//	�I�����L�����Z��
					characterInfo[selectInfo.playerselectInfo[p].num].select = false;
					selectInfo.playerselectInfo[p].isSelect= false;
				};

				//	�I���ς݂̓X���[
				if ( selectInfo.playerselectInfo[p].isSelect )	continue;

				//	���E�őI��
				if ( input[p]->Get( KEY_RIGHT ) == 3 )	selectInfo.playerselectInfo[p].num++;
				if ( input[p]->Get( KEY_LEFT ) == 3 )	selectInfo.playerselectInfo[p].num--;
				
				//	���l����
				if ( selectInfo.playerselectInfo[p].num >= CHARACTER_TYPE::MAX )	selectInfo.playerselectInfo[p].num = 0;
				if ( selectInfo.playerselectInfo[p].num < 0 )	selectInfo.playerselectInfo[p].num = CHARACTER_TYPE::MAX - 1;

				if ( input[p]->Get( KEY_SPACE ) == 3 )
				{
					//	���I���Ȃ�I��
					if ( !characterInfo[selectInfo.playerselectInfo[p].num].select )
					{
						characterInfo[selectInfo.playerselectInfo[p].num].select = true;
						selectInfo.playerselectInfo[p].isSelect = true;
						sound->PlaySE( SE::DECIDE_SE );
					}

					//	�S�����I���Ȃ�߂�
					bool	isSelect[PLAYER_MAX] =
					{
						selectInfo.playerselectInfo[0].isSelect,
						selectInfo.playerselectInfo[1].isSelect,
						selectInfo.playerselectInfo[2].isSelect,
						selectInfo.playerselectInfo[3].isSelect,
					};

					//	�S���̑I���`�F�b�N
					bool	state = false;
					switch ( setInfo.playerNum )
					{
					case 1:
						if ( isSelect[0] )	state = true;
						break;

					case 2:
						if ( isSelect[0] && isSelect[1] )	state = true;
						break;

					case 3:
						if ( isSelect[0] && isSelect[1] && isSelect[2] )	state = true;
						break;

					case 4:
						if ( isSelect[0] && isSelect[1] && isSelect[2] && isSelect[3] )	state = true;
						break;
					}
					//	�S���I���ς݂������玟�̃X�e�b�v��
					if ( state )
					{
						selectInfo.select_mode++;
						sound->PlaySE( SE::DECIDE_SE );
						for ( int i = 0; i < PLAYER_MAX; i++ )
						{
							setInfo.characterType[i] = selectInfo.playerselectInfo[i].num;
						}
					}
				}

			}
		}

		//	CPU�I��
		void	sceneTitle::SelectCPU( void )
		{
			//	�v���C���[���S�l�����������
			if ( setInfo.playerNum == PLAYER_MAX )	selectInfo.select_mode++;
			else
			{
				for ( int p = setInfo.playerNum; p < PLAYER_MAX; p++ )
				{
					//	�I��
					if (KEY(KEY_RIGHT) == 3)	setInfo.characterType[p]++;
					if (KEY(KEY_LEFT) == 3)	setInfo.characterType[p]--;

					//	���l����
					if ( setInfo.characterType[p] >= CHARACTER_TYPE::MAX )	setInfo.characterType[p] = 0;
					if ( setInfo.characterType[p] < 0 )	setInfo.characterType[p] = CHARACTER_TYPE::MAX - 1;
				}
				
				//	����
				if ( KEY( KEY_SPACE ) == 3 )
				{
					selectInfo.select_mode++;
					sound->PlaySE( SE::DECIDE_SE );
				}
				
				//	�L�����Z��
				if ( KEY( KEY_DOWN ) == 3 ) selectInfo.select_mode--;
			}
		}

	//--------------------------------------------------------
	//	�X�e�[�W�I��
	//--------------------------------------------------------
		
		//	�X�V
		void	sceneTitle::SelectStageUpdate( void )
		{
			//	�ړ����͑I��s��
			if ( !m_Camera->GetMoveState() )	return;

			//	�I��
			if ( KEY( KEY_RIGHT ) == 3 ) 	setInfo.stageType++;
			if ( KEY( KEY_LEFT ) == 3 )		setInfo.stageType--;

			//	�㉺���ݒ�
			if ( setInfo.stageType >= STAGE_MAX )	setInfo.stageType = 0;
			if ( setInfo.stageType < 0 )						setInfo.stageType = STAGE_MAX - 1;

			//	����
			if ( KEY( KEY_SPACE ) == 3 )	
			{
				mode = TITLE_MODE::SELECT_CHECK;
				sound->PlaySE( SE::DECIDE_SE );
				m_Camera->SetNextPoint( TITLE_TARGET::CHECK, 0.01f );
				return;
			}

			//	�L�����Z��
			if ( KEY( KEY_DOWN ) == 3 )
			{
				selectInfo.mode = TITLE_TARGET::SELECTCHARACTER;
				m_Camera->SetNextPoint( selectInfo.mode, 0.01f );
			}
		}

		//    �`��
		void    sceneTitle::SelectStageRender( void )
		{
			DrawString("�X�e�[�W�I������", 50, 50);
			DrawString("[SPACE]�F�ŏI�m�F��", 300, 400);

			char    str[64];
			wsprintf(str, "�X�e�[�W�F%d��\n", setInfo.stageType);
			IEX_DrawText(str, 300, 300, 200, 20, 0xFFFFFFFF);
		}

	//--------------------------------------------------------
	//	�ŏI�m�F
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::SelectCheckUpdate( void )
		{
			m_Camera->Update( VIEW_MODE::TITLE );
			if ( !m_Camera->GetMoveState() )	return;
			if ( KEY( KEY_RIGHT ) == 3 )		setInfo.ready = !setInfo.ready;
			if ( KEY( KEY_LEFT ) == 3 )		setInfo.ready = !setInfo.ready;
			if ( KEY( KEY_DOWN ) == 3 )		mode = TITLE_MODE::SELECT_PLAYERNUM;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				if ( setInfo.ready )
				{
					//	�����}�l�[�W���[�ɓo�^
					for ( int p = 0; p < PLAYER_MAX; p++ )		gameManager->SetCharacterType( p, setInfo.characterType[p] );
					gameManager->SetPlayerNum( setInfo.playerNum );
					gameManager->SetStageType( setInfo.stageType );
					sound->PlaySE( SE::DECIDE_SE );
					mode = TITLE_MODE::MOVE_MAIN;
				}
				else	mode = TITLE_MODE::SELECT_PLAYERNUM;
			}
		}
		
		//    �`��
		void    sceneTitle::SelectCheckRender( void )
		{
			//�@�I�����
			char    str[64];
			for (int p = 0; p < PLAYER_MAX; p++)
			{
				LPSTR string = characterInfo[setInfo.characterType[p]].name;
				wsprintf(str, "\n%d�o�̃L�����^�C�v�F", p + 1);
				strcat(str, string);
				DrawString(str, 300, 250 + p * 20);
			}

			wsprintf(str, "�v���C���[�l���F%d\n", setInfo.playerNum);
			DrawString(str, 300, 350);

			wsprintf(str, "�X�e�[�W�F%d��\n", setInfo.stageType);
			DrawString(str, 300, 370);

			DrawString("�ŏI�m�F����", 50, 50);
			DrawString("[SPACE]�FsceneMain��", 300, 470);

			static bool ready = true;
			if (KEY(KEY_RIGHT) == 3) ready = !ready;
			if (KEY(KEY_LEFT) == 3) ready = !ready;
			wsprintf(str, "�͂���1�A��������0�F%d��\n", ready);
			IEX_DrawText(str, 300, 390, 200, 20, 0xFFFFFFFF);
		}

	//--------------------------------------------------------
	//	�I�v�V����
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::OptionUpdate( void )
		{
			m_Camera->Update( VIEW_MODE::TITLE );
			if ( !m_Camera->GetMoveState() )	return;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
				m_Camera->SetNextPoint( TITLE_TARGET::PLAYERNUMBER, 0.01f );
			}
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
			m_Camera->Update( VIEW_MODE::TITLE );
			if ( !m_Camera->GetMoveState() )	return;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
				m_Camera->SetNextPoint( TITLE_TARGET::PLAYERNUMBER, 0.01f );
			}
		}

		//	�`��
		void	sceneTitle::CreditRender( void )
		{
			DrawString( "��������m�����̏Љ��", 50, 50 );
			DrawString( "[SPACE]�F���j���[��", 300, 400, 0xFFFFFF00 );
		}