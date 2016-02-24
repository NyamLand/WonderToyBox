
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
//	グローバル変数
//
//*****************************************************************************************************************************

//*****************************************************************************************************************************
//
//	初期化
//
//*****************************************************************************************************************************

	//	コンストラクタ
	sceneMain::sceneMain( void ) : backBuffer(nullptr), m_screen(nullptr),
		dir( 0.0f, 0.0f, 0.0f ),
		gameStartCoinNum( 0 )
	{
		
	}

	//	初期化
	bool	sceneMain::Initialize( void )
	{
		//	環境設定
		iexLight::SetAmbient( 0x404040 );
		iexLight::SetFog( 800, 1000, 0 );

		//	乱数初期化
		Random::Initialize();

		//	バックバッファポインタ退避
		iexSystem::GetDevice()->GetRenderTarget( 0, &backBuffer );

		//	ライト設定
		dir = Vector3( 0.5f, -1.0f, 1.0f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 0.5f, 0.5f, 0.5f );

		//	カメラ設定
		mainView = new Camera();
		
		//	プレイヤー
		PlayerInitialize();

		//	プレイヤーワイプ
		playerWipe->Initialize();

		//	レンダーターゲットスクリーン
		m_screen = make_unique<iex2DObj>( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);

		//	ステージ
		stage = new Stage();
		stage->Initialize();
		stage->LightInitialize( dir );
		
		//	コイン
		coinManager->Initialize();

		//	バレット
		m_BulletManager = new BulletManager();
		m_BulletManager->Initialize();

		//	アイテム
		itemManager->Initialize();
			
		//	パーティクル
		particle->Initialize();

		//　エフェクト
		m_Effect = new Effect();
		m_Effect->Initialize();

		//UI
		ui = new UI();
		ui->Initialize();

		//	BGM再生
		if ( gameManager->GetRound() !=Round::ROUND2 )		sound->PlayBGM( BGM::MAIN_BGM );
		else sound->PlayBGM( BGM::FAINLROUND_BGM );

		return true;
	}

	//	デストラクタ
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

	//	プレイヤー初期化
	void	sceneMain::PlayerInitialize( void )
	{
		//　プレイヤー設定
		for ( int i = 0; i < gameManager->GetPlayerNum(); i++ )
		{
			int        characterType = gameManager->GetCharacterType( i );
			Vector3    pos = gameManager->InitPos[i];
			characterManager->Initialize( i, characterType, pos, true );
		}

		//　ＣＰＵ設定
		for (int i = gameManager->GetPlayerNum(); i < PLAYER_MAX; i++)
		{
			int        characterType = gameManager->GetCharacterType( i );
			Vector3    pos = gameManager->InitPos[i];
			characterManager->Initialize( i, characterType, pos, false );
		}
	}

//*****************************************************************************************************************************
//
//		更新
//
//*****************************************************************************************************************************

	//	更新
	void	sceneMain::Update( void )
	{
		//	カメラ更新
		mainView->SetPlayerInfo( characterManager->GetPos(0), characterManager->GetPos(1), characterManager->GetPos(2), characterManager->GetPos(3) );
		mainView->Update( VIEW_MODE::CHASE, Vector3( 0.0f, 2.0f, 0.0f ) );

		//	UI
		ui->Update( gameManager->GetMode() );

		//	Playerワイプ更新
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

	//	スタート更新
	void	sceneMain::StartUpdate( void )
	{
		//	全体更新
		AllUpdate();

		//	とりあえず仮
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

	//	メイン更新
	void	sceneMain::MainGameUpdate( void )
	{			
		//	ゲームマネージャー
		gameManager->Update();

		//	全体更新
		if(gameManager->GetTimeStop() <= 0) AllUpdate();
	}

	//	クライマックス更新
	void	sceneMain::ClimaxUpdate( void )
	{
		//	ゲームマネージャー
		gameManager->Update();

		//	全体更新
		if ( gameManager->GetTimeStop() <= 0 )
		{
			AllUpdate();
		}
	}

	//	タイムアップ更新
	void	sceneMain::FinishUpdate( void )
	{
		//	ゲームマネージャー
		gameManager->Update();

		//	全体更新
		if ( gameManager->GetTimeStop() <= 0 ) AllUpdate();
	}

	//	全体更新
	void	sceneMain::AllUpdate( void )
	{
		//	アイテム更新
		itemManager->Update();

		//	player
		characterManager->Update();

		//　ステージ更新
		stage->Update();

		//	パーティクル更新
		particle->Update();

		//	イベント更新
		eventManager->Update();

		//	コイン更新
		coinManager->Update();

		//	リス　バレット更新
		m_BulletManager->Update();

		//　エフェクト更新
		m_Effect->Update();
	}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

	//	描画
	void	sceneMain::Render( void )
	{
		ui->RenderTargetParticle();

		//	プレイヤーの数　+　スクリーン
		FOR( 0, PLAYER_MAX + 1 )
		{
		//************************************************************
		//		レンダーターゲット処理
		//************************************************************
			//		スクリーン
			if ( value >= PLAYER_MAX )
			{
				m_screen->RenderTarget();
				
				//	画面クリア
				mainView->Activate();
				mainView->Clear( 0xFF02B4FE );
			}
			//		プレイヤー個人ワイプ
			else
			{
				if ( !playerWipe->GetCheck( value ) )	continue;
				playerWipe->RenderTarget( value );
			}

		//************************************************************
		//	メイン描画
		//************************************************************

			//	オブジェクト描画
			stage->Render( shader3D, "full_s" );
 			characterManager->Render(shader3D, "toon");
			coinManager->Render( shader3D, "full" );
			m_BulletManager->Render(shader3D, "bullet");
			itemManager->Render();

			//	パーティクル描画
			particle->Render();

			//　エフェクト描画
			m_Effect->Render();

		}

		//	フレームバッファへ切り替え
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );

		//	スクリーン描画
		m_screen->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight );
		playerWipe->Render();
		
		//UI
		ui->Render(gameManager->GetMode());
	}
