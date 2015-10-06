
#include	"iextreme.h"
#include	"Random.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"CharacterManager.h"
#include	"CoinManager.h"
#include	"ItemManager.h"
#include	"Bullet.h"
#include	"BulletManager.h"
#include	"sceneResult.h"
#include	"UI.h"
#include	"Effect.h"

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

		//	����������
		Random::Initialize();

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

		//	�ϐ�������
		timer = 0;
		playerNum = gameManager->GetPlayerNum();
		stageType = gameManager->GetStageType();

		//	�X�e�[�W
		m_CollisionStage = new iexMesh( "DATA/BG/desk_Collision.IMO" );
		m_Stage = new iexMesh( "DATA/back/stage.IMO" );
		
		//	�����蔻��
		Collision::Initiallize( m_CollisionStage );

		//	�v���C���[
		PlayerInitialize();
		
		//	�R�C��
		m_CoinManager = new CoinManager();
		m_CoinManager->Initialize();

		//	�o���b�g
		m_BulletManager = new BulletManager();
		m_BulletManager->Initialize();

		//	�A�C�e��
		itemManager->Initialize();
			
		//	�p�[�e�B�N��
		particle->Initialize();

		//�@�G�t�F�N�g
		m_Effect = new Effect();
		m_Effect->Initialize();

		//UI
		ui = new UI();
		ui->Initialize();

		//	BGM�Đ�
		sound->PlayBGM( BGM::MAIN_BGM );

		//�@

		//	�S�̍X�V
		Update();
		return true;
	}

	//	�f�X�g���N�^
	sceneMain::~sceneMain( void )
	{
		backBuffer->Release();
		SafeDelete(ShadowTex);
		SafeDelete(m_Camera);
		SafeDelete(m_CollisionStage);
		SafeDelete(m_CoinManager);
		SafeDelete(m_BulletManager);
		SafeDelete(ui);
		Random::Release();
		particle->Release();
		itemManager->Release();
		characterManager->Release();
		sound->AllStop();
	}

	//	�v���C���[������
	void    sceneMain::PlayerInitialize(void)
	{
		/*
			���̃X�R�[�v����PlayerManager��Initialize��Load�̒���
			�R�����g���O������Player��CPU�Ƃ��Ď��ʁ��ǂݍ��݂�����B�i�͂��j
			�i�P �` playerNum�Ԗڂ��v���C���[�AplayerNum �` PLAYER_MAX�Ԗڂ�CPU�j
		*/

		//�@�v���C���[�ݒ�
		//for (int i = 0; i < 4; i++)
		for ( int i = 0; i < playerNum; i++ )
		{
			int        characterType = gameManager->GetCharacterType( i );
			Vector3    pos = Vector3( -20.0f + ( 10.0f * i ), 10.0f, 0.0f );
			characterManager->Initialize( i, characterType, pos, true );
		}

		//�@�b�o�t�ݒ�
		for ( int i = playerNum; i < PLAYER_MAX; i++ )
		{
			int        characterType = gameManager->GetCharacterType( i );
			Vector3    pos = Vector3( -20.0f + ( 10.0f * i ), 10.0f, 0.0f );
			characterManager->Initialize( i, characterType, pos, false );
		}
	}

//*****************************************************************************************************************************
//
//		�X�V
//
//*****************************************************************************************************************************

	//	�X�V
	void	sceneMain::Update( void )
	{
		//	�J�����X�V
		m_Camera->SetPlayerInfo( characterManager->GetPos(0), characterManager->GetPos(1), characterManager->GetPos(2), characterManager->GetPos(3) );
		m_Camera->Update( VIEW_MODE::CHASE, Vector3( 0.0f, 2.0f, 0.0f ) );

		//	UI
		ui->Update( gameManager->GetMode() );

		//	�f�o�b�O���[�h�؂�ւ�
		if ( KEY( KEY_ENTER ) == 3 )		debug = !debug;

		switch ( gameManager->GetMode() )
		{
		case GAME_MODE::GAMESTART:		
			StartUpdate();
			break;

		case GAME_MODE::MAINGAME:
			MainGameUpdate();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			DonketsuUpdate();
			break;

		case GAME_MODE::CLIMAX:
			ClimaxUpdate();
			break;

		case GAME_MODE::TIMEUP:
			FinishUpdate();

			break;
		}

		if (KEY(KEY_D) == 1)
		{
			//particle->Hit(Vector3(0, 10.0f, 0), 20, 0.5f);
			//particle->Smoke(Vector3(0, 10.0f, 0), 20, 0.5f);
			particle->Aura(Vector3(0, 10.0f, 0), 3, 0.5f);

		}

		if ( ui->GetChangeFlag() )
		{
			MainFrame->ChangeScene( new sceneResult() );
			return;
		}
	}

	//	�X�^�[�g�X�V
	void	sceneMain::StartUpdate( void )
	{
		//	�S�̍X�V
		AllUpdate();

		if ( ui->GetChangeFlag() ) 
		{
			gameManager->SetMode( GAME_MODE::MAINGAME );
			for ( int i = 0; i < 4; i++ )		characterManager->SetMode( i, MODE_STATE::MOVE );
			ui->SetChangeFlag( false );
		}
	}

	//	���C���X�V
	void	sceneMain::MainGameUpdate( void )
	{			
		//	�Q�[���}�l�[�W���[
		gameManager->Update();

		//	�S�̍X�V
		AllUpdate();
	}

	//	�ǂ񂯂X�V
	void	sceneMain::DonketsuUpdate( void )
	{
		if ( ui->GetChangeFlag() )
		{
			gameManager->SetMode( GAME_MODE::CLIMAX );
			ui->SetChangeFlag( false );
		}
	}

	//	�N���C�}�b�N�X�X�V
	void	sceneMain::ClimaxUpdate( void )
	{
		//	�Q�[���}�l�[�W���[
		gameManager->Update();

		//	�S�̍X�V
		AllUpdate();

	}

	//	�^�C���A�b�v�X�V
	void	sceneMain::FinishUpdate( void )
	{
		//	�Q�[���}�l�[�W���[
		gameManager->Update();

		//	�S�̍X�V
		AllUpdate();
	}

	//	�S�̍X�V
	void	sceneMain::AllUpdate( void )
	{
		//	player
		characterManager->Update();

		//	�p�[�e�B�N���X�V
		particle->Update();

		//	�R�C���X�V
		m_CoinManager->Update();

		//	���X�@�o���b�g�X�V
		m_BulletManager->Update();

		//�@�G�t�F�N�g�X�V
		m_Effect->Update();

		//	�A�C�e���X�V
		itemManager->Update();
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
		characterManager->Render( shader3D, "toon" );
		m_CoinManager->Render();
		m_BulletManager->Render();
		itemManager->Render();
		
		//UI
		ui->Render( gameManager->GetMode() );

		//�@�G�t�F�N�g�`��
		m_Effect->Render();

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
		//m_Stage->Render( shader3D, "shadowBuf" );
		characterManager->Render( shader3D, "ShadowBuf" );
		m_CoinManager->Render( shader3D, "ShadowBuf" );
		itemManager->Render( shader3D, "ShadowBuf" );

		//	������V���h�E�e�N�X�`�����V�F�[�_�[�ɃZ�b�g
		shader3D->SetValue( "ShadowMap", ShadowTex );

		//	�����_�[�^�[�Q�b�g�̕���
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		iexSystem::GetDevice()->SetDepthStencilSurface( orgZ );
	}
