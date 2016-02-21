
#pragma once

//**********************************************************************
//
//	CoinNumUI�N���X
//
//**********************************************************************

//	include

//	class�O���錾
class NumberUI;
class CoinUIEffect;

//	class
class CoinNumUI
{
private:
	CoinUIEffect*	coinUIEffect;
	NumberUI*		numberUI[PLAYER_MAX];
	Vector3			colorTable[PLAYER_MAX];
	bool					effectFlag[PLAYER_MAX];
	int					posXTable[PLAYER_MAX];
	int					coinNum[PLAYER_MAX];

public:
	//	�������E���
	CoinNumUI( void );
	~CoinNumUI( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );
	void	TableInitialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	CoinEffectUpdate( void );
	void	Render( void );
	void	RenderToBackBuffer( void );

	//	����֐�
	void	CoinCount( int coinNum, int playerNum );
};
