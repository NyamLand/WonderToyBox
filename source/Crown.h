
#pragma once

//**********************************************************************
//
//	Crownクラス
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
	//	初期化・解放
	Crown( void );
	~Crown( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );


};
