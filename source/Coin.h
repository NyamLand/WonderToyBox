
#ifndef __COIN_H__
#define	__COIN_H__

//*****************************************************************************
//
//	Coin�N���X
//
//*****************************************************************************
class Coin
{
protected:


protected:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	int				judgeTimer;	//	����L������

public:
	//	�������E���
	Coin( void );
	~Coin( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	StageCollisionCheck( void );
	void	PlayerCollisionCheck( void );
	void	Move( void );
	void	Hitduringtheoperation( const Vector3& pos, const int& Num );	//�q�b�g������
	//	���擾�E�ݒ�
	void	SetPos( const Vector3& pos );
	void	SetAngle( const float& angle );
	void	SetScale( const float& angle );
	Vector3	GetPos( void );
	float		GetAngle( void );
	bool		GetState( void );

private:
	friend	class CoinManager;
};

//*****************************************************************************
#endif // !__COIN_H__
