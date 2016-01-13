
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"ItemManager.h"

//**********************************************************************************
//
//	ItemManagerクラス
//
//**********************************************************************************

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------

	//	初期化
	bool	ItemManager::Initialize( void )
	{
		//	オリジナル初期化
		FOR( 0, ITEM_TYPE::MAX )
		{
			org[value] = nullptr;
		}
		org[ITEM_TYPE::ATTACK_UP] = new iexMesh( "DATA/Object/Item/Item.IMO" );
		org[ITEM_TYPE::MAGNET] = new iexMesh( "DATA/Object/Item/Item04.IMO" );
		org[ITEM_TYPE::SPEED_UP] = new iexMesh( "DATA/Object/Item/Item03.IMO" );
		org[ITEM_TYPE::UNRIVALED] = new iexMesh();
		itemList.clear();

		return	true;
	}

	//	解放
	void	ItemManager::Release( void )
	{
		FOR( 0, ITEM_TYPE::MAX )
		{
			SafeDelete( org[value] );
		}
		
		for ( auto it = itemList.begin(); it != itemList.end(); )
		{
			it = itemList.erase( it );
		}
	}

//------------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------------

	//	更新
	void	ItemManager::Update( void )
	{
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			( *it )->Update();

			bool	state = ( *it )->GetState();
			
			if ( !state )
			{
				it = itemList.erase( it );
				break;
			}
		}
	}

	//	描画
	void	ItemManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			if (shader == nullptr || technique == nullptr)
			{
				( *it )->Render();
			}
			else
			{
				( *it )->Render( shader, technique );
			}
		}
	}

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

	//	リスト追加
	void	ItemManager::Append( const Vector3& pos, const int& type )
	{
		Item*	item = nullptr;
		item = new Item();
		item->Initialize();
		item->SetMesh( org[type]->Clone() );
		item->SetType( type );
		item->SetPos( pos );
		item->Update();
		item->SetState( true );
		itemList.push_back( item );
	}

//------------------------------------------------------------------------------------
//	情報取得
//------------------------------------------------------------------------------------

	//	実体取得
	ItemManager*	ItemManager::GetInstance( void )
	{
		static	ItemManager	out;
		return	&out;
	}

	//	リスト取得
	std::list<Item*>	ItemManager::GetList( void )
	{
		return	itemList;
	}

	//	与えた座標に一番近い座標を取得
	bool	ItemManager::GetMinPos( Vector3& out, const Vector3& pos )
	{
		//	適当に数値を用意
		float		dist = 1000.0f;

		//	リストの末端まで検索
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			//	アイテムの座標を取得
			Vector3 i_pos = ( *it )->GetPos();

			//	受け取った座標とアイテムの座標の距離を算出
			float	length = ( i_pos - pos ).Length();

			//	距離が最小値より小さかったら情報を上書き
			if ( length < dist )
			{
				dist = length;
				out = i_pos;
			}
		}

		//	値が変わってなかったらfalseを返す
		if ( dist >= 1000.0f )	return	false;
		else	return	true;
	}
