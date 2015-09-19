
#include	"iextreme.h"
#include	<random>
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Image.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"Item.h"
#include	"ItemManager.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"Bullet.h"
#include	"BulletManager.h"
#include	"sceneResult.h"
#include	"UI.h"

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

		//	ステージ
		m_CollisionStage = new iexMesh( "DATA/BG/desk_Collision.IMO" );
		m_Stage = new iexMesh( "DATA/back/stage.IMO" );
		//	当たり判定
		Collision::Initiallize( m_CollisionStage );

		//	プレイヤー
		m_Player = new PlayerManager();
		PlayerInitialize();
		
		//	コイン
		m_CoinManager = new CoinManager();
		m_CoinManager->Initialize();

		//	バレット
		m_BulletManager = new BulletManager();
		m_BulletManager->Initialize();

		//	アイテム
		//itemManager->Initialize();
			
		//	パーティクル
		Particle::Initialize();

		//UI
		ui = new UI();
		ui->Initialize();

		//	ディファード関係初期化
		DifferedInitialize();

		//	変数初期化
		timer = 0;
		playerNum = GameManager::GetPlayerNum();
		stageType = GameManager::GetStageType();

		PlayBGM( SoundInfo::BGM::MAIN_BGM );

		//	全体更新
		Update();
		return true;
	}

	//	デストラクタ
	sceneMain::~sceneMain( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_CollisionStage );
		SafeDelete( m_Camera );
		SafeDelete( m_CoinManager );
		SafeDelete( m_BulletManager );
		SafeDelete( ShadowTex );
		SafeDelete( diffuse );
		SafeDelete( specular );
		SafeDelete( depth );
		SafeDelete( normal );
		SafeDelete( light );
		SafeDelete( light_s );
		SafeDelete( screen );
		SafeDelete( m_Player );
		SafeDelete( ui );
		backBuffer->Release();
		Particle::Release();
		StopBGM();
	}

	//	プレイヤー初期化
	void	sceneMain::PlayerInitialize( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			int		characterType = GameManager::GetCharacterType( i );
			Vector3	pos = Vector3( -20.0f + ( 10.0f * i ), 10.0f, 0.0f );
			m_Player->Initialize( i, characterType, pos );
		}
	}

	//	ディファード初期化
	void	sceneMain::DifferedInitialize( void )
	{
		//	一時レンダリング用サーフェイス
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
//		更新
//
//*****************************************************************************************************************************

	//	更新
	void	sceneMain::Update( void )
	{
		//	カメラ更新
		m_Camera->SetPlayerInfo( m_Player->GetPos( 0 ), m_Player->GetPos( 1 ), m_Player->GetPos( 2 ), m_Player->GetPos( 3 ) );
		m_Camera->Update( VIEW_MODE::CHASE, Vector3( 0.0f, 2.0f, 0.0f ) );

		//	UI
		ui->Update( GameManager::GetMode() );

		//	デバッグモード切り替え
		if ( KEY( KEY_ENTER ) == 3 )		debug = !debug;

		switch ( GameManager::GetMode() )
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

			if ( ui->GetChangeFlag() )
			{
				MainFrame->ChangeScene( new sceneResult() );
				return;
			}
			break;
		}

	}

	//	スタート更新
	void	sceneMain::StartUpdate( void )
	{
		//	全体更新
		AllUpdate();

		if ( ui->GetChangeFlag() ) 
		{
			GameManager::SetMode( GAME_MODE::MAINGAME );
			for ( int i = 0; i < 4; i++ )		m_Player->SetMode( i, PlayerData::MOVE );
			ui->SetChangeFlag( false );
		}
	}

	//	メイン更新
	void	sceneMain::MainGameUpdate( void )
	{			
		//	ゲームマネージャー
		GameManager::Update();

		//	全体更新
		AllUpdate();
	}

	//	どんけつ更新
	void	sceneMain::DonketsuUpdate( void )
	{
		if ( ui->GetChangeFlag() )
		{
			GameManager::SetMode( GAME_MODE::CLIMAX );
			ui->SetChangeFlag( false );
		}
	}

	//	クライマックス更新
	void	sceneMain::ClimaxUpdate( void )
	{
		//	ゲームマネージャー
		GameManager::Update();

		//	全体更新
		AllUpdate();
	}

	//	タイムアップ更新
	void	sceneMain::FinishUpdate( void )
	{
		//	ゲームマネージャー
		GameManager::Update();

		//	全体更新
		AllUpdate();
	}

	//	全体更新
	void	sceneMain::AllUpdate( void )
	{
		//	player
		m_Player->Update();

		//	パーティクル更新
		Particle::Update();

		//	コイン更新
		m_CoinManager->Update();

		//	リス　バレット更新
		m_BulletManager->Update();

		//	アイテム更新
		//itemManager->Update();
	}

