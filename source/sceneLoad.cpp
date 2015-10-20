
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
	//�f�o�b�N�p
	int	Xs = 400;
	int Ys = 200;
//----------------------------------------------------------------------------------
//	�������E���	
//----------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	sceneLoad::sceneLoad( Scene*	nextScene ) : newScene( nextScene )
	{
		//	�t���O������
		threadState = false;
		loadflg = false;
	}

	//	�f�X�g���N�^
	sceneLoad::~sceneLoad( void )
	{
		SafeDelete( load );
		SafeDelete(load_anykey.obj);
		
	}

	//	������
	bool	sceneLoad::Initialize( void )
	{
		//    ���ݒ�
		iexLight::SetAmbient( 0x808080 );
		iexLight::SetFog( 800, 1000, 0 );

		//	�J�����ݒ�
		mainView = new Camera();
		load = new iex2DObj("DATA/Load/Lord-back.png");
		load_anykey.obj = new iex2DObj("DATA/UI/pressspace.png");
		ImageInitialize(load_anykey, 1100, 675, 256, 128, 0, 0, 256, 128);
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

		//	�_�ōX�V
		FlashingUpdate(load_anykey, D3DX_PI / 180 * 4.0f);

			//	�V�[���؂芷��
			if ( threadState ){
				if (KEY_Get(KEY_SPACE) == 3 || KEY(KEY_A) == 3){

					//	pressspace�g��
					static	float	wavespeed = 1.5f;
					SetWave(load_anykey, wavespeed);
					MainFrame->ChangeScene(newScene, false);
					return;
				}
			}

			//�f�o�b�O�p
			if (KEY_Get(KEY_RIGHT) == 1){
				Xs += 10;
			}
			if (KEY_Get(KEY_LEFT) == 1){
				Xs -= 10;
			}
			if (KEY_Get(KEY_DOWN) == 1){
				Ys += 10;

			}

			if (KEY_Get(KEY_UP) == 1){
				Ys -= 10;

			}
		}

	//	�`��
	void	sceneLoad::Render( void )
	{
		//	��ʃN���A
		mainView->Activate();
		mainView->Clear();
		load->Render(0, 0, 1280, 720, 0, 0, 1280, 720);
		DrawString( "���[�h��", 200, 300 );

		//	pressSpace�`��
		RenderImage(load_anykey, 0, 0, 256, 128, IMAGE_MODE::FLASH);
		RenderImage(load_anykey, 0, 0, 256, 128, IMAGE_MODE::WAVE);



		//�f�o�b�N�p
		sprintf_s(stri, "%d\n%d", Xs, Ys);
		DrawString(stri, 0, 250, 0xFFFFFFFF);
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