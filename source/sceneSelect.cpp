
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system\Scene.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"sceneMain.h"
#include	"sceneTitle.h"

#include	"sceneSelect.h"

//******************************************************************************
//
//	sceneSelect�N���X
//
//******************************************************************************

//--------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneSelect::sceneSelect( void ) : mode(0)
	{
	
	}

	//	�f�X�g���N�^
	sceneSelect::~sceneSelect( void )
	{
		SafeDelete( view );
	}

	//	������
	bool	sceneSelect::Initialize( void )
	{
		//	�J�����ݒ�
		view = new iexView();
		return	true;
	}

//--------------------------------------------------------
//	�S�̍X�V
//--------------------------------------------------------
	void	sceneSelect::Update( void )
	{
		switch ( mode )
		{
		case SELECT_PLAYERNUM:
			SelectPlayerNumUpdate();
			break;

		case SELECT_CHARACTER:
			SelectCharacterUpdate();
			break;
		
		case SELECT_STAGE:
			SelectStageUpdate();
			break;
		
		case SELECT_CHECK:
			SelectCheckUpdate();
			break;
		
		default:
			mode = 0;
			break;
		}
	}

//--------------------------------------------------------
//	�S�̕`��
//--------------------------------------------------------
	void	sceneSelect::Render( void )
	{
		view->Activate();
		view->Clear();
		
		switch ( mode )
		{
		case SELECT_PLAYERNUM:
			SelectPlayerNumRender();
			break;

		case SELECT_CHARACTER:
			SelectCharacterRender();
			break;

		case SELECT_STAGE:
			SelectStageRender();
			break;

		case SELECT_CHECK:
			SelectCheckRender();
			break;

		default:
			mode = 0;
			break;
		}

		//	�f�o�b�O�����`��
		//DrawString( "[sceneSelect]", 50, 50 );
		//DrawString( "���؁[���Ń��C����",300, 400, 0xFFFFFF00 );
	}

//******************************************************************
//�@�e��ʂ̃��\�b�h
//******************************************************************

	//--------------------------------------------------------
	//	�l���I��
	//--------------------------------------------------------
	void	sceneSelect::SelectPlayerNumUpdate( void )
	{
		if (KEY(KEY_SPACE) == 3) mode = SELECT_CHARACTER;
	}

	void	sceneSelect::SelectPlayerNumRender( void )
	{
		DrawString("�l���I������", 50, 50);
		DrawString("[SPACE]�F�L�����I����", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	�L�����I��
	//--------------------------------------------------------
	void	sceneSelect::SelectCharacterUpdate( void )
	{
		if (KEY(KEY_SPACE) == 3) mode = SELECT_STAGE;
	}

	void	sceneSelect::SelectCharacterRender( void )
	{
		DrawString("�L�����I������", 50, 50);
		DrawString("[SPACE]�F�X�e�[�W�I����", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	�X�e�[�W�I��
	//--------------------------------------------------------
	void	sceneSelect::SelectStageUpdate( void )
	{
		if ( KEY( KEY_SPACE ) == 3 ) mode = SELECT_CHECK;
	}

	void	sceneSelect::SelectStageRender( void )
	{
		DrawString( "�X�e�[�W�I������", 50, 50 );
		DrawString( "[SPACE]�F�ŏI�m�F��", 300, 400, 0xFFFFFF00 );
	}

	//--------------------------------------------------------
	//	�ŏI�m�F
	//--------------------------------------------------------
	void	sceneSelect::SelectCheckUpdate( void )
	{
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneMain() );
			return;
		}
	}

	void	sceneSelect::SelectCheckRender( void )
	{
		DrawString( "�ŏI�m�F����", 50, 50 );
		DrawString( "[SPACE]�FsceneMain��", 300, 400, 0xFFFFFF00 );
	}
