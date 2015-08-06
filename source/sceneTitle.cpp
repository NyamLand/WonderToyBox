
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

#include	"sceneTitle.h"

//*****************************************************************************
//
//	sceneTitle�N���X
//
//*****************************************************************************

namespace c_Move{
	Vector3 TARGET[4]=
	{
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 20.0f, 0.0f, 10.0f ),
		Vector3( 5.0f, 0.0f, -10.0f ),
		Vector3( -20.0f, 0.0f, 10.0f ),
	};

}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneTitle::sceneTitle(void) : orientation(0, 0, 0, 1), mode(TITLE)
	{

	}
	
	//	�f�X�g���N�^
	sceneTitle::~sceneTitle( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_Camera );
		SafeDelete( m_Player );
		SafeDelete( m_CollisionStage );
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

		switch (mode)
		{
		case TITLE:
			TitleUpdate();
			break;
		
		case MENU:
			MenuUpdate();
			break;

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

		case OPTION:
			OptionUpdate();
			break;

		case CREDIT:
			CreditUpdate();
			break;

		default:
			mode = 0;
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
		m_Stage->Render(shader3D, "full");
		m_Player->Render(shader3D, "full");

		switch (mode)
		{
		case TITLE:
			TitleRender();
			break;

		case MENU:
			MenuRender();
			break;

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

		case OPTION:
			OptionRender();
			break;

		case CREDIT:
			CreditRender();
			break;

		default:
			mode = 0;
			break;
		}

		////	�f�o�b�O�p
		//DrawString( "[sceneTitle]", 50, 50 );
		//DrawString( "���؁[�������Ă�", 300, 400, 0xFFFFFF00 );
		//DrawString( "�́[���Ղ�����I", 1100, 700, 0xFFFFFF00 );
	}

//******************************************************************
//�@�e��ʂ̃��\�b�h
//******************************************************************

	//--------------------------------------------------------
	//	�^�C�g��
	//--------------------------------------------------------
	void	sceneTitle::TitleUpdate()
	{
		//�@SPACE�ŃX�^�[�g
		if (KEY(KEY_SPACE) == 3)
		{
			mode = MENU;
		}
	}

	void	sceneTitle::TitleRender()
	{
		DrawString("�^�C�g������", 50, 50);
		DrawString("[SPACE]�F���j���[��", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	���j���[
	//--------------------------------------------------------
	void	sceneTitle::MenuUpdate()
	{
		//	�ړ����ς���
		if (KEY(KEY_C) == 3 && t > 0.7f)
		{
			if (testpos < 3)	testpos++;
			else testpos = 0;
			t = 0;
		}
		if (KEY(KEY_D) == 3 && t > 0.7f)
		{
			if (testpos > 0)	testpos--;
			else testpos = 3;
			t = 0;

		}

		if (t <= 0.7f){
			Lerp(c_pos, s_pos, c_Move::TARGET[testpos], t - 0.1f);

		}
		else{
			s_pos = m_Camera->GetPos();
		}

		switch (testpos)
		{
		case 0:
			if (KEY(KEY_SPACE) == 3) mode = SELECT_PLAYERNUM;
			break;
		case 1:
			if (KEY(KEY_SPACE) == 3) mode = OPTION;
			break;
		case 2:
			if (KEY(KEY_SPACE) == 3) mode = CREDIT;
			break;
		}

		//	�J�����X�V
		m_Camera->Update(VIEW_MODE::SLERP, c_Move::TARGET[testpos]);
		m_Camera->SetPos(c_pos);

		t += 0.01f;
	}
	
	void	sceneTitle::MenuRender()
	{
		DrawString("���j���[��ʂ���", 50, 50);
		DrawString("C�EV�łǂꂩ�I��ł�", 300, 400, 0xFFFFFF00);

		switch (testpos)
		{
		case 0:
			DrawString("�݂�Ȃł����ԃ��[�h�I", 50, 100);
			DrawString("[SPACE]�F�L�����I����", 300, 200, 0xFFFFFF00);
			break;
		case 1:
			DrawString("�I�v�V����", 50, 100);
			DrawString("[SPACE]�F�I�v�V������", 300, 200, 0xFFFFFF00);
			break;
		case 2:
			DrawString("��������m�����I", 50, 100);
			DrawString("[SPACE]�F�N���W�b�g��", 300, 200, 0xFFFFFF00);
			break;
		}

	}

	//--------------------------------------------------------
	//	�l���I��
	//--------------------------------------------------------
	void	sceneTitle::SelectPlayerNumUpdate(void)
	{
		if (KEY(KEY_SPACE) == 3) mode = SELECT_CHARACTER;
	}

	void	sceneTitle::SelectPlayerNumRender(void)
	{
		DrawString("�l���I������", 50, 50);
		DrawString("[SPACE]�F�L�����I����", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	�L�����I��
	//--------------------------------------------------------
	void	sceneTitle::SelectCharacterUpdate(void)
	{
		if (KEY(KEY_SPACE) == 3) mode = SELECT_STAGE;
	}

	void	sceneTitle::SelectCharacterRender(void)
	{
		DrawString("�L�����I������", 50, 50);
		DrawString("[SPACE]�F�X�e�[�W�I����", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	�X�e�[�W�I��
	//--------------------------------------------------------
	void	sceneTitle::SelectStageUpdate(void)
	{
		if (KEY(KEY_SPACE) == 3) mode = SELECT_CHECK;
	}

	void	sceneTitle::SelectStageRender(void)
	{
		DrawString("�X�e�[�W�I������", 50, 50);
		DrawString("[SPACE]�F�ŏI�m�F��", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	�ŏI�m�F
	//--------------------------------------------------------
	void	sceneTitle::SelectCheckUpdate(void)
	{
		if (KEY(KEY_SPACE) == 3)
		{
			MainFrame->ChangeScene(new sceneMain());
			return;
		}
	}

	void	sceneTitle::SelectCheckRender(void)
	{
		DrawString("�ŏI�m�F����", 50, 50);
		DrawString("[SPACE]�FsceneMain��", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	�I�v�V����
	//--------------------------------------------------------
	void	sceneTitle::OptionUpdate()
	{
		if (KEY(KEY_SPACE) == 3) mode = MENU;
	}
	
	void	sceneTitle::OptionRender()
	{
		DrawString("�I�v�V��������B�ݒ肢�����Ă�", 50, 50);
		DrawString("[SPACE]�F���j���[��", 300, 400, 0xFFFFFF00);
	}

	//--------------------------------------------------------
	//	�N���W�b�g
	//--------------------------------------------------------
	void	sceneTitle::CreditUpdate()
	{
		if (KEY(KEY_SPACE) == 3) mode = MENU;
	}

	void	sceneTitle::CreditRender()
	{
		DrawString("��������m�����̏Љ��", 50, 50);
		DrawString("[SPACE]�F���j���[��", 300, 400, 0xFFFFFF00);
	}