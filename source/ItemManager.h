
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
using namespace std;

class ItemManager
{
private:
	list<Item*>	itemList;

public:
	//	�������E���
	ItemManager( void );
	~ItemManager( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	Append( const Vector3& pos );
};

//*********************************************************************************
#endif // !__ITEMMANAGER_H__
