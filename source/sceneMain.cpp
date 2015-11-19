
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
#include	"EventManager.h"
#include	"CoinManager.h"
#include	"ItemManager.h"
#include	"BulletManager.h"
#include	"sceneResult.h"
#include	"UI.h"
#include	"Effect.h"
#include	"Wipe.h"

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
	sceneMain::sceneMain( void ) : m_Stage( NULL ), playerNum(0), stageType(0), stageScale( 1.0f )
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
		hdr = new iex2DObj( 512, 512, IEX2D_RENDERTARGET );

		//	�V���h�E�}�b�v
		ShadowTex = new iex2DObj( SHADOW_SIZE, SHADOW_SIZE, IEX2D_RENDERTARGET );
		iexSystem::GetDevice()->CreateDepthStencilSurface( SHADOW_SIZE, SHADOW_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE,	0, FALSE, &ShadowZ, NULL );

		//	���˃}�b�v
		RefTex = new iex2DObj( 1280, 720, IEX2D_RENDERTARGET );

		//	���C�g�ݒ�
		dir = Vector3( 1.0f, -2.0f, -1.0f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 0.5f, 0.5f, 0.5f );

		//	�J�����ݒ�
		mainView = new Camera();

		//	�v���C���[���C�v
		playerWipe->Initialize();

		//	�����_�[�^�[�Q�b�g�X�N���[��
		m_screen = make_unique<iex2DObj>( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);

		//	gameManager������擾
		playerNum = gameManager->GetPlayerNum();
		stageType = gameManager->GetStageType();
		gameStartCoinNum = 0;

		//	�X�e�[�W
		StageInitialize();

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
		ui->Initialize( UI_MODE::MAIN );

		//	BGM�Đ�
		sound->PlayBGM( BGM::MAIN_BGM );

		/*
			���C������n�߂邽�߂ɖ������Ăяo���Ă��܂��B
			�{�`�����ɖ߂��ꍇ��WinMain�̃V�[���ǂݍ��݂��^�C�g���ɖ߂��āA
			���́��̊֐����R�����g�A�E�g���Ă��������B
		*/
		//InitializeDebug();

		return true;
	}

	void	sceneMain::InitializeDebug(void)
	{
		gameManager->InitializeDebug();
		gameStartCoinNum = 0;

		//�@�X�e�[�W
		StageInitialize();

		//�@�v���C���[�ECPU
		PlayerInitialize();

		m_BulletManager->Initialize();
		itemManager->Initialize();
		m_CoinManager->Initialize();
		m_Effect->Initialize();
		ui->Initialize(UI_MODE::MAIN);
	}

	//	�f�X�g���N�^
	sceneMain::~sceneMain( void )
	{
		backBuffer->Release();
		SafeDelete( ShadowTex );
		SafeDelete( RefTex );
		SafeDelete( mainView );
		SafeDelete( m_CollisionStage );
		SafeDelete( m_CoinManager );
		SafeDelete( m_BulletManager );
		SafeDelete( ui );

		//SafeDelete( diffuse );
		//SafeDelete( specular );
		//SafeDelete( depth );
		//SafeDelete( normal );
		//SafeDelete( light );
		//SafeDelete( light_s );

		Random::Release();
		particle->Release();
		itemManager->Release();
		characterManager->Release();
		sound->AllStop();
	}

	//	�v���C���[������
	void    sceneMain::PlayerInitialize(void)
	{
		//�@�v���C���[�ݒ�
		for ( int i = 0; i < gameManager->GetPlayerNum(); i++ )
		{
			int        characterType = gameManager->GetCharacterType( i );
			Vector3    pos = gameManager->InitPos[i];
			characterManager->Initialize( i, characterType, pos, true );
		}

		//�@�b�o�t�ݒ�
		for (int i = gameManager->GetPlayerNum(); i < PLAYER_MAX; i++)
		{
			int        characterType = gameManager->GetCharacterType( i );
			Vector3    pos = gameManager->InitPos[i];
			characterManager->Initialize( i, characterType, pos, false );
		}
	}

	//	�X�e�[�W������
	void	sceneMain::StageInitialize( void )
	{
		switch ( gameManager->GetStageType() )
		{
		case 0:	//	���X�e�[�W
			m_CollisionStage = new iexMesh( "DATA/back/Collision.IMO" );
			m_Stage = new iexMesh( "DATA/back/stage.IMO" );
			iexLight::DirLight( shader3D, 0, &dir, 1.5f, 1.5f, 1.5f );
			m_Stage->SetAngle( D3DX_PI );
			break;

		case 1:	//	�X�X�e�[�W
			m_CollisionStage = new iexMesh( "DATA/BG/Forest/Collision/collision_forest.IMO" );
			m_Stage = new iexMesh( "DATA/BG/Forest/model/forest.IMO" );
			iexLight::DirLight( shader3D, 0, &dir, 0.5f, 0.5f, 0.5f );
			m_Stage->SetScale( stageScale );
			break;
		}
		
		//	�����ڃ��f�����P�W�O����]���ď��X�V
		//m_Stage->SetAngle( D3DX_PI );
		m_Stage->Update();

		//	�����蔻��p���f���o�^
		Collision::Initiallize( m_CollisionStage );
	}

	//	�f�B�t�@�[�h������
	void	sceneMain::DifferedInitialize( void )
	{
		//diffuse = new iex2DObj(1280, 720, IEX2D_RENDERTARGET);
		//specular = new iex2DObj(1280, 720, IEX2D_RENDERTARGET);
		//depth = new iex2DObj(1280, 720, IEX2D_FLOAT);
		//normal = new iex2DObj(1280, 720, IEX2D_RENDERTARGET);
		//light = new iex2DObj(1280, 720, IEX2D_RENDERTARGET);
		//light_s = new iex2DObj(1280, 720, IEX2D_RENDERTARGET);
		//shaderD->SetValue("DepthBuf", depth);
		//shaderD->SetValue("SpecularBuf", specular);
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
		mainView->SetPlayerInfo( characterManager->GetPos(0), characterManager->GetPos(1), characterManager->GetPos(2), characterManager->GetPos(3) );
		mainView->Update( VIEW_MODE::CHASE, Vector3( 0.0f, 2.0f, 0.0f ) );

		//	UI
		ui->Update( gameManager->GetMode() );

		//	Player���C�v�X�V
		playerWipe->Update();
		FOR(0, PLAYER_MAX)
		{
			playerWipe->Check( value );
			playerWipe->Arrow( value, mainView->GetTarget() );
			playerWipe->Move( value, mainView->GetTarget() );
			Vector3 test = characterManager->GetPos(0) - mainView->GetTarget();
			
			printf(" %f\n", test.Length());
		}

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

		//	�Ƃ肠������
		if ( gameStartCoinNum < GAME_START_COIN_NUM )
		{
			switch ( gameManager->GetStageType() )
			{
			case 0:
				m_CoinManager->Set( Vector3( 0.0f, 5.0f, -25.0f ), Vector3( Random::GetFloat( -0.5f, 0.5f ), Random::GetFloat( 0.1f, 0.3f ), 1.0f ), Random::GetFloat( 2.0f, 3.5f ) );
				break;

			case 1:
				m_CoinManager->Set( Vector3( 0.0f, 7.0f, -25.0f ), Vector3( Random::GetFloat( -0.5f, 0.5f ), Random::GetFloat( 0.2f, 0.3f ), 1.0f ), Random::GetFloat( 1.0f, 5.5f ) );
				break;
			}
			gameStartCoinNum++;
		}

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
		//	�A�C�e���X�V
		itemManager->Update();

		//	player
		characterManager->Update();

		//	�p�[�e�B�N���X�V
		particle->Update();

		//	�C�x���g�X�V
		eventManager->Update();

		//	�R�C���X�V
		m_CoinManager->Update();

		//	���X�@�o���b�g�X�V
		m_BulletManager->Update();

		//�@�G�t�F�N�g�X�V
		m_Effect->Update();

	}

