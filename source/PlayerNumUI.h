
#pragma	once

//***********************************************************************
//
//	PlayerNumUI�N���X
//
//***********************************************************************

//	include

//	class
class PlayerNumUI
{
private:
	iex2DObj*	orgPlayerNum;
	ImageObj		playerNumImage[PLAYER_MAX];

public:
	//	�������E���
	PlayerNumUI( void );
	~PlayerNumUI( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	���ݒ�
	void	SetPos( int playerNum, int posX, int posY );

	//	���擾
	//int	GetPosX( int playerNum );
	//int	GetPosY( int playerNum );
};
