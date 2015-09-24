
#ifndef __ITEM_H__
#define	__ITEM_H__

//*****************************************************************************
//
//	Itemクラス
//
//*****************************************************************************

//インクルード
#include	"Coin.h"

class Item  :public Coin
{
private:
	

public:
	//	初期化・解放
	Item( void );
	~Item( void );
	
	//	動作関数
	void	Move();
	void	Hitduringtheoperation(const Vector3& pos, const int& Num);	//ヒット時動作
	
	//	情報取得・設定
	void	SetMesh( iexMesh* obj );
	void	SetState( bool state );
};

//*****************************************************************************
#endif // !__ITEM_H__
