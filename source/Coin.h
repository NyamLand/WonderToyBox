
#ifndef __COIN_H__
#define	__COIN_H__

//*****************************************************************************
//
//	Coin�N���X
//
//*****************************************************************************

//	data
namespace
{
	namespace COIN_MODE
	{
		enum
		{
			NORMAL,
			GETAWAY,
			MAGNET,
		};
	}
}

class Coin
{
protected:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	bool			getAwayflag;
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
	virtual	void	Move( void );
	virtual	void	Hitduringtheoperation( const Vector3& pos, const int& Num );	//�q�b�g������
	void	GetAway( void );
	void	Magnet( void );

	//	���擾�E�ݒ�
	void	SetPos( const Vector3& pos );
	void	SetAngle( const float& angle );
	void	SetScale( const float& angle );
	void	SetGetAwayFlag( bool flag );
	Vector3	GetPos( void );
	float		GetAngle( void );
	bool		GetState( void );

private:
	friend	class CoinManager;
};

//*****************************************************************************
#endif // !__COIN_H__
