
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

using namespace std;

class ItemManager
{
private:
	//	定数
	static const int ITEM_MAX = 10;		//	アイテム最大数
	std::list<Item*>	itemList;
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
	std::list<Item*>	GetList( void );
	//	情報取得
	bool	GetMinPos( Vector3& out, const Vector3& pos );
	static	ItemManager*	GetInstance();
};

#define	itemManager	( ItemManager::GetInstance() )

//*********************************************************************************
#endif // !__ITEMMANAGER_H__
