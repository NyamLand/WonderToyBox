
#ifndef __ITEMMANAGER_H__
#define	__ITEMMANAGER_H__

//*********************************************************************************
//
//	ItemManager�N���X
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
	//	�萔
	static const int ITEM_MAX = 10;		//	�A�C�e���ő吔
	list<Item*>	itemList;

public:
	//	�������E���
	ItemManager( void );
	~ItemManager( void );
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	Append( const Vector3& pos );
};

//*********************************************************************************
#endif // !__ITEMMANAGER_H__
