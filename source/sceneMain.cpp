
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

		//	ライト設定
		Vector3 dir( 1.0f, -1.0f, -0.5f );
		dir.Normalize();
		iexLight::DirLight( shader, 0, &dir, 0.8f, 0.8f, 0.8f );

		//	カメラ設定
		m_Camera = new Camera();

		//	ステージ
		m_Stage = new iexMesh( "DATA/BG/CollisionGround.IMO" );
		
		//	当たり判定
		Collision::Initiallize( m_Stage );

		//	プレイヤー
		m_Player = new PlayerManager();
		PlayerInitialize();
		
		//	コイン
		m_CoinManager = new CoinManager();
		m_CoinManager->Initialize();

		//	パーティクル
		particle = new iexParticle();
		particle->Initialize( "DATA/Particle.png", 10000 );

		//	ブロック描画
		m_Block = new Block();
		m_Block->Initialize( 0, 0, Vector3( 0.0f, 0.0f, 0.0f ), 0.02f );

		//	全体更新
		Update();
		return true;
	}

	//	解放
	sceneMain::~sceneMain( void )
	{
		SafeDelete( m_Stage );
		SafeDelete( m_Player );
		SafeDelete( m_Camera );
		SafeDelete( particle );
		SafeDelete( m_CoinManager );
		SafeDelete( m_Block );
	}

	//	プレイヤー初期化
	void	sceneMain::PlayerInitialize( void )
	{
		m_Player->Initialize( 0, 0, Vector3( 0.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 1, 0, Vector3( 10.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 2, 0, Vector3( 5.0f, 0.0f, 0.0f ) );
		m_Player->Initialize( 3, 0, Vector3( -5.0f, 0.0f, 0.0f ) );
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
		
		//	パーティクル更新
		particle->Update();

		//	コイン更新
		m_CoinManager->Update();

		//	ブロック更新
		m_Block->Update();
	}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

	//	描画
	void	sceneMain::Render( void )
	{
		//	画面クリア
		m_Camera->Activate();
		m_Camera->Clear();

		//	オブジェクト描画
		m_Stage->Render();
		m_Player->Render();
		m_CoinManager->Render();
		//m_Block->Render();
		//	パーティクル描画
		particle->Render();


	}


