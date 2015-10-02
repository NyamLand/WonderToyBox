
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
	iexMesh*	org;

public:
	//	�������E���
	ItemManager( void );
	~ItemManager( void );
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	//	�V�F�[�_�[�t���`��
	void	Render(iexShader* shader, LPSTR technique);

	//	����֐�
	void	Append( const Vector3& pos );

	//	���擾
	static	ItemManager*	GetInstance();
};

#define	itemManager	( ItemManager::GetInstance() )

//*********************************************************************************
#endif // !__ITEMMANAGER_H__