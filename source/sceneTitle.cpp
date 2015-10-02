
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"Image.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Random.h"
#include	"PlayerManager.h"
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
	//	�J�����ړ��p�ʒu���
	namespace c_Move{
		Vector3 TARGET[4]=
		{
			Vector3( 0.0f, 0.0f, 0.0f ),
			Vector3( 20.0f, 0.0f, 10.0f ),
			Vector3( 5.0f, 0.0f, -10.0f ),
			Vector3( -20.0f, 0.0f, 10.0f ),
		};

	}

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

		//	�}�l�[�W���[������
		gameManager->Initialize();

		//	�p�[�e�B�N��������
		particle->Initialize();

		//	���o�^
		sound->Initialize();

		//	����������
		Random::Initialize();
		
		//	�J�����ݒ�
		m_Camera = new Camera();

		//	�\���̏�����
		{
			//	�L�����N�^�[��񏉊���
			for ( int i = 0; i < PLAYER_TYPE::MAX; i++ )
			{
				characterInfo[i].name = characterName[i];
				characterInfo[i].select = false;
			}
			
			//	�I����񏉊���
			selectInfo.playerNum = 1;
			selectInfo.stageType = 0;
			for ( int i = 0; i < PLAYER_NUM; i++ )	selectInfo.characterType[i] = i;

			//	�J�������\���̏�����
			cameraInfo.lerpStartPos = cameraInfo.pos = m_Camera->GetPos();
			cameraInfo.target = Vector3( 0.0f, 0.0f, 0.0f );
			cameraInfo.t = 0.0f;
			cameraInfo.posNum = 0;
		}

		return	true;
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
			SelectPlayerNumUpdate();
			break;

		case TITLE_MODE::SELECT_CHARACTER:
			SelectCharacterUpdate();
			break;

		case TITLE_MODE::SELECT_STAGE:
			SelectStageUpdate();
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
		for ( int i = 0; i < PLAYER_NUM; i++)	particle->BlueFlame( c_Move::TARGET[i] );
		particle->Update();
		
	}

	//	�`��
	void	sceneTitle::Render( void )
	{
		//	��ʃN���A
		m_Camera->Activate();
		m_Camera->Clear();

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
			SelectPlayerNumRender();
			break;

		case TITLE_MODE::SELECT_CHARACTER:
			SelectCharacterRender();
			break;

		case TITLE_MODE::SELECT_STAGE:
			SelectStageRender();
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
			
		}

		//	�`��
		void	sceneTitle::TitleRender( void )
		{
			DrawString( "�^�C�g������", 50, 50 );
			DrawString( "[SPACE]�F���j���[��", 300, 400, 0xFFFFFF00 );
		}

	//--------------------------------------------------------
	//	���j���[
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::MenuUpdate( void )
		{
			//	�ړ����ς���
			if ( KEY( KEY_C ) == 3 && cameraInfo.t > 0.7f )
			{
				if ( cameraInfo.posNum < 3 )	cameraInfo.posNum++;
				else cameraInfo.posNum = 0;
				cameraInfo.t = 0.0f;
			}
			if ( KEY( KEY_D ) == 3 && cameraInfo.t > 0.7f )
			{
				if ( cameraInfo.posNum > 0 )	cameraInfo.posNum--;
				else cameraInfo.posNum = 3;
				cameraInfo.t = 0;
			}

			//	�J�����ړ�
			if ( cameraInfo.t <= 0.7f ){
				CubicFunctionInterpolation( cameraInfo.pos, cameraInfo.lerpStartPos, c_Move::TARGET[cameraInfo.posNum], cameraInfo.t - 0.1f );
			}
			else{
				cameraInfo.lerpStartPos = m_Camera->GetPos();
			}

			//	����
			if ( KEY( KEY_SPACE ) == 3 )
			{
				switch ( cameraInfo.posNum )
				{
				case 0:		mode = TITLE_MODE::SELECT_PLAYERNUM;	break;
				case 1:		mode = TITLE_MODE::OPTION;						break;
				case 2:		mode = TITLE_MODE::CREDIT;						break;
				}	
				sound->PlaySE( SE::DECIDE_SE );
			}

			//	�J�����X�V
			m_Camera->Update( VIEW_MODE::SLERP, c_Move::TARGET[cameraInfo.posNum] );
			m_Camera->SetPos( cameraInfo.pos );

			//	�p�����[�^���Z
			cameraInfo.t += 0.01f;
			if ( cameraInfo.t >= 1.0f )	cameraInfo.t = 1.0f;
		}

		//	�`��
		void	sceneTitle::MenuRender( void )
		{
			DrawString( "���j���[��ʂ���", 50, 50 );
			DrawString( "C�EV�łǂꂩ�I��ł�", 300, 400, 0xFFFFFF00 );

			switch ( cameraInfo.posNum )
			{
			case 0:
				DrawString( "�݂�Ȃł����ԃ��[�h�I", 50, 100 );
				DrawString( "[SPACE]�F�L�����I����", 300, 200, 0xFFFFFF00 );
				break;

			case 1:
				DrawString( "�I�v�V����", 50, 100 );
				DrawString( "[SPACE]�F�I�v�V������", 300, 200, 0xFFFFFF00 );
				break;

			case 2:
				DrawString( "��������m�����I", 50, 100 );
				DrawString( "[SPACE]�F�N���W�b�g��", 300, 200, 0xFFFFFF00 );
				break;
			}

		}

	//--------------------------------------------------------
	//	�l���I��
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::SelectPlayerNumUpdate( void )
		{
			//�@�uA�v�uD�v�őI��
			//�@�uSPACE�v�Ō���
			//�@�uS�v�Ŗ߂�

			if ( KEY( KEY_RIGHT ) == 3 )		selectInfo.playerNum++;
			if ( KEY( KEY_LEFT ) == 3 )		selectInfo.playerNum--;

			if ( selectInfo.playerNum > PLAYER_NUM )	selectInfo.playerNum = 1;
			if ( selectInfo.playerNum < 1 )						selectInfo.playerNum = PLAYER_NUM; 

			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::SELECT_CHARACTER;
				sound->PlaySE( SE::DECIDE_SE );
			}
			if ( KEY( KEY_DOWN ) == 3 )		mode = TITLE_MODE::MENU;
		}
		
		//	�`��
		void	sceneTitle::SelectPlayerNumRender( void )
		{
			DrawString( "�l���I������", 50, 50 );
			DrawString( "[SPACE]�F�L�����I����", 300, 400, 0xFFFFFF00 );

			char	str[64];
			wsprintf( str, "�v���C���[�l���F%d\n", selectInfo.playerNum );
			IEX_DrawText( str, 300, 300, 200, 20, 0xFFFF00FF );
		}

	//--------------------------------------------------------
	//	�L�����I��
	//--------------------------------------------------------
		
		//	�X�V
		void	sceneTitle::SelectCharacterUpdate( void )
		{
			//�@�v���C���[�L�����I���@���@CPU�L�����I��
			//�@�uA�v�uD�v�őI��
			//�@�uSPACE�v�Ō���
			//�@�uS�v�Ŗ߂�

			static bool	select[4] = { false, false, false, false };
			static int step = 0;
			int selectCheck = 0;

			switch ( step )
			{
			case 0:		//�@�v���C���[�L�����I��
				for ( int p = 0; p < selectInfo.playerNum; p++ )
				{
					if ( input[p]->Get( KEY_DOWN ) == 3 )
					{
						//	�S�����̓��̓`�F�b�N
						selectCheck = 0;
						for ( int i = 0; i < PLAYER_NUM; i++ )
						{
							if ( select[i] )		selectCheck++;
						}

						//	�S�����I���Ȃ�߂�
						if ( selectCheck == 0 )
						{
							mode = TITLE_MODE::SELECT_PLAYERNUM;
							return;
						}

						//	�I�����L�����Z��
						characterInfo[selectInfo.characterType[p]].select = false;
						select[p] = false;
					};

					if ( select[p] )	continue;
					if ( input[p]->Get( KEY_RIGHT ) == 3 )	selectInfo.characterType[p]++;
					if ( input[p]->Get( KEY_LEFT ) == 3 )	selectInfo.characterType[p]--;
					if ( input[p]->Get( KEY_SPACE ) == 3 )
					{
						//	���I���Ȃ�I��
						if ( !characterInfo[selectInfo.characterType[p]].select )
						{
							characterInfo[selectInfo.characterType[p]].select = true;
							select[p] = true;
							sound->PlaySE( SE::DECIDE_SE );
						}

						//	�S�����̓��̓`�F�b�N
						selectCheck = 0;
						for ( int i = 0; i < PLAYER_NUM; i++ )
						{
							if (select[i])		selectCheck++;
						}

						//	�S���I���ς݂������玟�̃X�e�b�v��
						if ( selectCheck >= selectInfo.playerNum )
						{
							step++;
							sound->PlaySE( SE::DECIDE_SE );
						}
					}

					//	���l����
					if ( selectInfo.characterType[p] >= PLAYER_TYPE::MAX )	selectInfo.characterType[p] = 0;
					if ( selectInfo.characterType[p] < 0 )	selectInfo.characterType[p] = PLAYER_TYPE::MAX - 1;
				}
				break;

			case 1:		//�@CPU�L�����I��
				if ( selectInfo.playerNum == PLAYER_NUM )	step++;
				else
				{
					for ( int p = selectInfo.playerNum; p < PLAYER_NUM; p++ )
					{
						if ( KEY( KEY_RIGHT ) == 3 )	selectInfo.characterType[p]++;
						if ( KEY( KEY_LEFT ) == 3 )	selectInfo.characterType[p]--;

						//	���l����
						if ( selectInfo.characterType[p] >= PLAYER_TYPE::MAX )	selectInfo.characterType[p] = 0;
						if ( selectInfo.characterType[p] < 0 )	selectInfo.characterType[p] = PLAYER_TYPE::MAX - 1;
					}
					if ( KEY( KEY_SPACE ) == 3 )
					{
						step++;
						sound->PlaySE( SE::DECIDE_SE );
					}
					if ( KEY( KEY_DOWN ) == 3 ) step--;
				}
				break;

			case 2:
				mode = TITLE_MODE::SELECT_STAGE;
				step = 0;
				break;
			}
		}
		
		//	�`��
		void	sceneTitle::SelectCharacterRender( void )
		{
			DrawString( "�L�����I������", 50, 50 );
			DrawString( "[SPACE]�F�X�e�[�W�I����", 300, 400, 0xFFFFFF00 );

			char	str[64];
			for ( int p = 0; p < PLAYER_NUM; p++ )
			{
				LPSTR string = characterInfo[selectInfo.characterType[p]].name;
				sprintf_s( str, "\n%d�o�̃L�����^�C�v�F", p + 1 );
				strcat( str, string );
				DrawString( str, 300, 300 + p * 20 );
			}
		}

	//--------------------------------------------------------
	//	�X�e�[�W�I��
	//--------------------------------------------------------
		
		//	�X�V
		void	sceneTitle::SelectStageUpdate( void )
		{
			//�@�uA�v�uD�v�őI��
			//�@�uSPACE�v�Ō���
			//�@�uS�v�Ŗ߂�

			//	�I��
			if ( KEY( KEY_RIGHT ) == 3 ) 	selectInfo.stageType++;
			if ( KEY( KEY_LEFT ) == 3 )		selectInfo.stageType--;

			//	�㉺���ݒ�
			if ( selectInfo.stageType >= STAGE_MAX )	selectInfo.stageType = 0;
			if ( selectInfo.stageType < 0 )					selectInfo.stageType = STAGE_MAX - 1;

			//	����E�L�����Z��
			if ( KEY( KEY_SPACE ) == 3 )	
			{
				mode = TITLE_MODE::SELECT_CHECK;
				sound->PlaySE( SE::DECIDE_SE );
			}
			if ( KEY( KEY_DOWN ) == 3 )		mode = TITLE_MODE::SELECT_CHARACTER;
		}

		//	�`��
		void	sceneTitle::SelectStageRender( void )
		{
			DrawString( "�X�e�[�W�I������", 50, 50 );
			DrawString( "[SPACE]�F�ŏI�m�F��", 300, 400, 0xFFFFFF00 );

			char	str[64];
			wsprintf( str, "�X�e�[�W�F%d��\n", selectInfo.stageType );
			IEX_DrawText( str, 300, 300, 200, 20, 0xFFFF00FF );
		}

	//--------------------------------------------------------
	//	�ŏI�m�F
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::SelectCheckUpdate( void )
		{
			//		�uA�v�uD�v�ŁA�͂��E�������I�Ȃ��I��
			//�@	�uSPACE�v�Ō���
			//�@	�uS�v�Ŗ߂�
			if ( KEY( KEY_RIGHT ) == 3 )		selectInfo.ready = !selectInfo.ready;
			if ( KEY( KEY_LEFT ) == 3 )		selectInfo.ready = !selectInfo.ready;
			if ( KEY( KEY_DOWN ) == 3 )		mode = TITLE_MODE::SELECT_STAGE;
			if ( KEY( KEY_SPACE ) == 3 )
			{
				if ( selectInfo.ready )
				{
					//	�����}�l�[�W���[�ɓo�^
					for ( int p = 0; p < PLAYER_NUM; p++ )		gameManager->SetCharacterType( p, selectInfo.characterType[p] );
					gameManager->SetPlayerNum( selectInfo.playerNum );
					gameManager->SetStageType( selectInfo.stageType );
					sound->PlaySE( SE::DECIDE_SE );
					mode = TITLE_MODE::MOVE_MAIN;
				}
				else	mode = TITLE_MODE::SELECT_STAGE;
			}
		}
		
		//	�`��
		void	sceneTitle::SelectCheckRender( void )
		{
			//�@�I�����
			char	str[64];
			for ( int p = 0; p < PLAYER_NUM; p++ )
			{
				LPSTR string = characterInfo[selectInfo.characterType[p]].name;
				wsprintf( str, "\n%d�o�̃L�����^�C�v�F", p + 1 );
				strcat( str, string );
				DrawString( str, 300, 250 + p * 20, 0xFFFF00FF );
			}

			wsprintf( str, "�v���C���[�l���F%d\n", selectInfo.playerNum );
			DrawString( str, 300, 350, 0xFFFF00FF );

			wsprintf( str, "�X�e�[�W�F%d��\n", selectInfo.stageType );
			DrawString( str, 300, 370, 0xFFFF00FF );

			DrawString( "�ŏI�m�F����", 50, 50 );
			DrawString( "[SPACE]�FsceneMain��", 300, 470, 0xFFFFFF00 );

			static bool ready = true;
			if ( KEY( KEY_RIGHT ) == 3 ) ready = !ready;
			if ( KEY( KEY_LEFT ) == 3 ) ready = !ready;
			wsprintf( str, "�͂���1�A��������0�F%d��\n", ready );
			IEX_DrawText( str, 300, 390, 200, 20, 0xFFFF00FF );
		}

	//--------------------------------------------------------
	//	�I�v�V����
	//--------------------------------------------------------
	
		//	�X�V
		void	sceneTitle::OptionUpdate( void )
		{
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
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
			if ( KEY( KEY_SPACE ) == 3 )
			{
				mode = TITLE_MODE::MENU;
				sound->PlaySE( SE::DECIDE_SE );
			}
		}

		//	�`��
		void	sceneTitle::CreditRender( void )
		{
			DrawString( "��������m�����̏Љ��", 50, 50 );
			DrawString( "[SPACE]�F���j���[��", 300, 400, 0xFFFFFF00 );
		}