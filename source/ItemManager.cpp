
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

	//	コンストラクタ
	ItemManager::ItemManager( void )
	{
	
	}

	//	デストラクタ
	ItemManager::~ItemManager( void )
	{

	}

	//	初期化
	bool	ItemManager::Initialize( void )
	{
		org = new iexMesh( "DATA/Object/Item.IMO" );
		itemList.clear();

		for ( int i = 0; i < ITEM_MAX; i++ )
		{
			Append( Vector3( Random::GetFloat( -10.0f, 10.0f ), Random::GetFloat( 0.0f, 50.0f ), Random::GetFloat( -10.0f, 10.0f ) ) );
		}

		return	true;
	}

	//	解放
	void	ItemManager::Release( void )
	{
		SafeDelete( org );
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
	void	ItemManager::Render( void )
	{
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			( *it )->Render();
		}
	}

	//	シェーダー付き描画
	void	ItemManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( auto it = itemList.begin(); it != itemList.end(); it++ )
		{
			(*it)->Render( shader, technique );
		}
	}

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

	//	リスト追加
	void	ItemManager::Append( const Vector3& pos )
	{
		Item*	item = nullptr;
		item = new Item();
		item->Initialize();
		item->SetMesh( org->Clone() );
		item->SetPos( pos );
		item->Update();
		item->SetState( true );
		itemList.push_back( item );
	}

//------------------------------------------------------------------------------------
//	情報取得
//------------------------------------------------------------------------------------

	//	実体取得
	ItemManager*	ItemManager::GetInstance()
	{
		static	ItemManager	out;
		return	&out;
	}

	//	リスト取得
	std::list<Item*>	ItemManager::GetList( void )
	{
		return	itemList;
	}

