
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
	
	//	static�錾
	bool	sceneLoad::threadState;

//----------------------------------------------------------------------------------
//	�������E���	
//----------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	sceneLoad::sceneLoad( Scene*	nextScene ) : newScene( nextScene )
	{
		//	�t���O������
		threadState = false;
	}

	//	�f�X�g���N�^
	sceneLoad::~sceneLoad( void )
	{

	}

	//	������
	bool	sceneLoad::Initialize( void )
	{
		//    ���ݒ�
		iexLight::SetAmbient( 0x808080 );
		iexLight::SetFog( 800, 1000, 0 );

		//	�J�����ݒ�
		m_Camera = new Camera();

		//	�ʃX���b�h�쐬
		_beginthread( Thread, 0, ( void* )newScene );
		
		return	true;
	}

//----------------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------------
	
	//	�X�V
	void	sceneLoad::Update( void )
		{
			//	�V�[���؂芷��
			if ( threadState ){
				MainFrame->ChangeScene( newScene, false );
				return;
			}
		}

	//	�`��
	void	sceneLoad::Render( void )
	{
		//	��ʃN���A
		m_Camera->Activate();
		m_Camera->Clear( 0xFF55FFFF );
	}

//----------------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------------
	
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