
#pragma	once

//**********************************************************************
//
//	FallLife�N���X
//
//**********************************************************************

//	include

//	class
class FallLife
{
private:
	//	���������萔
	enum FALL_DIRECTION
	{
		LEFT_FALL = -1,
		RIGHT_FALL = 1,
	};

	//	���W�ۑ��p�\����
	struct SavePos
	{
		int	x;
		int	y;
	};

private:
	iex2DObj*				orgLife;
	ImageObj					lifeImage[PLAYER_MAX];
	bool							damageFlag[PLAYER_MAX];
	float							param[PLAYER_MAX];
	FALL_DIRECTION		fallDirection[PLAYER_MAX];
	SavePos					savePos[PLAYER_MAX];
	int							moveY[PLAYER_MAX];

public:
	//	�������E���
	FallLife( void );
	~FallLife( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	bool	AddPercentage( int playerNum );
	void	Fall( int playerNum );
	void	AlphaUpdate( int playerNum );
	void	AngleUpdate( int playerNum );

	//	���ݒ�
	void	SetPos( int playerNum, int posX, int posY );
	void	SetSrcPos( int playerNum, int sx, int sy );
	void	SetDamageFlag( int playerNum, bool flag, int culLife );
};
