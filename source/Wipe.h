#ifndef	__WIPE_H__
#define	__WIPE_H__

//*******************************************************************************
//	
//	Wipeクラス
//
//*******************************************************************************

class PlayerWipe
{
private:
		//	定数
	static	const		int		WIPE_WIDTH = 250;
	static	const		int		WIPE_HEIGHT = 250;
	static	const		int		SPACE = 50;

						float	WIPE_RIGHT;
						float	WIPE_LEFT;
						float	WIPE_UP;
						float	WIPE_DOWN;
						float	LEN_MAX;

private:
	//	個人カメラ
	unique_ptr<Camera>		playerView[PLAYER_MAX];
	
	//	ワイプ
	unique_ptr<iex2DObj>	wipe[PLAYER_MAX];

	//	画像
	ImageObj	arrow;

	//	変数
	Vector3		pos[PLAYER_MAX];
	Vector3		color[PLAYER_MAX];
	bool		check[PLAYER_MAX];
	Vector3		arrow_pos[PLAYER_MAX];
	float		arrow_angle[PLAYER_MAX];
	

public:
	//	初期化・解放
	PlayerWipe( void );
	~PlayerWipe( void );
	bool	Initialize( void );

	//	更新
	void	Update( void );
	void	Set( void );
	void	Check( int num );
	void	Arrow( int num, Vector3 target );
	void	Move( int num, Vector3 target );
	Vector3	LengthChecker( int num, Vector3 target );

	//	描画
	void	Render( void );
	void	RenderTarget( int num );

	//	情報取得
	static	PlayerWipe*		GetInstance();

};
#define	 playerWipe ( PlayerWipe::GetInstance() )


#endif	//	!__WIPE_H__