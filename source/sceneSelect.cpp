
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
	sceneSelect::sceneSelect( void )
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

//--------------------------------------------------------------------------------
//	�X�V�E�`��
//--------------------------------------------------------------------------------

	//	�X�V
	void	sceneSelect::Update( void )
	{
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneMain() );
			return;
		}
	}

	//	�`��
	void	sceneSelect::Render( void )
	{
		view->Activate();
		view->Clear();
		
		//	�f�o�b�O�����`��
		DrawString( "[sceneSelect]", 50, 50 );
		DrawString( "���؁[���Ń��C����",300, 400, 0xFFFFFF00 );
	}
