
#pragma once

//**********************************************************************
//
//	LifeUIクラス
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
	//	初期化・解放
	LifeUI( void );
	~LifeUI( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	情報取得
	int	GetPosX( int playerNum )const;
	int	GetPosY( int playerNum )const;
};
