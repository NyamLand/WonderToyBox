
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
#include	"StageManager.h"
#include	"Wipe.h"
#include	"Stage.h"

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
	sceneMain::sceneMain( void ) : backBuffer(nullptr), m_screen(nullptr),
		dir( 0.0f, 0.0f, 0.0f ),
		gameStartCoinNum( 0 )
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

		//	���C�g�ݒ�
		dir = Vector3( 0.5f, -1.0f, 1.0f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 0.5f, 0.5f, 0.5f );

		//	�J�����ݒ�
		mainView = new Camera();
		
		//	�v���C���[
		PlayerInitialize();

		//	�v���C���[���C�v
		playerWipe->Initialize();

		//	�����_�[�^�[�Q�b�g�X�N���[��
		m_screen = make_unique<iex2DObj>( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);

		//	�X�e�[�W
		stage = new Stage();
		stage->Initialize();
		stage->LightInitialize( dir );
		
		//	�R�C��
		coinManager->Initialize();

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
		if ( gameManager->GetRound() !=Round::ROUND2 )		sound->PlayBGM( BGM::MAIN_BGM );
		else sound->PlayBGM( BGM::FAINLROUND_BGM );

		return true;
	}

	//	�f�X�g���N�^
	sceneMain::~sceneMain( void )
	{
		SafeDelete( mainView );
		SafeDelete( m_BulletManager );
		SafeDelete( ui );
		SafeDelete( stage );
		backBuffer->Release();
		particle->Release();
		itemManager->Release();
		sound->AllStop();
		coinManager->Release();
		characterManager->Release();
	}

	//	�v���C���[������
	void	sceneMain::PlayerInitialize( void )
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
		playerWipe->Update( mainView->GetTarget() );

		switch ( gameManager->GetMode() )
		{
		case GAME_MODE::GAMESTART:
			gameManager->SetCoinCollision( false );
			StartUpdate();
			break;

		case GAME_MODE::MAINGAME:
			gameManager->SetCoinCollision( true );
			MainGameUpdate();
			break;

		case GAME_MODE::CLIMAX:
			ClimaxUpdate();
			break;

		case GAME_MODE::TIMEUP:
			gameManager->SetCoinCollision( false );
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
		if ( gameStartCoinNum < 50 )
		{
			switch ( gameManager->GetStageType() )
			{
			case 0:
				coinManager->Append( Vector3( 0.0f, 5.0f, -25.0f ), Vector3( Random::GetFloat( -0.5f, 0.5f ), Random::GetFloat( 0.1f, 0.3f ), 1.0f ), Random::GetFloat( 2.0f, 3.5f ), Coin::COIN );
				break;

			case 1:
				coinManager->Append( Vector3( 0.0f, 7.0f, -25.0f ), Vector3( Random::GetFloat( -0.5f, 0.5f ), Random::GetFloat( 0.2f, 0.3f ), 1.0f ), Random::GetFloat( 1.0f, 5.5f ), Coin::COIN );
				break;
			}
			gameStartCoinNum++;
		}

		if ( ui->GetChangeFlag() ) 
		{
			gameManager->SetMode( GAME_MODE::MAINGAME );
			for (int i = 0; i < 4; i++)
				(characterManager->GetIsPlayer(i)) ?
				characterManager->SetMode(i, MODE_STATE::MOVE) :
				characterManager->SetAIMode(i, AI_MODE_STATE::MOVE);
			ui->SetChangeFlag( false );
		}
	}

	//	���C���X�V
	void	sceneMain::MainGameUpdate( void )
	{			
		//	�Q�[���}�l�[�W���[
		gameManager->Update();

		//	�S�̍X�V
		if(gameManager->GetTimeStop() <= 0) AllUpdate();
	}

	//	�N���C�}�b�N�X�X�V
	void	sceneMain::ClimaxUpdate( void )
	{
		//	�Q�[���}�l�[�W���[
		gameManager->Update();

		//	�S�̍X�V
		if ( gameManager->GetTimeStop() <= 0 )
		{
			AllUpdate();
		}
	}

	//	�^�C���A�b�v�X�V
	void	sceneMain::FinishUpdate( void )
	{
		//	�Q�[���}�l�[�W���[
		gameManager->Update();

		//	�S�̍X�V
		if ( gameManager->GetTimeStop() <= 0 ) AllUpdate();
	}

	//	�S�̍X�V
	void	sceneMain::AllUpdate( void )
	{
		//	�A�C�e���X�V
		itemManager->Update();

		//	player
		characterManager->Update();

		//�@�X�e�[�W�X�V
		stage->Update();

		//	�p�[�e�B�N���X�V
		particle->Update();

		//	�C�x���g�X�V
		eventManager->Update();

		//	�R�C���X�V
		coinManager->Update();

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
		ui->RenderTargetParticle();

		//	�v���C���[�̐��@+�@�X�N���[��
		FOR( 0, PLAYER_MAX + 1 )
		{
		//************************************************************
		//		�����_�[�^�[�Q�b�g����
		//************************************************************
			//		�X�N���[��
			if ( value >= PLAYER_MAX )
			{
				m_screen->RenderTarget();
				
				//	��ʃN���A
				mainView->Activate();
				mainView->Clear( 0xFF02B4FE );
			}
			//		�v���C���[�l���C�v
			else
			{
				if ( !playerWipe->GetCheck( value ) )	continue;
				playerWipe->RenderTarget( value );
			}

		//************************************************************
		//	���C���`��
		//************************************************************

			//	�I�u�W�F�N�g�`��
			stage->Render( shader3D, "full_s" );
 			characterManager->Render(shader3D, "toon");
			coinManager->Render( shader3D, "full" );
			m_BulletManager->Render(shader3D, "bullet");
			itemManager->Render();

			//	�p�[�e�B�N���`��
			particle->Render();

			//�@�G�t�F�N�g�`��
			m_Effect->Render();

		}

		//	�t���[���o�b�t�@�֐؂�ւ�
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );

		//	�X�N���[���`��
		m_screen->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight );
		playerWipe->Render();
		
		//UI
		ui->Render(gameManager->GetMode());
	}
