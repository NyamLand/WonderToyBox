
#include	"iextreme.h"
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
		itemList.clear();

		return	true;
	}

	//	解放
	void	ItemManager::Release( void )
	{
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

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

	//	リスト追加
	void	ItemManager::Append( const Vector3& pos )
	{

	}

