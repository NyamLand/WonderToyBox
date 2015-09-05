
#include	"iextreme.h"
#include	<random>
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GameManager.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"Bullet.h"
#include	"BulletManager.h"
#include	"sceneResult.h"
#include	"UI.h"

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
	sceneMain::sceneMain( void ) : m_Stage( NULL ), playerNum( 0 ), stageType( 0 )
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
		m_CollisionStage = new iexMesh( "DATA/BG/desk_Collision.IMO" );
		m_Stage = new iexMesh( "DATA/back/stage.IMO" );
		//	�����蔻��
		Collision::Initiallize( m_CollisionStage );

		//	�v���C���[
		m_Player = new PlayerManager();
		PlayerInitialize();
		
		//	�R�C��
		m_CoinManager = new CoinManager();
		m_CoinManager->Initialize();

		m_BulletManager = new BulletManager();
		m_BulletManager->Initialize();

		//	�p�[�e�B�N��
		Particle::Initialize();

		//UI
		m_UI = new HeadUpDisplay();
		m_UI->Initialize();

		//	�f�B�t�@�[�h�֌W������
		DifferedInitialize();

		//	�ϐ�������
		timer = 0;
		playerNum = GameManager::GetPlayerNum();
		stageType = GameManager::GetStageType();

		PlayBGM( SoundInfo::BGM::MAIN_BGM );

		//	�S�̍X�V
		Update();
		return true;
	}

	//	�f�X�g���N�^
	sceneMain::~sceneMain( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_CollisionStage );
		SafeDelete( m_Camera );
		SafeDelete( m_CoinManager );
		SafeDelete( m_BulletManager );
		SafeDelete( ShadowTex );
		SafeDelete( m_UI );
		SafeDelete( diffuse );
		SafeDelete( specular );
		SafeDelete( depth );
		SafeDelete( normal );
		SafeDelete( light );
		SafeDelete( light_s );
		SafeDelete( screen );
		SafeDelete( m_Player );
		backBuffer->Release();
		Particle::Release();
	}

	//	�v���C���[������
	void	sceneMain::PlayerInitialize( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			int		characterType = GameManager::GetCharacterType( i );
			Vector3	pos = Vector3( -20.0f + ( 10.0f * i ), 10.0f, 0.0f );
			m_Player->Initialize( i, characterType, pos );
		}
	}

	//	�f�B�t�@�[�h������
	void	sceneMain::DifferedInitialize( void )
	{
		//	�ꎞ�����_�����O�p�T�[�t�F�C�X
		screen = new iex2DObj( 1280,  720, IEX2D_RENDERTARGET );

		diffuse = new iex2DObj( 1280, 720, IEX2D_RENDERTARGET );
		specular = new iex2DObj( 1280, 720, IEX2D_RENDERTARGET );
		depth = new iex2DObj( 1280, 720, IEX2D_FLOAT );
		normal = new iex2DObj( 1280, 720, IEX2D_RENDERTARGET );
		light = new iex2DObj( 1280, 720, IEX2D_RENDERTARGET );
		light_s = new iex2DObj( 1280, 720, IEX2D_RENDERTARGET );
		shaderD->SetValue( "DepthBuf", depth );
		shaderD->SetValue( "SpecularBuf", specular );
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
		shader3D->SetValue( "plight_range", 6.0f );
		shader3D->SetValue( "plight_color", Vector3( 2.0f, 0.0f, 2.0f ) );
		
		//	�p�[�e�B�N���X�V
		Particle::Update();

		//	�R�C���X�V
		m_CoinManager->Update();

		//	���X�@�o���b�g�X�V
		m_BulletManager->Update();

		//	�J�����X�V
		m_Camera->Update( VIEW_MODE::FIX, Vector3( 0.0f, 2.0f, 0.0f ) );
		shader3D->SetValue( "ViewPos", m_Camera->GetPos() );
		shader3D->SetValue( "matView", m_Camera->GetMatrix() );

		//	�f�o�b�O���[�h�؂�ւ�
		if ( KEY( KEY_ENTER ) == 3 )		debug = !debug;
		
		//	�V�[���؂�ւ�
		if ( GameManager::GetChangeSceneFlag() )
		{
			MainFrame->ChangeScene( new sceneResult() );
			return;
		}

		//	�Q�[���}�l�[�W���[
		GameManager::Update();
	}

