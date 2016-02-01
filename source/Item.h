
#ifndef __ITEM_H__
#define	__ITEM_H__

//*****************************************************************************
//
//	Itemクラス
//
//*****************************************************************************

//インクルード
#include	"Coin.h"

//	情報
namespace
{
	namespace ITEM_TYPE
	{
		enum
		{
			ATTACK_UP,
			SPEED_UP,
			MAGNET,
			UNRIVALED,
			LIFE,
			MAX,
			BOMB,
			SEALED_ATTACK,
			SELFISHNESS_JUMP,
			OTHERS_SPEED_DOWN,
		};
	}
}

class Item  :public Coin
{
private:
	int		type;
	int		time;

public:
	//	初期化・解放
	Item( void );
	~Item( void );
	bool	Initialize( void )override;
	
	//	動作関数
	void	Move( void )override;
	void	Hitduringtheoperation( const Vector3& pos, const int& Num )override;	//ヒット時動作
	
	//	情報設定
	void	SetMesh( iexMesh* obj );
	void	SetState( bool state );
	void	SetType( const int& type );

	//	情報取得
	int	GetType( void );
	bool	GetState( void );

};

//*****************************************************************************
#endif // !__ITEM_H__
