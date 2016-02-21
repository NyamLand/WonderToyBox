
#pragma once

//**********************************************************************
//
//	Crown�N���X
//
//**********************************************************************

//	include

//	class
class Crown
{
private:
	iex2DObj*	orgCrown;
	ImageObj		crownImage[PLAYER_MAX];
	bool				state[PLAYER_MAX];

public:
	//	�������E���
	Crown( void );
	~Crown( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );


};
