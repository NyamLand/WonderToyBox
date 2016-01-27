
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
	//	�萔
	static const int ITEM_MAX = 10;		//	�A�C�e���ő吔
	std::list<Item*>	itemList;
	iexMesh*	org[ITEM_TYPE::MAX];

public:
	//	�������E���
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	void	Append( const Vector3& pos, const int& type );
	std::list<Item*>	GetList( void );

	//	���擾
	bool	GetMinPos( Vector3& out, const Vector3& pos );
	static	ItemManager*	GetInstance();
};

#define	itemManager	( ItemManager::GetInstance() )

//*********************************************************************************
#endif // !__ITEMMANAGER_H__