//*****************************************************************************************************************************
//
//		����֐�
//
//*****************************************************************************************************************************

	//	�_����
	void	sceneMain::PointLight( const Vector3& pos, const Vector3& color, float range )
	{
		//	�ϊ��s��ݒ�
		Matrix	InvProj;
		D3DXMatrixInverse( &InvProj, NULL, &matProjection );
		shaderD->SetValue( "InvProj", InvProj );

		Matrix	mat = matView;

		//	�J������ԕϊ�
		Vector3	LightPos;
		LightPos.x = pos.x * mat._11 + pos.y * mat._21 + pos.z * mat._31 + mat._41;
		LightPos.y = pos.x * mat._12 + pos.y * mat._22 + pos.z * mat._32 + mat._42;
		LightPos.z = pos.x * mat._13 + pos.y * mat._23 + pos.z * mat._33 + mat._43;

		//	�V�F�[�_�[�ݒ�
		shaderD->SetValue( "pLightPos", LightPos );
		shaderD->SetValue( "pLightColor", ( Vector3 )color );
		shaderD->SetValue( "pLightRange", range );

		//	�����_�����O
		normal->Render( shaderD, "pointlight" );
	}

	//	���s��
	void	sceneMain::DirLight( const Vector3& dir, const Vector3& color )
	{
		Matrix	mat = matView;
		Vector3	LightDir;

		//	�J������ԕϊ�
		LightDir.x = dir.x * mat._11 + dir.y * mat._21 + dir.z * mat._31;
		LightDir.y = dir.x * mat._12 + dir.y * mat._22 + dir.z * mat._32;
		LightDir.z = dir.x * mat._13 + dir.y * mat._23 + dir.z * mat._33;
		LightDir.Normalize();

		//	�V�F�[�_�[�ݒ�
		shaderD->SetValue( "LightVec", LightDir );
		shaderD->SetValue( "LightColor", ( Vector3 )color );

		//	�����_�����O
		normal->Render( shaderD, "dirlight2" );
	}

//*****************************************************************************************************************************
//
//		�`��֘A
//
//*****************************************************************************************************************************

	//	�`��
	void	sceneMain::Render( void )
	{
		//	�J�����ݒ�
		m_Camera->Activate();
		m_Camera->Clear();
		
		//	�e
		RenderShadowBuffer();
		
		//	�I�u�W�F�N�g�`��
		m_Stage->Render( shader3D, "nolight_s" );
		m_Player->Render( shader3D, "toon" );
		m_CoinManager->Render();
		m_BulletManager->Render();
		
		//UI
		GameManager::Render();

		//	�p�[�e�B�N���`��
		Particle::Render();
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
		//m_Stage->Render( shader3D, "shadowBuf" );
		m_Player->Render( shader3D, "ShadowBuf" );
		m_CoinManager->Render( shader3D, "ShadowBuf" );

		//	������V���h�E�e�N�X�`�����V�F�[�_�[�ɃZ�b�g
		shader3D->SetValue( "ShadowMap", ShadowTex );

		//	�����_�[�^�[�Q�b�g�̕���
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		iexSystem::GetDevice()->SetDepthStencilSurface( orgZ );
	}

	//	�f�B�t�@�[�h�`��
	void	sceneMain::RenderDiffered( void )
	{
		//	�f�B�t�@�[�h�ݒ�
		diffuse->RenderTarget();
		specular->RenderTarget( 1 );
		depth->RenderTarget( 2 );
		normal->RenderTarget( 3 );

		//	�r���[�ݒ�
		m_Camera->Activate();
		m_Camera->Clear();
		shaderD->SetValue( "matView", matView );

		//	�ϊ��s��ݒ�
		Matrix	InvProj;
		D3DXMatrixInverse( &InvProj, NULL, &matProjection );
		shaderD->SetValue( "InvProj", InvProj );

		//	���̕`��
		m_Stage->Render( shaderD, "differed" );
		m_Player->Render( shaderD, "differed" );
		m_CoinManager->Render( shaderD, "differed" );

		//	���C�g�o�b�t�@�쐬
		light->RenderTarget();
		light_s->RenderTarget( 1 );
		iexSystem::Device->SetRenderTarget( 2, NULL );
		iexSystem::Device->SetRenderTarget( 3, NULL );
		m_Camera->Clear( 0x000000 );

		//	���s��
		//DirLight( Vector3( 1.0f, -1.0f, -0.5f ), Vector3( 0.1f, 0.1f, 0.1f ));

		//	�_����
		PointLight( Vector3( 0.0f, 3.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), 5.0f );

		//	�ꎞ�o�b�t�@�֐؂�ւ�
		screen->RenderTarget();
		iexSystem::Device->SetRenderTarget( 1, NULL );
		iexSystem::Device->SetRenderTarget( 2, NULL );
		iexSystem::Device->SetRenderTarget( 3, NULL );
		m_Camera->Clear();


	}

	
