
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
		hdr = new iex2DObj( 512, 512, IEX2D_RENDERTARGET );

		//	シャドウマップ
		ShadowTex = new iex2DObj( SHADOW_SIZE, SHADOW_SIZE, IEX2D_RENDERTARGET );
		iexSystem::GetDevice()->CreateDepthStencilSurface( SHADOW_SIZE, SHADOW_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE,	0, FALSE, &ShadowZ, NULL );

		//	反射マップ
		RefTex = new iex2DObj( 1280, 720, IEX2D_RENDERTARGET );

		//	ライト設定
		Vector3 dir( 1.0f, -1.0f, -0.5f );
		dir.Normalize();
		iexLight::DirLight( shader3D, 0, &dir, 1.1f, 1.1f, 1.1f );

		//	カメラ設定
		m_Camera = new Camera();

		//	変数初期化
		timer = 0;
		playerNum = gameManager->GetPlayerNum();
		stageType = gameManager->GetStageType();

		//	ステージ
		m_CollisionStage = new iexMesh( "DATA/back/Collision.IMO" );
		m_Stage = new iexMesh( "DATA/back/stage.IMO" );
		m_Stage->SetAngle( D3DX_PI );
		m_Stage->Update();
		
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
		ui->Initialize( UI_MODE::MAIN );

		//	BGM再生
		sound->PlayBGM( BGM::MAIN_BGM );

		//	全体更新
		Update();
		return true;
	}

	//	デストラクタ
	sceneMain::~sceneMain( void )
	{
		backBuffer->Release();
		SafeDelete( ShadowTex );
		SafeDelete( RefTex );
		SafeDelete( m_Camera );
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

	//	プレイヤー初期化
	void    sceneMain::PlayerInitialize(void)
	{
		//　プレイヤー設定
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

	//	ディファード初期化
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
		//	particle->Aura(Vector3(0, 10.0f, 0), 3, 0.5f);
			//particle->Arrow(Vector3(0, 15.0f, 0), 6, 0.5f, 100);


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
		//	アイテム更新
		itemManager->Update();

		//	player
		characterManager->Update();

		//	パーティクル更新
		particle->Update();

		//	イベント更新
		eventManager->Update();

		//	コイン更新
		m_CoinManager->Update();

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
		m_Camera->Activate();
		m_Camera->Clear();

		//	影
		RenderShadowBuffer();

		//	オブジェクト描画
		if ( characterManager->GetParameterState( 0, PARAMETER_STATE::SLIP ) )
		{
			m_Stage->Render( shader3D, "full_sr" );
		}
		else
		{
			m_Stage->Render( shader3D, "full_s" );
		}
		characterManager->Render( shader3D, "toon" );
		m_CoinManager->Render();
		m_BulletManager->Render();
		itemManager->Render();
		
		//	パーティクル描画
		particle->Render();

		//　エフェクト描画
		m_Effect->Render();

		//UI
		ui->Render( gameManager->GetMode() );
	}

	//	HDR描画
	void	sceneMain::RenderHDR( void )
	{
		//	HDR用バッファへ切り替え
		hdr->RenderTarget();
		m_Camera->SetViewport( 0, 0, 512, 512 );
		m_Camera->SetProjection( 0.8f, 0.1f, 300.0f, 1280.0f / 720.0f );
		m_Camera->Activate();
		m_Camera->Clear();

		//	物体の描画
		m_Stage->Render( shader3D, "specular" );
		m_CoinManager->Render( shader3D, "specular" );
		characterManager->Render( shader3D, "specular" );
		itemManager->Render( shader3D, "specular" );

		//	ぼかし
		DWORD	color = ( ( 8 * 6 ) << 24 ) | 0xFFFFFF;
		for ( int i = 0; i < 24; i += 4 )
		{
			hdr->Render( -i, 0, 512, 512, 0, 0, 512, 512, RS_ADD, color );
			hdr->Render( i, 0, 512, 512, 0, 0, 512, 512, RS_ADD, color );
			hdr->Render( 0, -i, 512, 512, 0, 0, 512, 512, RS_ADD, color );
			hdr->Render( 0, i, 512, 512, 0, 0, 512, 512, RS_ADD, color );
			color -= 8 << 24;
		}

		//	バックバッファへ切り替え
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
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

	//	反射
	void	sceneMain::RenderRef( void )
	{
		RefTex->RenderTarget();
		m_Camera->SetViewport();

		m_Camera->Activate();
		m_Camera->Clear();

		//	物体描画
		m_Stage->Render( shader3D, "Refrect" );
		characterManager->Render( shader3D, "Refrect" );
		shader3D->SetValue( "RefMap", RefTex );

		//	レンダーターゲットの復元
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
	}
