
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

public:
	//	初期化・解放
	ItemManager( void );
	~ItemManager( void );
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	Append( const Vector3& pos );
};

//*********************************************************************************
#endif // !__ITEMMANAGER_H__
