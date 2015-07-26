
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
//	TAKATORI

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

		//	�o�b�N�o�b�t�@�|�C���^�ޔ�
		iexSystem::GetDevice()->GetRenderTarget( 0, &backBuffer );

		//	�V���h�E�}�b�v
		ShadowTex = new iex2DObj( SHADOW_SIZE, SHADOW_SIZE, IEX2D_RENDERTARGET );
		iexSystem::GetDevice()->CreateDepthStencilSurface( SHADOW_SIZE, SHADOW_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE,	0, FALSE, &ShadowZ, NULL );

		//	���C�g�ݒ�
		Vector3 dir( 1.0f, -1.0f, -0.5f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 0.8f, 0.8f, 0.8f );

		//	�J�����ݒ�
		m_Camera = new Camera();

		//	�X�e�[�W
		m_CollisionStage = new iexMesh( "DATA/BG/CollisionGround.IMO" );
		m_Stage = new iexMesh( "DATA/BG/2_1/FIELD2_1.IMO" );
		
		//	�����蔻��
		Collision::Initiallize( m_CollisionStage );

		//	�v���C���[
		m_Player = new PlayerManager();
		PlayerInitialize();
		
		//	�R�C��
		m_CoinManager = new CoinManager();
		m_CoinManager->Initialize();

		//	�p�[�e�B�N��
		particle = new iexParticle();
		particle->Initialize( "DATA/Particle.png", 10000 );

		//	�S�̍X�V
		Update();
		return true;
	}

	//	���
	sceneMain::~sceneMain( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_CollisionStage );
		SafeDelete( m_Player );
		SafeDelete( m_Camera );
		SafeDelete( particle );
		SafeDelete( m_CoinManager );
		SafeDelete( ShadowTex );
		backBuffer->Release();
	}

	//	�v���C���[������
	void	sceneMain::PlayerInitialize( void )
	{
		m_Player->Initialize( 0, PlayerData::Y2009, Vector3( 0.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 1, PlayerData::ECCMAN, Vector3( 10.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 2, PlayerData::Y2009, Vector3( 5.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 3, PlayerData::Y2009, Vector3( -5.0f, 0.0f, 0.0f ) );
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
		
		//	�_�����ݒ�
		shader3D->SetValue( "plight_pos", Vector3( 0.0f, 0.0f, 0.0f ) );
		shader3D->SetValue("plight_range", 6.0f );
		shader3D->SetValue("plight_color", Vector3( 2.0f, 0.0f, 2.0f ) );
		
		//	�p�[�e�B�N���X�V
		particle->Update();

		//	�R�C���X�V
		m_CoinManager->Update();

		//	�J�����X�V
		m_Camera->Update( VIEW_MODE::FIX, Vector3( 0.0f, 2.0f, 0.0f ) );

		shader3D->SetValue( "ViewPos", m_Camera->GetPos() );
		shader3D->SetValue( "matView", m_Camera->GetMatrix() );
	}

//*****************************************************************************************************************************
//
//		�`��֘A
//
//*****************************************************************************************************************************

	//	�`��
	void	sceneMain::Render( void )
	{
		//	�e
		RenderShadowBuffer();

		//	��ʃN���A
		m_Camera->Activate();
		m_Camera->Clear();

		//	�I�u�W�F�N�g�`��
		m_Stage->Render( shader3D, "full_s" );
		m_Player->Render( shader3D, "full" );
		m_CoinManager->Render();

		//	�p�[�e�B�N���`��
		particle->Render();
	}

	//	�V���h�E�o�b�t�@�`��
	void	sceneMain::RenderShadowBuffer( void )
	{
		ShadowTex->RenderTarget();

		//	Z�o�b�t�@�ݒ�
		Surface*	orgZ;
		iexSystem::GetDevice()->GetDepthStencilSurface( &orgZ );
		iexSystem::GetDevice()->SetDepthStencilSurface( ShadowZ );

		//	���C�g����
		Vector3 dir( 0.5f, -1.0f, -0.5f );
		dir.Normalize();

		//	�V���h�E�쐬
		Vector3	target( 0.0f, 0.0f, 0.0f );
		Vector3	pos = target - dir * 10;
		Vector3	up( 0.0f, 1.0f, 0.0f );

		//	���_�ƃ��C�g�ʒu��
		D3DXMATRIX		ShadowMat, work;
		LookAtLH( ShadowMat, pos, target, up );
		D3DXMatrixOrthoLH( &work, 100, 100, -100.0, 100.0f );	//	���s���e�s��i�͈́~100�j
		ShadowMat *= work;

		shader3D->SetValue( "ShadowProjection", &ShadowMat );

		D3DVIEWPORT9	vp = { 0, 0, SHADOW_SIZE, SHADOW_SIZE, 0, 1.0f };
		iexSystem::GetDevice()->SetViewport( &vp );

		//	�����_�����O
		iexSystem::GetDevice()->Clear( 0, NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0 );

		//	�`��
		m_Player->Render( shader3D, "ShadowBuf" );
		//m_CoinManager->Render( shader3D, "ShadowBuf" );

		//	������V���h�E�e�N�X�`�����V�F�[�_�[�ɃZ�b�g
		shader3D->SetValue( "ShadowMap", ShadowTex );

		//	�����_�[�^�[�Q�b�g�̕���
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		iexSystem::GetDevice()->SetDepthStencilSurface( orgZ );


	}


