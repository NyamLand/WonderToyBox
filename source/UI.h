
#ifndef __UI_H__
#define	__UI_H__

//*********************************************************************************
//
//	UIクラス
//
//*********************************************************************************

//	UIクラス
class UI
{
private:
	//	各画像
	iex2DObj*	timer;
	iex2DObj*	coinbar;
	iex2DObj*	face;
	ImageObj	countImage;

private:
	//	システム
	bool	changeflag;

	//	カウントダウン・スタート・タイムアップ演出
	int		count;
	int		waitTimer;

	//	タイマー
	int		x, y, sx, sy;
	int		time;
	int		second, minute[2];

	//	コインバー
	static	const		int		NUM_BAR = 4;
	int		frame_x, frame_y, frame_sx, frame_sy;
	int		bar_x[4], bar_y[4], bar_sx[4], bar_sy[4];
	int		state_x[4];
	int		state_type[4];

	//	ニュース
	NewsBar	newsbar;
	
	//　プレイヤー（顔）
	int		charatype[4];
	int		f;
	int		roulette;
	
public:
	//	初期化・解放
	UI( void );
	~UI( void );
	bool	Initialize( void );
	void	CoinBarInitialize( void );
	void	TimerInitialize( void );
	void	NewsBarInitialize( void );
	void	StartAndTimeUpInitialize( void );
	void	DonketsuDirectionInitialize( void );
	void	Release( void );

	//	更新
	void	Update( const int& mode );
	void	TimerUpdate( void );
	void	CoinBarUpdate( void );
	void	NewsBarUpdate( void );
	void	StartUpdate( void );
	void	FinishUpdate( void );
	void	DonketsuDirectionUpdate( void );

	//	描画
	void	Render( const int& mode );
	void	TimerRender( void );
	void	CoinBarRender( void );
	void	NewsBarRender( void );
	void	StartRender( void );
	void	FinishRender( void );
	void	DonketsuDirectionRender( void );

	//	動作関数
	void BarControl( void );
	void StateImageControl( void );

	//	情報取得・設定
	void	SetChangeFlag( const bool& flag );
	bool	GetChangeFlag( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__