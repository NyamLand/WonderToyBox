
#include	"iextreme.h"
#include	"system/System.h"
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
//	グローバル変数
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

	}

	//	初期化
	bool	CoinManager::Initialize( void )
	{
		org = new iexMesh( "DATA/coin.imo" );
		c_Coin = new Coin[ COIN_MAX ];
		coin_num = 0;

		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i].Initialize();
			c_Coin[i].state = false;
		}

		//	コインをランダム生成
		std::uniform_real_distribution<float> posrand( -20.0f, 20.0f );
		std::uniform_real_distribution<float> heightrand( 0.0f, 50.0f );
		std::uniform_real_distribution<float>	moverand( -0.1f, 0.1f );
		for ( int i = 0; i < COIN_MAX / 2; i++ )
		{
			Set( Vector3( 0.0f, heightrand( ran ), 0.0f ), Vector3( moverand( ran ), moverand( ran ), moverand( ran ) ), 1.0f );
		}

		if ( org != NULL ) 	return	false;
		return	true;
	}

//---------------------------------------------------------------------------------
//	更新・描画
//---------------------------------------------------------------------------------

	//	更新
	void	CoinManager::Update( void )
	{
		//	枚数カウント初期化
		coin_num = 0;

		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( !c_Coin[i].state )	continue;
			coin_num++;
			c_Coin[i].Update();
		}
	}

	//	描画
	void	CoinManager::Render( void )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( !c_Coin[i].state )	continue;
			c_Coin[i].Render();
		}

		char	str[256];
		sprintf_s( str, "coin_num = %d", coin_num );
		DrawString( str, 20, 100 );
	}

	//	シェーダー付き描画
	void	CoinManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( !c_Coin[i].state )	continue;
			c_Coin[i].Render( shader, technique );
		}
	}

//---------------------------------------------------------------------------------
//	動作関数
//---------------------------------------------------------------------------------

	//	生成
	void	CoinManager::Set( const Vector3& pos, const Vector3& vec, float speed )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( c_Coin[i].state )	continue;

			c_Coin[i].judgeTimer		=		30;
			c_Coin[i].activate			=		false;
			c_Coin[i].state				=		true;
			c_Coin[i].obj					=		org->Clone();
			c_Coin[i].pos					=		pos;
			Vector3	v						=		vec;
			v.Normalize();
			c_Coin[i].move				=		v * speed;
			c_Coin[i].scale				=		0.5f;
			break;
		}
	}