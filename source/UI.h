
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

	//	コイン枚数構造体
	struct NUMBERIMAGE_INFO
	{
		int				scale;
		POINT			pos;
		ImageObj		hundred;			//	コイン三桁目
		ImageObj		ten;				//	コイン二桁目
		ImageObj		one;				//	コイン一桁目
		bool			hundredRenderFlag;	//	百の位レンダー用フラグ
	};

	//	数字情報
	struct NUMBER_INFO
	{
		int		hundred;		//コイン三桁目
		int		ten;			//コイン二桁目
		int		one;			//コイン一桁目
		bool	H_flg;			//百の位レンダー用フラグ
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
	iex2DObj*	pCoinNumImage;

private:
	//	システム
	bool	changeflag;

	//	カウントダウン・スタート・タイムアップ演出
	int		count;
	int		waitTimer;

	//	タイマー
	int		x, y, sx, sy;
	int		time;
	int		second[2], minute;

	//	コインバー
	static	const		int		NUM_BAR = 4;
	int		frame_x, frame_y, frame_sx, frame_sy;
	int		bar_x[4], bar_y[4], bar_sx[4], bar_sy[4];
	int		state_x[4];
	int		state_type[4];

	//	コイン枚数
	int		coinNum[PLAYER_MAX];
	Vector3	coinColor[PLAYER_MAX];

	//　キャラ情報
	int		charatype[PLAYER_MAX];

	//	ラスト時間
	bool	last_state;
	float	last_alpha;
	float	last_t;

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

	//	コイン枚数情報
	NUMBERIMAGE_INFO	coinNumInfo[PLAYER_MAX];
	NUMBER_INFO			numInfo[PLAYER_MAX];

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
	void	LastProductionInitialize( void );
	void	PlayerNumberInitialize( void );
	void	CoinNumberInitialize( void );

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
	void	CoinNumberUpdate( void );
	void	CoinImageInfoUpdate(NUMBERIMAGE_INFO& numImageinfo, NUMBER_INFO& numinfo, const int& num);


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
	void	CoinNumberRender( void );

	//	メイン動作関数
	void BarControl( void );
	void StateImageControl( void );
	void FaceRoulette( int face_wait );
	void DB_Direction( int wait );
	void P_Direction( int wait );
	void CoinCounter( int coin , int num);

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
	void	SetCoinImageInfo(NUMBERIMAGE_INFO& numImageinfo, NUMBER_INFO& numinfo, const int& num);
	void	SetNumberInfo(NUMBER_INFO& nomber, int coin);

	//	情報取得
	bool	GetChangeFlag( void );
	bool	GetMoveState( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__