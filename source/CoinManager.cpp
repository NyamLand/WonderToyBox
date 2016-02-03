
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
#define	COIN_SIZE	3.0f
#define	COIN_BAG_SIZE 0.05f

//---------------------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//	初期化・解放
//---------------------------------------------------------------------------------

	//	初期化
	bool	CoinManager::Initialize( void )
	{
		//	元モデル読み込み
		FOR( 0, Coin::MAX )	org[value] = nullptr;
		org[Coin::COIN] = new iexMesh( "DATA/Object/Coin/Coin/coin01.imo" );
		org[Coin::COIN_BAG_5] = new iexMesh( "DATA/Object/Coin/CoinBag/CoinBag5.IMO" );
		org[Coin::COIN_BAG_10] = new iexMesh( "DATA/Object/Coin/CoinBag/CoinBag10.IMO" );

		//	リスト初期化
		coinList.clear();

		if ( org != nullptr ) 	return	false;
		return	true;
	}

	//	解放
	void	CoinManager::Release( void )
	{
		for ( auto it = coinList.begin(); it != coinList.end(); )
		{
			it = coinList.erase( it );
		}

		FOR( 0, Coin::MAX ) 
		{
			SafeDelete( org[value] );
		}
	}

//---------------------------------------------------------------------------------
//	更新・描画
//---------------------------------------------------------------------------------

	//	更新
	void	CoinManager::Update( void )
	{
		//	枚数カウント初期化
		coin_num = 0;

		//	リスト内コイン更新
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			//	枚数カウント
			coin_num++;

			//	情報更新
			( *it )->Update();

			//	消滅していたらリストから削除
			bool state = ( *it )->GetState();
			if ( !state )
			{
				it = coinList.erase( it );
				break;
			}
		}
	}

	//	描画
	void	CoinManager::Render( iexShader* shader, LPSTR technique )
	{
		//	コイン描画
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			if ( shader == nullptr || technique == nullptr )
			{
				( *it )->Render();
			}
			else
			{
				( *it )->Render( shader, technique );
			}
		}
	}

//---------------------------------------------------------------------------------
//	動作関数
//---------------------------------------------------------------------------------

	//	生成
	void	CoinManager::Append( const Vector3& pos, const Vector3& vec, const float& speed, int type )
	{
		Coin*	coin = new Coin();
		//	各種情報設定
		coin->Initialize();
		coin->judgeTimer = 30;
		coin->activate = false;
		coin->type = type;
		coin->obj = org[type]->Clone();
		Vector3	v = vec;
		//v.Normalize();
		Vector3	moveParam = v * speed;
		coin->SetPos( pos );
		coin->SetMove( moveParam );
		coin->SetState( true );

		switch ( type )
		{
		case Coin::COIN:
			coin->SetScale( COIN_SIZE );
			break;

		case Coin::COIN_BAG_5:
		case Coin::COIN_BAG_10:
			coin->SetScale( COIN_BAG_SIZE );
			break;
		}
		coin->Update();

		//	リストに追加
		coinList.push_back( coin );
	}

//---------------------------------------------------------------------------------
//	情報設定
//---------------------------------------------------------------------------------

	//	コインのモード設定
	void	CoinManager::SetCoinGetAwayFlag( bool flag )
	{
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			( *it )->SetGetAwayFlag( flag );
		}
	}

	//	状態取得
	void	CoinManager::SetState( bool state )
	{
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			( *it )->SetState( state );
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
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			if ( !( *it )->GetState() )	continue;

			//	コインの座標を取得
			Vector3 coin_pos = ( *it )->GetPos();

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
	
	//　近くにあるコインの数を返す
	int		CoinManager::GetNearCoinNum( float dist, const Vector3& pos )
	{
		int	coinNum = 0;

		//	コイン全部をチェック
		FOR_LIST( coinList.begin(), coinList.end() )
		{
			if ( !( *it )->GetState() )	continue;

			//	コインの座標を取得
			Vector3 coin_pos = ( *it )->GetPos();

			//	与えられた座標との距離を算出
			float	length = ( coin_pos - pos ).Length();

			//	距離が最小値より短かったら情報を交換
			if ( length < dist )
			{
				coinNum++;
			}
		}

		return coinNum;
	}

	//	コインリスト取得
	list<Coin*>	CoinManager::GetList( void )const
	{
		return	coinList;
	}

	//	実体取得
	CoinManager*	CoinManager::GetInstance( void )
	{
		static	CoinManager	out;
		return	&out;
	}