
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
	int		type;

public:
	//	初期化・解放
	Item( void );
	~Item( void );
	
	//	動作関数
	void	Move();
	void	Hitduringtheoperation(const Vector3& pos, const int& Num);	//ヒット時動作
	
	//	情報設定
	void	SetMesh( iexMesh* obj );
	void	SetState( bool state );

	//	情報取得
	int		GetType( void );
	bool	GetState( void );
};

//*****************************************************************************
#endif // !__ITEM_H__
