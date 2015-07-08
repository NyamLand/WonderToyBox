
#include	"iextreme.h"
#include	<random>
#include	"system/system.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"Block.h"

#include	"sceneMain.h"

//*****************************************************************************************************************************
//
//	�O���[�o���ϐ�
//
//*****************************************************************************************************************************


//*****************************************************************************************************************************
//
//	������
//
//*****************************************************************************************************************************

	//	�R���X�g���N�^
	sceneMain::sceneMain( void ) : m_Stage( NULL )
	{
		
	}

	//	������
	bool	sceneMain::Initialize( void )
	{
		//	���ݒ�
		iexLight::SetAmbient( 0x404040 );
		iexLight::SetFog( 800, 1000, 0 );

		//	���C�g�ݒ�
		Vector3 dir( 1.0f, -1.0f, -0.5f );
		dir.Normalize();
		iexLight::DirLight( shader, 0, &dir, 0.8f, 0.8f, 0.8f );

		//	�J�����ݒ�
		m_Camera = new Camera();

		//	�X�e�[�W
		m_Stage = new iexMesh( "DATA/BG/CollisionGround.IMO" );
		
		//	�����蔻��
		Collision::Initiallize( m_Stage );

		//	�v���C���[
		m_Player = new PlayerManager();
		PlayerInitialize();
		
		//	�R�C��
		m_CoinManager = new CoinManager();
		m_CoinManager->Initialize();

		//	�p�[�e�B�N��
		particle = new iexParticle();
		particle->Initialize( "DATA/Particle.png", 10000 );

		//	�u���b�N�`��
		m_Block = new Block();
		m_Block->Initialize( 0, 0, Vector3( 0.0f, 0.0f, 0.0f ), 0.02f );

		//	�S�̍X�V
		Update();
		return true;
	}

	//	���
	sceneMain::~sceneMain( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_Player );
		SafeDelete( m_Camera );
		SafeDelete( particle );
		SafeDelete( m_CoinManager );
		SafeDelete( m_Block );
	}

	//	�v���C���[������
	void	sceneMain::PlayerInitialize( void )
	{
		m_Player->Initialize( 0, 0, Vector3( 0.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 1, 0, Vector3( 10.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 2, 0, Vector3( 5.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 3, 0, Vector3( -5.0f, 0.0f, 0.0f ) );
	}

//*****************************************************************************************************************************
//
//		�X�V
//
//*****************************************************************************************************************************

	//	�X�V
	void	sceneMain::Update( void )
	{
		//	�v���C���[�X�V
		m_Player->Update();
		
		//	�p�[�e�B�N���X�V
		particle->Update();

		//	�R�C���X�V
		m_CoinManager->Update();

		//	�u���b�N�X�V
		m_Block->Update();
	}

//*****************************************************************************************************************************
//
//		�`��֘A
//
//*****************************************************************************************************************************

	//	�`��
	void	sceneMain::Render( void )
	{
		//	��ʃN���A
		m_Camera->Activate();
		m_Camera->Clear();

		//	�I�u�W�F�N�g�`��
		m_Stage->Render();
		m_Player->Render();
		m_CoinManager->Render();
		//m_Block->Render();
		//	�p�[�e�B�N���`��
		particle->Render();


	}


