
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"Collision.h"
#include	"system/System.h"
#include	"Image.h"
#include	"GameManager.h"
#include	"Player.h"
#include	"Camera.h"
#include	"sceneTitle.h"
#include	"PlayerManager.h"
#include	"sceneMain.h"

#include	"sceneResult.h"

//********************************************************************************
//
//	sceneResult�N���X
//
//********************************************************************************

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneResult::sceneResult( void )
	{
	
	}

	//	�f�X�g���N�^
	sceneResult::~sceneResult( void )
	{
		SafeDelete(m_Camera);
		SafeDelete(m_Player);
		SafeDelete( collision );
		SafeDelete( back );
		Random::Release();
	}

	//	������
	bool	sceneResult::Initialize( void )
	{
		//	���ݒ�
		iexLight::SetAmbient(0x404040);
		iexLight::SetFog(800, 1000, 0);

		//	���C�g�ݒ�
		Vector3 dir(1.0f, -1.0f, -0.5f);
		dir.Normalize();
		iexLight::DirLight(shader3D, 0, &dir, 0.8f, 0.8f, 0.8f);

		//	�J�����ݒ�
		m_Camera = new Camera();

		//	�w�i������
		back = new iex2DObj( "DATA/Result/back.png" );

		//	�R���W����
		collision = new iexMesh( "DATA/BG/CollisionGround.IMO" );
		Collision::Initiallize( collision );

		//	�v���C���[������
		m_Player = new PlayerManager();
		for ( int i = 0; i < 4; i++ )
		{
			coinNum[i] = 0;
			resultInfo[i].p_Coin = gameManager->GetCoinNum( i );
			resultInfo[i].p_num = i;

			int		characterType = gameManager->GetCharacterType( i );
			Vector3	pos = Vector3( 0.0f, 0.0f, 0.0f );
			m_Player->Initialize( i, characterType, pos );
		}

		//	�ϐ�������
		lastBonus = 0;
		step = 0;
		playerNum = 0;
		wait = 0;
		
		//	����������
		Random::Initialize();

		//	�\�[�g
		BubbleSort();

		//	���X�g�{�[�i�X�ݒ�
		SetLastBonus();

		return	true;
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	sceneResult::Update( void ) 
	{
		m_Player->Update();
		m_Camera->Update( VIEW_MODE::SLERP, m_Player->GetPos( 0 ) );
		if ( KEY( KEY_SPACE ) == 3 )
		{
			MainFrame->ChangeScene( new sceneTitle() );
			return;
		}
	}

	//	�`��
	void	sceneResult::Render( void ) 
	{
		m_Camera->Activate();
		m_Camera->Clear();

		//iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		//back->Render(0, 0, 1280, 720, 0, 0, 2048, 1024);
		//iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		m_Player->Render( shader3D, "ShadowBuf" );

		//	�f�o�b�O�����`��
		DrawString( "[sceneResult]", 50, 50 );
		DrawString( "���؁[���Ń^�C�g����", 300, 400, 0xFFFFFF00 );
		DrawString( GameInfo::LastBonus[lastBonus], 300, 150 );

		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "%d�� : player%d�@%d��", i + 1, resultInfo[i].p_num + 1, resultInfo[i].p_Coin );
			DrawString( str, 550, 250 + i * 30, 0xFFFFFFFF );
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	�o�u���\�[�g
	void	sceneResult::BubbleSort( void )
	{
		RESULT_INFO		temp;

		for ( int i = 0; i < 4; ++i )
		{
			//	��납�珇�ԂɃ`�F�b�N���Ă���
			for ( int s =3; s > i; --s )
			{
				//	����̗v�f�Ɣ�r
				if ( resultInfo[s].p_Coin >	resultInfo[s - 1].p_Coin )
				{
					//	�ꎞ�I�ɑޔ�
					temp = resultInfo[s - 1];

					//	����
					resultInfo[s - 1] = resultInfo[s];

					//	�ޔ����Ă����߂�
					resultInfo[s] = temp;
				}
			}
		}
	}

	//	���X�g�{�[�i�X�ݒ�
	void	sceneResult::SetLastBonus( void )
	{
		if ( Random::PercentageRandom( 0.3f ) )
		{
			//	���X�g�{�[�i�X��ݒ�
			lastBonus = gameManager->GetLastBonus();
		}
		else
		{
			//	�Ⴄ���ʂ��o��܂Ń��[�v
			while ( lastBonus == gameManager->GetLastBonus() )
			{
				lastBonus = rand() % 5;
			}
		}
	}