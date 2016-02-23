
#pragma once

//**********************************************************************
//
//	LifeUI�N���X
//
//**********************************************************************

//	include

//	class
class LifeUI
{
private:
	iex2DObj*	orgLife;
	ImageObj	lifeImage[PLAYER_MAX];
	int	life[PLAYER_MAX];

public:
	//	�������E���
	LifeUI( void );
	~LifeUI( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	���擾
	int	GetPosX( int playerNum )const;
	int	GetPosY( int playerNum )const;
};
