
#ifndef __ITEM_H__
#define	__ITEM_H__

//*****************************************************************************
//
//	Item�N���X
//
//*****************************************************************************

//�C���N���[�h
#include	"Coin.h"

//	���
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
	//	�������E���
	Item( void );
	~Item( void );
	bool	Initialize( void )override;
	
	//	����֐�
	void	Move( void )override;
	void	Hitduringtheoperation( const Vector3& pos, const int& Num )override;	//�q�b�g������
	
	//	���ݒ�
	void	SetMesh( iexMesh* obj );
	void	SetState( bool state );
	void	SetType( const int& type );

	//	���擾
	int	GetType( void );
	bool	GetState( void );

};

//*****************************************************************************
#endif // !__ITEM_H__