//*****************************************************************************************************************************
//
//		動作関数
//
//*****************************************************************************************************************************

	//	点光源
	void	sceneMain::PointLight( const Vector3& pos, const Vector3& color, float range )
	{
		//	変換行列設定
		Matrix	InvProj;
		D3DXMatrixInverse( &InvProj, NULL, &matProjection );
		shaderD->SetValue( "InvProj", InvProj );

		Matrix	mat = matView;

		//	カメラ空間変換
		Vector3	LightPos;
		LightPos.x = pos.x * mat._11 + pos.y * mat._21 + pos.z * mat._31 + mat._41;
		LightPos.y = pos.x * mat._12 + pos.y * mat._22 + pos.z * mat._32 + mat._42;
		LightPos.z = pos.x * mat._13 + pos.y * mat._23 + pos.z * mat._33 + mat._43;

		//	シェーダー設定
		shaderD->SetValue( "pLightPos", LightPos );
		shaderD->SetValue( "pLightColor", ( Vector3 )color );
		shaderD->SetValue( "pLightRange", range );

		//	レンダリング
		normal->Render( shaderD, "pointlight" );
	}

	//	平行光
	void	sceneMain::DirLight( const Vector3& dir, const Vector3& color )
	{
		Matrix	mat = matView;
		Vector3	LightDir;

		//	カメラ空間変換
		LightDir.x = dir.x * mat._11 + dir.y * mat._21 + dir.z * mat._31;
		LightDir.y = dir.x * mat._12 + dir.y * mat._22 + dir.z * mat._32;
		LightDir.z = dir.x * mat._13 + dir.y * mat._23 + dir.z * mat._33;
		LightDir.Normalize();

		//	シェーダー設定
		shaderD->SetValue( "LightVec", LightDir );
		shaderD->SetValue( "LightColor", ( Vector3 )color );

		//	レンダリング
		normal->Render( shaderD, "dirlight2" );
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
		m_Player->Render( shader3D, "toon" );
		m_CoinManager->Render();
		m_BulletManager->Render();
		//itemManager->Render();
		
		//UI
		ui->Render( GameManager::GetMode() );

		//	パーティクル描画
		Particle::Render();
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
		m_Player->Render( shader3D, "ShadowBuf" );
		m_CoinManager->Render( shader3D, "ShadowBuf" );

		//	作ったシャドウテクスチャをシェーダーにセット
		shader3D->SetValue( "ShadowMap", ShadowTex );

		//	レンダーターゲットの復元
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		iexSystem::GetDevice()->SetDepthStencilSurface( orgZ );
	}

	//	ディファード描画
	void	sceneMain::RenderDiffered( void )
	{
		//	ディファード設定
		diffuse->RenderTarget();
		specular->RenderTarget( 1 );
		depth->RenderTarget( 2 );
		normal->RenderTarget( 3 );

		//	ビュー設定
		m_Camera->Activate();
		m_Camera->Clear();
		shaderD->SetValue( "matView", matView );

		//	変換行列設定
		Matrix	InvProj;
		D3DXMatrixInverse( &InvProj, NULL, &matProjection );
		shaderD->SetValue( "InvProj", InvProj );

		//	物体描画
		m_Stage->Render( shaderD, "differed" );
		m_Player->Render( shaderD, "differed" );
		m_CoinManager->Render( shaderD, "differed" );

		//	ライトバッファ作成
		light->RenderTarget();
		light_s->RenderTarget( 1 );
		iexSystem::Device->SetRenderTarget( 2, NULL );
		iexSystem::Device->SetRenderTarget( 3, NULL );
		m_Camera->Clear( 0x000000 );

		//	平行光
		//DirLight( Vector3( 1.0f, -1.0f, -0.5f ), Vector3( 0.1f, 0.1f, 0.1f ));

		//	点光源
		PointLight( Vector3( 0.0f, 3.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ), 5.0f );

		//	一時バッファへ切り替え
		screen->RenderTarget();
		iexSystem::Device->SetRenderTarget( 1, NULL );
		iexSystem::Device->SetRenderTarget( 2, NULL );
		iexSystem::Device->SetRenderTarget( 3, NULL );
		m_Camera->Clear();

	}

	
