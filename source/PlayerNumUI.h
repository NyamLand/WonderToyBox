
#pragma	once

//***********************************************************************
//
//	PlayerNumUIクラス
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
	//	初期化・解放
	PlayerNumUI( void );
	~PlayerNumUI( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	情報設定
	void	SetPos( int playerNum, int posX, int posY );

	//	情報取得
	//int	GetPosX( int playerNum );
	//int	GetPosY( int playerNum );
};
