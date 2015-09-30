
#ifndef __ITEMMANAGER_H__
#define	__ITEMMANAGER_H__

//*********************************************************************************
//
//	ItemManagerクラス
//
//*********************************************************************************

//	include
#include	<list>
#include	"Item.h"
#include	"CoinManager.h"
using namespace std;

class ItemManager		:	public CoinManager
{
private:
	//	定数
	static const int ITEM_MAX = 10;		//	アイテム最大数
	list<Item*>	itemList;
	iexMesh*	org;

public:
	//	初期化・解放
	ItemManager( void );
	~ItemManager( void );
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	//	シェーダー付き描画
	void	Render(iexShader* shader, LPSTR technique);

	//	動作関数
	void	Append( const Vector3& pos );

	//	情報取得
	static	ItemManager*	GetInstance();
};

#define	itemManager	( ItemManager::GetInstance() )

//*********************************************************************************
#endif // !__ITEMMANAGER_H__
