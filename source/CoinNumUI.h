
#pragma once

//**********************************************************************
//
//	CoinNumUIクラス
//
//**********************************************************************

//	include

//	class前方宣言
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
	//	初期化・解放
	CoinNumUI( void );
	~CoinNumUI( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );
	void	TableInitialize( void );

	//	更新・描画
	void	Update( void );
	void	CoinEffectUpdate( void );
	void	Render( void );
	void	RenderToBackBuffer( void );

	//	動作関数
	void	CoinCount( int coinNum, int playerNum );
};
