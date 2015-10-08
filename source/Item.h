
#ifndef __ITEM_H__
#define	__ITEM_H__

//*****************************************************************************
//
//	Item�N���X
//
//*****************************************************************************

//�C���N���[�h
#include	"Coin.h"

class Item  :public Coin
{
private:
	int		type;

public:
	//	�������E���
	Item( void );
	~Item( void );
	
	//	����֐�
	void	Move();
	void	Hitduringtheoperation(const Vector3& pos, const int& Num);	//�q�b�g������
	
	//	���ݒ�
	void	SetMesh( iexMesh* obj );
	void	SetState( bool state );

	//	���擾
	int		GetType( void );
	bool	GetState( void );
};

//*****************************************************************************
#endif // !__ITEM_H__
