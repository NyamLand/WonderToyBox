
#pragma	once

//**********************************************************************
//
//	FallLifeクラス
//
//**********************************************************************

//	include

//	class
class FallLife
{
private:
	//	落下方向定数
	enum FALL_DIRECTION
	{
		LEFT_FALL = -1,
		RIGHT_FALL = 1,
	};

	//	座標保存用構造体
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
	//	初期化・解放
	FallLife( void );
	~FallLife( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	bool	AddPercentage( int playerNum );
	void	Fall( int playerNum );
	void	AlphaUpdate( int playerNum );
	void	AngleUpdate( int playerNum );

	//	情報設定
	void	SetPos( int playerNum, int posX, int posY );
	void	SetSrcPos( int playerNum, int sx, int sy );
	void	SetDamageFlag( int playerNum, bool flag, int culLife );
};
