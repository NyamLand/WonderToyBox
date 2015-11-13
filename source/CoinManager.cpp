
#include	"iextreme.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GlobalFunction.h"

#include	"CoinManager.h"

//******************************************************************************
//
//	CoinManagerクラス
//
//******************************************************************************

//	定数
#define	COIN_SIZE	2.0f

//---------------------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------------------

	//	実体
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
		//	モデル読み込み
		org = nullptr;
		org = make_unique<iexMesh>( LPSTR( "DATA/Object/coin01.imo" ) );

		//	コイン初期化
		c_Coin = new Coin[ COIN_MAX ];
		coin_num = 0;
		FOR( 0, COIN_MAX )
		{
			c_Coin[value].Initialize();
			c_Coin[value].state = false;
		}

		if ( org != nullptr ) 	return	false;
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

		//	全コイン更新
		FOR( 0, COIN_MAX )
		{
			if ( !c_Coin[value].state )	continue;
			
			//	枚数カウント
			coin_num++;
			
			//	位置調整
			DistCheck( value );

			//	情報更新
			c_Coin[value].Update();
		}
	}

	//	描画
	void	CoinManager::Render( iexShader* shader, LPSTR technique )
	{
		//	存在している全コイン描画
		FOR( 0, COIN_MAX )
		{
			//	存在してなかったらスキップ
			if ( !c_Coin[value].state )	continue;

			//	コイン描画
			if ( shader == nullptr || technique == nullptr )
			{
				c_Coin[value].Render();
			}
			else
			{
				c_Coin[value].Render( shader, technique );
			}
		}
	}

//---------------------------------------------------------------------------------
//	動作関数
//---------------------------------------------------------------------------------

	//	生成
	void	CoinManager::Set( const Vector3& pos, const Vector3& vec, const float& speed )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			if ( c_Coin[i].state )	continue;

			c_Coin[i].judgeTimer		=		30;
			c_Coin[i].activate			=		false;
			c_Coin[i].obj					=		org->Clone();
			c_Coin[i].pos					=		pos;
			Vector3	v						=		vec;
			v.Normalize();
			c_Coin[i].move				=		v * speed;
			c_Coin[i].scale				=		COIN_SIZE;
			c_Coin[i].state				=		true;
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

//---------------------------------------------------------------------------------
//	情報設定
//---------------------------------------------------------------------------------

	//	コインのモード設定
	void	CoinManager::SetCoinGetAwayFlag( bool flag )
	{
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			c_Coin[i].SetGetAwayFlag( flag );
		}
	}


	void	CoinManager::SetState(bool state)
	{
		for (int i = 0; i < COIN_MAX; i++)
		{
			c_Coin[i].SetState(state);
		}
	}
//---------------------------------------------------------------------------------
//	情報取得
//---------------------------------------------------------------------------------

	//	与えられた座標に一番近いコインの座標を返す
	bool	CoinManager::GetMinPos( Vector3& out, const Vector3& pos )
	{
		//	適当にでかい数値を用意
		float		dist = 1000.0f;

		//	コイン全部をチェック
		for ( int i = 0; i < COIN_MAX; i++ )
		{
			//	存在チェック
			if ( !c_Coin[i].state )	continue;

			//	コインの座標を取得
			Vector3 coin_pos = c_Coin[i].GetPos();

			//	与えられた座標との距離を算出
			float	length = ( coin_pos - pos ).Length();

			//	距離が最小値より短かったら情報を交換
			if ( length < dist )
			{
				dist = length;
				out = coin_pos;
			}
		}

		//	値が変わってなかったらfalseを返す
		if ( dist >= 1000.0f )	return	false;
		return	true;
	}

	//	フィールドに存在するコインの枚数を取得
	int		CoinManager::GetFreeCoinNum( void )const
	{
		return	coin_num;
	}

	Coin*	CoinManager::GetCoin( void )const
	{
		return c_Coin;
	}