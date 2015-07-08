
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"system/Scene.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"sceneMain.h"
#include	"sceneSelect.h"

#include	"sceneTitle.h"

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneTitle::sceneTitle( void )
	{
	
	}
	
	//	�f�X�g���N�^
	sceneTitle::~sceneTitle( void )
	{
		SafeDelete( view );
	}
	
	//	������
	bool	sceneTitle::Initialize( void )
	{
		view = new iexView();
		return	true;
	}

	//	�T�E���h������
	void	sceneTitle::InitSound( void )
	{

	}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

	//	�X�V
	void	sceneTitle::Update( void )
	{
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneSelect() );
			return;
		}
	}

	//	�`��
	void	sceneTitle::Render( void )
	{
		//	��ʃN���A
		view->Activate();
		view->Clear();

		//	�f�o�b�O�p
		DrawString( "[sceneTitle]", 50, 50 );
		DrawString( "���؁[�������Ă�", 300, 400, 0xFFFFFF00 );
		DrawString( "�́[���Ղ�����I", 1100, 700, 0xFFFFFF00 );
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	