
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
//	グローバル変数
//
//*****************************************************************************************************************************


//*****************************************************************************************************************************
//
//	初期化
//
//*****************************************************************************************************************************

	//	コンストラクタ
	sceneMain::sceneMain( void ) : m_Stage( NULL ), playerNum( 0 ), stageType( 0 )
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

		//	シャドウマップ
		ShadowTex = new iex2DObj( SHADOW_SIZE, SHADOW_SIZE, IEX2D_RENDERTARGET );
		iexSystem::GetDevice()->CreateDepthStencilSurface( SHADOW_SIZE, SHADOW_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE,	0, FALSE, &ShadowZ, NULL );

		//	ライト設定
		Vector3 dir( 1.0f, -1.0f, -0.5f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 0.8f, 0.8f, 0.8f );

		//	カメラ設定
		m_Camera = new Camera();

		//	変数初期化
		timer = 0;
		playerNum = gameManager->GetPlayerNum();
		stageType = gameManager->GetStageType();

		//	ステージ
		m_CollisionStage = new iexMesh( "DATA/BG/desk_Collision.IMO" );
		m_Stage = new iexMesh( "DATA/back/stage.IMO" );
		
		//	当たり判定
		Collision::Initiallize( m_CollisionStage );

		//	プレイヤー
		PlayerInitialize();
		
		//	コイン
		m_CoinManager = new CoinManager();
		m_CoinManager->Initialize();

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
		sound->PlayBGM( BGM::MAIN_BGM );

		//　

		//	全体更新
		Update();
		return true;
	}

	//	デストラクタ
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

	//	プレイヤー初期化
	void    sceneMain::PlayerInitialize(void)
	{
		/*
			このスコープ内＆PlayerManagerのInitializeとLoadの中の
			コメントを外したらPlayer＆CPUとして識別＆読み込みをする。（はず）
			（１ ～ playerNum番目がプレイヤー、playerNum ～ PLAYER_MAX番目がCPU）
		*/

		//　プレイヤー設定
		//for (int i = 0; i < 4; i++)
		for ( int i = 0; i < playerNum; i++ )
		{
			int        characterType = gameManager->GetCharacterType( i );
			Vector3    pos = Vector3( -20.0f + ( 10.0f * i ), 10.0f, 0.0f );
			characterManager->Initialize( i, characterType, pos, true );
		}

		//　ＣＰＵ設定
		for ( int i = playerNum; i < PLAYER_MAX; i++ )
		{
			int        characterType = gameManager->GetCharacterType( i );
			Vector3    pos = Vector3( -20.0f + ( 10.0f * i ), 10.0f, 0.0f );
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
		m_Camera->SetPlayerInfo( characterManager->GetPos(0), characterManager->GetPos(1), characterManager->GetPos(2), characterManager->GetPos(3) );
		m_Camera->Update( VIEW_MODE::CHASE, Vector3( 0.0f, 2.0f, 0.0f ) );

		//	UI
		ui->Update( gameManager->GetMode() );

		//	デバッグモード切り替え
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

	//	スタート更新
	void	sceneMain::StartUpdate( void )
	{
		//	全体更新
		AllUpdate();

		if ( ui->GetChangeFlag() ) 
		{
			gameManager->SetMode( GAME_MODE::MAINGAME );
			for ( int i = 0; i < 4; i++ )		characterManager->SetMode( i, MODE_STATE::MOVE );
			ui->SetChangeFlag( false );
		}
	}

	//	メイン更新
	void	sceneMain::MainGameUpdate( void )
	{			
		//	ゲームマネージャー
		gameManager->Update();

		//	全体更新
		AllUpdate();
	}

	//	どんけつ更新
	void	sceneMain::DonketsuUpdate( void )
	{
		if ( ui->GetChangeFlag() )
		{
			gameManager->SetMode( GAME_MODE::CLIMAX );
			ui->SetChangeFlag( false );
		}
	}

	//	クライマックス更新
	void	sceneMain::ClimaxUpdate( void )
	{
		//	ゲームマネージャー
		gameManager->Update();

		//	全体更新
		AllUpdate();

	}

	//	タイムアップ更新
	void	sceneMain::FinishUpdate( void )
	{
		//	ゲームマネージャー
		gameManager->Update();

		//	全体更新
		AllUpdate();
	}

	//	全体更新
	void	sceneMain::AllUpdate( void )
	{
		//	player
		characterManager->Update();

		//	パーティクル更新
		particle->Update();

		//	コイン更新
		m_CoinManager->Update();

		//	リス　バレット更新
		m_BulletManager->Update();

		//　エフェクト更新
		m_Effect->Update();

		//	アイテム更新
		itemManager->Update();
	}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

	//	描画
	void	sceneMain::Render( void )
	{
		//	カメラ設定
		m_Camera->Activate();
		m_Camera->Clear();
		
		//	影
		RenderShadowBuffer();
		
		//	オブジェクト描画
		m_Stage->Render( shader3D, "nolight_s" );
		characterManager->Render( shader3D, "toon" );
		m_CoinManager->Render();
		m_BulletManager->Render();
		itemManager->Render();
		
		//UI
		ui->Render( gameManager->GetMode() );

		//　エフェクト描画
		m_Effect->Render();

		//	パーティクル描画
		particle->Render();
	}

	//	シャドウバッファ描画
	void	sceneMain::RenderShadowBuffer( void )
	{
		ShadowTex->RenderTarget();

		//	Zバッファ設定
		Surface*	orgZ;
		iexSystem::GetDevice()->GetDepthStencilSurface( &orgZ );
		iexSystem::GetDevice()->SetDepthStencilSurface( ShadowZ );

		//	ライト方向
		Vector3 dir( 0.5f, -1.0f, -0.5f );
		dir.Normalize();

		//	シャドウ作成
		Vector3	target( 0.0f, 0.0f, 0.0f );
		Vector3	pos = target - dir * 10;
		Vector3	up( 0.0f, 1.0f, 0.0f );

		//	視点とライト位置へ
		D3DXMATRIX		ShadowMat, work;
		LookAtLH( ShadowMat, pos, target, up );
		D3DXMatrixOrthoLH( &work, 100, 100, -100.0, 100.0f );	//	平行投影行列（範囲×100）
		ShadowMat *= work;

		shader3D->SetValue( "ShadowProjection", &ShadowMat );

		D3DVIEWPORT9	vp = { 0, 0, SHADOW_SIZE, SHADOW_SIZE, 0, 1.0f };
		iexSystem::GetDevice()->SetViewport( &vp );

		//	レンダリング
		iexSystem::GetDevice()->Clear( 0, NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0 );

		//	描画
		//m_Stage->Render( shader3D, "shadowBuf" );
		characterManager->Render( shader3D, "ShadowBuf" );
		m_CoinManager->Render( shader3D, "ShadowBuf" );
		itemManager->Render( shader3D, "ShadowBuf" );

		//	作ったシャドウテクスチャをシェーダーにセット
		shader3D->SetValue( "ShadowMap", ShadowTex );

		//	レンダーターゲットの復元
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		iexSystem::GetDevice()->SetDepthStencilSurface( orgZ );
	}
