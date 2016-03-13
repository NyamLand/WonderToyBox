
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

class Item // :public Coin
{
private:
	struct SHADOW_INFO
	{
		iex2DObj*	obj;
		Vector3		pos;
		LVERTEX		v[4];
		float				scale;
	}; 

private:
	iexMesh*	obj;
	Vector3	pos;
	Vector3	move;
	float			scale;
	float			angle;
	float			stageHeight;
	bool			state;
	int		type;
	int		time;

	//	�e�\����
	SHADOW_INFO	shadow;

public:
	//	�������E���
	Item( void );
	~Item( void );
	bool	Initialize( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );
	void	ObjectUpdate( void );
	void	ShadowUpdate( void );

	//	����֐�
	void	Move( void );
	virtual	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum );	//�q�b�g������
	void	StageCollisionCheck( void );
	void	PlayerCollisionCheck( void );
	
	//	���ݒ�
	void	SetPos( const Vector3& pos );
	void	SetMove( const Vector3& move );
	void	SetAngle( const float& angle );
	void	SetScale( const float& scale );
	void	SetMesh( iexMesh* obj );
	void	SetState( bool state );
	void	SetType( const int& type );

	//	���擾
	int	GetType( void )const;
	bool	GetState( void )const;
	Vector3	GetPos( void )const;
	float	GetAngle( void )const;
};

//*****************************************************************************
#endif // !__ITEM_H__
