
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
//	グローバル変数
//
//*****************************************************************************************************************************
//	TAKATORI

//*****************************************************************************************************************************
//
//	初期化
//
//*****************************************************************************************************************************

	//	コンストラクタ
	sceneMain::sceneMain( void ) : m_Stage( NULL )
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
		m_CollisionStage = new iexMesh( "DATA/BG/CollisionGround.IMO" );
		m_Stage = new iexMesh( "DATA/BG/2_1/FIELD2_1.IMO" );
		
		//	当たり判定
		Collision::Initiallize( m_CollisionStage );

		//	プレイヤー
		m_Player = new PlayerManager();
		PlayerInitialize();
		
		//	コイン
		m_CoinManager = new CoinManager();
		m_CoinManager->Initialize();

		//	パーティクル
		particle = new iexParticle();
		particle->Initialize( "DATA/Particle.png", 10000 );

		//	全体更新
		Update();
		return true;
	}

	//	解放
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

	//	プレイヤー初期化
	void	sceneMain::PlayerInitialize( void )
	{
		m_Player->Initialize( 0, PlayerData::Y2009, Vector3( 0.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 1, PlayerData::ECCMAN, Vector3( 10.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 2, PlayerData::Y2009, Vector3( 5.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 3, PlayerData::Y2009, Vector3( -5.0f, 0.0f, 0.0f ) );
	}

//*****************************************************************************************************************************
//
//		更新
//
//*****************************************************************************************************************************

	//	更新
	void	sceneMain::Update( void )
	{
		//	プレイヤー更新
		m_Player->Update();
		
		//	点光源設定
		shader3D->SetValue( "plight_pos", Vector3( 0.0f, 0.0f, 0.0f ) );
		shader3D->SetValue("plight_range", 6.0f );
		shader3D->SetValue("plight_color", Vector3( 2.0f, 0.0f, 2.0f ) );
		
		//	パーティクル更新
		particle->Update();

		//	コイン更新
		m_CoinManager->Update();

		//	カメラ更新
		m_Camera->Update( VIEW_MODE::FIX, Vector3( 0.0f, 2.0f, 0.0f ) );

		shader3D->SetValue( "ViewPos", m_Camera->GetPos() );
		shader3D->SetValue( "matView", m_Camera->GetMatrix() );
	}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

	//	描画
	void	sceneMain::Render( void )
	{
		//	影
		RenderShadowBuffer();

		//	画面クリア
		m_Camera->Activate();
		m_Camera->Clear();

		//	オブジェクト描画
		m_Stage->Render( shader3D, "full_s" );
		m_Player->Render( shader3D, "full" );
		m_CoinManager->Render();

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
		m_Player->Render( shader3D, "ShadowBuf" );
		//m_CoinManager->Render( shader3D, "ShadowBuf" );

		//	作ったシャドウテクスチャをシェーダーにセット
		shader3D->SetValue( "ShadowMap", ShadowTex );

		//	レンダーターゲットの復元
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		iexSystem::GetDevice()->SetDepthStencilSurface( orgZ );


	}


