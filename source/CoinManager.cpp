
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Coin.h"
#include	<random>

#include	"CoinManager.h"

//******************************************************************************
//
//	CoinManagerクラス
//
//******************************************************************************

//---------------------------------------------------------------------------------
//	初期化・解放
//---------------------------------------------------------------------------------
CoinManager*	m_CoinManager;

//---------------------------------------------------------------------------------
//	初期化・解放
//---------------------------------------------------------------------------------

	//	コンストラクタ
	CoinManager::CoinManager( void )
	{
	
	}
	
	//	デストラクタ
	CoinManager::~CoinManager( void )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
			SafeDelete( c_Coin[i] );
	}

	//	初期化
	bool	CoinManager::Initialize( void )
	{
		std::uniform_real_distribution<float> posrand( -20.0f, 20.0f );
		std::uniform_real_distribution<float> heightrand( 0.0f, 50.0f );
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i] = new Coin();
			c_Coin[i]->Initialize();
			c_Coin[i]->SetPos( Vector3( posrand( ran ), heightrand( ran ), posrand( ran ) ) );
		}

		return	true;
	}

//---------------------------------------------------------------------------------
//	更新・描画
//---------------------------------------------------------------------------------

	//	更新
	void	CoinManager::Update( void )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i]->Update();
		}
	}

	//	描画
	void	CoinManager::Render( void )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i]->Render();
		}
	}

	//	シェーダー付き描画
	void	CoinManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i]->Render( shader, technique );
		}
	}