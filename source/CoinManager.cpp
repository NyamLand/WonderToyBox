
#include	"iextreme.h"
#include	"system/System.h"
#include	<random>
#include	"GlobalFunction.h"
#include	"Coin.h"

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
		std::uniform_real_distribution<float> posrand( -10.0f, 10.0f );
		std::uniform_real_distribution<float> heightrand( 0.0f, 50.0f );
		std::uniform_real_distribution<float>	moverand( -0.1f, 0.1f );
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			Set( Vector3( posrand( ran ), heightrand( ran ), posrand( ran ) ), Vector3( 0.0f, 0.0f, 0.0f ), 1.0f );
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
			
			//	位置調整
			DistCheck( i );
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

		//	デバッグ用
		if ( !debug )	return;	

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

	//	位置調整
	void	CoinManager::DistCheck( int n )
	{
		//	コイン同士の位置チェック		
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			//	自分vs自分は除外		
			if ( i == n ) continue;

			//	自分→相手へのベクトル
			Vector3	coin_pos1 = c_Coin[n].GetPos();
			Vector3	coin_pos2 = c_Coin[i].GetPos();
			Vector3	vec = coin_pos2 - coin_pos1;
			

			//	距離計測			
			float length;
			length = vec.Length();

			//	近い場合は離す			
			if ( length < 0.5f )
			{
				//	ベクトル正規化		
				vec.Normalize();

				//	離す
				coin_pos1 =  coin_pos1 - vec * 0.5f;

				//	位置情報設定
				c_Coin[n].SetPos( coin_pos1 );
			}
			             
		}
	}