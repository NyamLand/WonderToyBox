
#ifndef __COIN_H__
#define	__COIN_H__

//*****************************************************************************
//
//	Coin�N���X
//
//*****************************************************************************
class Coin
{
private:
	const float GRAVITY = -0.01f;

private:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			activate;

public:
	//	�������E���
	Coin( void );
	~Coin( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	StageCollisionCheck( void );
	void	Move( void );

	//	���擾�E�ݒ�
	void	SetPos( Vector3 pos );
	void	SetAngle( float angle );
	void	SetScale( float angle );
	Vector3	GetPos( void );
	float		GetAngle( void );
};

//*****************************************************************************
#endif // !__COIN_H__