//*****************************************************************************************************************************
//
//		�`��֘A
//
//*****************************************************************************************************************************

	//	�`��
	void	sceneMain::Render( void )
	{
		//	�v���C���[�̐��@+�@�X�N���[��
		FOR(0, PLAYER_MAX + 1){
		//************************************************************
		//		�����_�[�^�[�Q�b�g����
		//************************************************************
			//		�X�N���[��
			if (value >= PLAYER_MAX)
			{
				m_screen->RenderTarget();
				//	��ʃN���A
				mainView->Activate();
				mainView->Clear();
			}
			//		�v���C���[�l���C�v
			else
			{
				playerWipe->RenderTarget( value );
			}

		//************************************************************
		//	���C���`��
		//************************************************************

			//	�I�u�W�F�N�g�`��
			m_Stage->Render(shader3D, "full_s");
			characterManager->Render(shader3D, "toon");
			m_CoinManager->Render(shader3D, "full");
			m_BulletManager->Render();
			itemManager->Render();

			//	�p�[�e�B�N���`��
			particle->Render();

			//�@�G�t�F�N�g�`��
			m_Effect->Render();

		}
		//-------------------
		//	�X�N���[���֕`��
		//-------------------
		//UI	
		ui->Render(gameManager->GetMode());


		//	�t���[���o�b�t�@�֐؂�ւ�
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );

		m_screen->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight );
		playerWipe->Render();
	}

	//	HDR�`��
	void	sceneMain::RenderHDR( void )
	{
		//	HDR�p�o�b�t�@�֐؂�ւ�
		hdr->RenderTarget();
		mainView->SetViewport( 0, 0, 512, 512 );
		mainView->SetProjection( 0.8f, 0.1f, 300.0f, 1280.0f / 720.0f );
		mainView->Activate();
		mainView->Clear();

		//	���̂̕`��
		m_Stage->Render( shader3D, "specular" );
		m_CoinManager->Render( shader3D, "specular" );
		characterManager->Render( shader3D, "specular" );
		itemManager->Render( shader3D, "specular" );

		//	�ڂ���
		DWORD	color = ( ( 8 * 6 ) << 24 ) | 0xFFFFFF;
		for ( int i = 0; i < 24; i += 4 )
		{
			hdr->Render( -i, 0, 512, 512, 0, 0, 512, 512, RS_ADD, color );
			hdr->Render( i, 0, 512, 512, 0, 0, 512, 512, RS_ADD, color );
			hdr->Render( 0, -i, 512, 512, 0, 0, 512, 512, RS_ADD, color );
			hdr->Render( 0, i, 512, 512, 0, 0, 512, 512, RS_ADD, color );
			color -= 8 << 24;
		}

		//	�o�b�N�o�b�t�@�֐؂�ւ�
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
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
		//characterManager->Render( shader3D, "ShadowBuf" );
		//m_CoinManager->Render( shader3D, "ShadowBuf" );
		//itemManager->Render( shader3D, "ShadowBuf" );

		//	������V���h�E�e�N�X�`�����V�F�[�_�[�ɃZ�b�g
		shader3D->SetValue( "ShadowMap", ShadowTex );

		//	�����_�[�^�[�Q�b�g�̕���
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		iexSystem::GetDevice()->SetDepthStencilSurface( orgZ );
	}

	//	����
	void	sceneMain::RenderRef( void )
	{
		RefTex->RenderTarget();
		mainView->SetViewport();

		mainView->Activate();
		mainView->Clear();

		//	���̕`��
		m_Stage->Render( shader3D, "Refrect" );
		characterManager->Render( shader3D, "Refrect" );
		shader3D->SetValue( "RefMap", RefTex );

		//	�����_�[�^�[�Q�b�g�̕���
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
	}
