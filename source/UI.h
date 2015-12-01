
#ifndef __UI_H__
#define	__UI_H__

//*********************************************************************************
//
//	UIクラス
//
//*********************************************************************************

#include	"GameManager.h"

namespace
{
	namespace UI_MODE
	{
		enum
		{
			TITLE,
			MAIN,
			RESULT,
		};
	}
	
	namespace ALERT_TYPE_INFO
	{
		enum
		{
			JAM,
			COIN
		};
	}

}

//	UIクラス
class UI
{
private:
	struct ALERT_INFO
	{
		bool	flag;
		int		timer;
		float	alpha;
		float	param;
		int		type;
	};

	struct TITLE_INFO
	{
		ImageObj	textImage;
		float	param;
		float	t;
		int		step;
		int		mode;
		bool	moveState;
		int		savePos;
	};

	struct DONKETSU_DIRECT_INFO
	{
		const int WAIT_MAX = 5 * SECOND + 30;

		//　顔関連
		ImageObj	face;
		int		f;
		int		roulette;
		int		face_step;

		//　DonketsuBoooooost!!!
		ImageObj	DB;
		int			DB_step;

		//　？P
		ImageObj	P;
		int			P_step;
	};

	struct HURRY_INFO
	{
		bool	flag;
		int		timer;
		float	alpha;
		float	param;
	};

	struct AIRPLANE_INFO
	{
		int IN_START_POS_X;
		int IN_START_POS_Y;
		int IN_END_POS_X;
		int IN_END_POS_Y;
		int OUT_START_POS_X;
		int OUT_END_POS_X;
		int OUT_END_POS_Y;
		int ROLL_POINT_ADJUST_X;
	};

	//	コインバー情報
	struct COINBAR_INFO
	{
		float	param[4];
		float	sub[4];
		float	scale[4];
		int		coinNum[4];
	};

private:
	//	各画像
	ImageObj	timer;
	iex2DObj*	coinbar;
	ImageObj	frame;
	ImageObj	backgauge;
	ImageObj	gauge;
	iex2DObj*	face;
	ImageObj	faceImage;
	ImageObj	countImage;
	ImageObj	alertImage;
	ImageObj	alert_coinImage;
	iex2DObj*	playerNumber;
	ImageObj	pNumImage[PLAYER_MAX];

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

	//　キャラ情報
	int		charatype[4];

	//	ニュース
	NewsBar	newsbar;

	//	警告パラメータ
	ALERT_INFO	alertInfo;

	//	タイトルパラメータ
	TITLE_INFO	titleInfo;

	//　どんけつ演出パラメータ
	DONKETSU_DIRECT_INFO ddInfo;

	//	HurryUpパラメータ
	HURRY_INFO	hurryInfo;

	//	飛行機パラメータ
	AIRPLANE_INFO		airPlaneInfo;

	//	コインバー情報
	COINBAR_INFO	coinBarInfo;

	//	パラメータ
	int		scene;
	
public:
	//------------------------------共通処理------------------------------------//
	//	初期化・解放
	UI( void );
	~UI( void );
	bool	Initialize( int scene );
	void	Release( void );

	//	各シーン初期化
	void	TitleInitialize( void );
	void	MainInitialize( void );
	void	ResultInitialize( void );
	
	//	各シーン解放
	void	TitleRelease( void );
	void	MainRelease( void );
	void	ResultRelease( void );

	//	更新・描画
	void	Update( const int& mode );
	void	Render( const int& mode );
	void	DrawDebug();

	//	各シーン更新
	void	TitleUpdate( int mode );
	void	MainUpdate( int mode );
	void	ResultUpdate( int mode );

	//	各シーン描画
	void	TitleRender( int mode );
	void	MainRender( int mode );
	void	ResultRender( int mode );

	//------------------------------タイトル処理------------------------------------//
	void	AirPlaneInitialize( void );
	bool	FlyingIn( void );
	bool	FlyingOut( int startPos );

	//-------------------------------メイン処理-------------------------------------//
	//	メイン動作初期化
	void	CoinBarInitialize( void );
	void	TimerInitialize( void );
	void	NewsBarInitialize( void );
	void	StartAndTimeUpInitialize( void );
	void	DonketsuDirectionInitialize( void );
	void	AlertInitialize( void );
	void	HurryUpInitialize( void );
	void	PlayerNumberInitialize( void );

	//	メイン動作更新
	void	TimerUpdate( void );
	void	CoinBarUpdate( void );
	void	NewsBarUpdate( void );
	void	StartUpdate( void );
	void	FinishUpdate( void );
	void	DonketsuDirectionUpdate( void );
	void	AlertUpdate( void );
	void	HurryUpdate( void );
	void	LastProduction( void );
	void	PlayerNumberUpdate( void );

	//	メイン描画
	void	TimerRender( void );
	void	CoinBarRender( void );
	void	NewsBarRender( void );
	void	StartRender( void );
	void	FinishRender( void );
	void	DonketsuDirectionRender( void );
	void	AlertRender( void );
	void	LastProductionRender( void );
	void	PlayerNumberRender( void );

	//	メイン動作関数
	void BarControl( void );
	void StateImageControl( void );
	void FaceRoulette( int face_wait );
	void DB_Direction( int wait );
	void P_Direction( int wait );

	//	テスト用
	void	BarControl2( void );
	void	BarRender( void );
	//------------------------------情報取得・設定------------------------------------//

	//	情報設定
	void	SetChangeFlag( const bool& flag );
	void	SetAlertInfo( bool flag, int type );
	void	SetHurryFlag( bool flag );
	void	SetFlyingIn( int type );
	void	SetImageSrcPos( int mode );
	void	SetImageSrcPos( int sx, int sy );

	//	情報取得
	bool	GetChangeFlag( void );
	bool	GetMoveState( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__