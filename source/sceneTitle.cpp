
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"sceneMain.h"
#include	"sceneSelect.h"

#include	"sceneTitle.h"

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

namespace c_Move{
	Vector3 TARGET[MAX_TARGET]=
	{
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 20.0f, 0.0f, 10.0f ),
		Vector3( 5.0f, 0.0f, -10.0f ),
		Vector3( -20.0f, 0.0f, 10.0f ),
		Vector3( 0.0f, 50.0f, 200.0f ),
	};

}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneTitle::sceneTitle( void ) : orientation( 0, 0, 0, 1 )
	{

	}
	
	//	�f�X�g���N�^
	sceneTitle::~sceneTitle( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_Camera );
		SafeDelete( m_Player );
		SafeDelete( m_CollisionStage );
		SafeDelete( screen );
	}
	
	//	������
	bool	sceneTitle::Initialize( void )
	{
		//view = new iexView();

		//	���ݒ�
		iexLight::SetAmbient(0x404040);
		iexLight::SetFog(800, 1000, 0);

		//	���C�g�ݒ�
		Vector3 dir(1.0f, -1.0f, -0.5f);
		dir.Normalize();
		iexLight::DirLight(shader3D, 0, &dir, 0.8f, 0.8f, 0.8f);

		//	�J�����ݒ�
		m_Camera = new Camera();

		//	�X�e�[�W
		m_CollisionStage = new iexMesh("DATA/BG/CollisionGround.IMO");
		m_Stage = new iexMesh( "DATA/BG/2_1/FIELD2_1.IMO" );

		//	�����蔻��
		Collision::Initiallize( m_CollisionStage );

		//	�v���C���[
		m_Player = new PlayerManager();
		m_Player->Initialize( 0, PlayerData::Y2009, c_Move::TARGET[0] );
		m_Player->Initialize( 1, PlayerData::Y2009, c_Move::TARGET[1] );
		m_Player->Initialize( 2, PlayerData::Y2009, c_Move::TARGET[2] );
		m_Player->Initialize( 3, PlayerData::Y2009, c_Move::TARGET[3] );
		testpos = 0;
		t = 0;
		s_pos = m_Camera->GetPos();
		c_pos = m_Camera->GetPos();
		t_pos = Vector3( 0, 0, 0 );

		//	�V�[���؂�ւ��p�X�e�[�g
		state_step = 0;
		scene_change = MAX_TARGET - 1;
		//	�w�i�E�X�N���[���摜������
		screen = new iex2DObj("DATA/Screen.png");

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
		//	�v���C���[�X�V
		m_Player->Update();


		//	�ړ����ς���
		if ( KEY( KEY_C ) == 3  && t > 0.7f)
		{
			if ( testpos < 3 )	testpos++;
			else testpos = 0;
			t = 0;
		}
		if ( KEY( KEY_D ) == 3	&& t > 0.7f )
		{
			if ( testpos > 0 )	testpos--;
			else testpos = 3;
			t = 0;

		}		
		
		if ( t <= 0.7f ){
			Lerp( c_pos, s_pos, c_Move::TARGET[testpos], t - 0.1f );
	
		}
		else{
			s_pos = m_Camera->GetPos();
		}

		//	�J�����X�V
		m_Camera->Update( VIEW_MODE::SLERP, c_Move::TARGET[testpos] );
		m_Camera->SetPos( c_pos );

		//	�X�y�[�X�������ăV�[����؂�ւ��鏈��
		SceneState();

		//	
		if (t < 1.0f){
			t += 0.01f;
		}
	}

	//	�V�[���X�e�[�g
	void	sceneTitle::SceneState( void )
	{
		switch (state_step)
		{
		case 0:
			if (KEY(KEY_SPACE) == 3)
			{
				state_step++;
				//MainFrame->ChangeScene(new sceneSelect());
				//return;
			}
			break;
		case 1:
			testpos = scene_change;
			t = 0;
			state_step++;
			break;

		case 2:
			if (t >= 1.0f)
			{
				MainFrame->ChangeScene(new sceneSelect());
				return;
			}
			break;

		}
	}

	//	�`��
	void	sceneTitle::Render( void )
	{
		//	��ʃN���A
		m_Camera->Activate();
		m_Camera->Clear();

		//	�I�u�W�F�N�g�`��
		m_Stage->Render( shader3D, "full" );
		m_Player->Render( shader3D, "full" );

		//	�f�o�b�O�p
		DrawString( "[sceneTitle]", 50, 50 );
		DrawString( "���؁[�������Ă�", 300, 400, 0xFFFFFF00 );
		DrawString( "�́[���Ղ�����I", 1100, 700, 0xFFFFFF00 );

		if (state_step >= 2){
			shader2D->SetValue("alpha", t);
			screen->Render(0, 0, 1280, 720, 0, 0, 512, 512, shader2D, "copy");
		}
	}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

	//	