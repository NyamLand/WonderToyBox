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

private:
	//	個人カメラ
	unique_ptr<Camera>		playerView[PLAYER_MAX];
	
	//	ワイプ
	unique_ptr<iex2DObj>	wipe[PLAYER_MAX];

	//	変数
	Vector2		pos[PLAYER_MAX];
	Vector3		color[PLAYER_MAX];
	bool		check[PLAYER_MAX];
	float		ASPECT;

public:
	//	初期化・解放
	PlayerWipe( void );
	~PlayerWipe( void );
	bool	Initialize( void );

	//	更新
	void	Update( void );
	void	Set( void );
	void	Check( float angle, int num );

	//	描画
	void	Render( void );
	void	RenderTarget( int num );

	//	情報取得
	static	PlayerWipe*		GetInstance();

};
#define	 playerWipe ( PlayerWipe::GetInstance() )


#endif	//	!__WIPE_H__