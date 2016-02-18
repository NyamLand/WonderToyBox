
#ifndef __UI_H__
#define	__UI_H__

//*********************************************************************************
//
//	UIクラス
//
//*********************************************************************************

#include	"iextreme.h"
#include	"GameManager.h"
#include	"AirPlane.h"
#include	"Camera.h"
#include	<memory>

namespace
{	
	namespace ALERT_TYPE_INFO
	{
		enum
		{
			JAM,
			COIN,
			MISSION
		};
	}

	//　イベントの種類
	namespace EVENT_TEX_INFO
	{
		enum
		{
			C_FALL,
			C_WAVE,
			C_DUBBLE,
			J_CAMERA,
			J_SLIP,
			J_GETAWAY
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

	struct HURRY_INFO
	{
		bool	flag;
		int		timer;
		float	alpha;
		float	param;
	};

	//	コイン枚数構造体
	struct NUMBERIMAGE_INFO
	{
		int				scale;
		POINT			pos;
		ImageObj		hundred;						//	コイン三桁目
		ImageObj		ten;								//	コイン二桁目
		ImageObj		one;								//	コイン一桁目
		bool				hundredRenderFlag;	//	百の位レンダー用フラグ
	};

	//	数字情報
	struct NUMBER_INFO
	{
		int		hundred;	//コイン三桁目
		int		ten;			//コイン二桁目
		int		one;			//コイン一桁目
		bool		H_flg;		//百の位レンダー用フラグ
	};
	
	//	ライフ情報
	struct LIFE_INFO
	{
		int	life;
		ImageObj	lifeImage;
	};

	//	王冠情報
	struct CROWN_INFO
	{
		bool	state;
		ImageObj	crownImage;
	};

	//　イベント情報
	struct EVENT_INFO
	{
		int mode;
		int step;
		int state;
		ImageObj	texture;	//　飛行機
		AirPlane*	airPlane;
	};

private:
	//	各画像
	ImageObj	timer;
	iex2DObj*	coinbar;
	ImageObj		frame;
	ImageObj		backgauge;
	ImageObj		gauge;
	iex2DObj*	face;
	ImageObj		faceImage[PLAYER_MAX];
	ImageObj		countImage;
	ImageObj		alertImage;
	ImageObj		alert_coinImage;
	iex2DObj*	playerNumber;
	iex2DObj*	startNumber;
	ImageObj		startNum[PLAYER_MAX];
	ImageObj		pNumImage[PLAYER_MAX];
	iex2DObj*	pCoinNumImage;
	iex2DObj*	life;
	iex2DObj*	crown;
	ImageObj		roundImage;
	ImageObj		finishImage;

private:
	//	定数
	Vector3 PAR_POS;

	//	システム
	bool	changeflag;

	//	カウントダウン・スタート・タイムアップ演出
	int		count;
	Vector3 start_pos;
	Vector3 finish_pos;
	float	start_t;
	int		start_step;
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

	//	HurryUpパラメータ
	HURRY_INFO	hurryInfo;

	//	ライフ情報
	LIFE_INFO			lifeInfo[4];

	//	王冠情報
	CROWN_INFO			crownInfo[4];

	//	コイン枚数情報
	NUMBERIMAGE_INFO	coinNumInfo[PLAYER_MAX];
	NUMBER_INFO			numInfo[PLAYER_MAX];

	//　イベント情報
	EVENT_INFO	eventInfo;
	
	//	パーティクル用バックバッファ
	std::unique_ptr<iex2DObj>	target_par;

	//	パーティクル用
	bool		coin_flg[PLAYER_MAX];
	int		coin_timer[PLAYER_MAX];

	//	パーティクル用カメラ
	std::unique_ptr<Camera>		particle_camera;
		
	//	パラメータ
	int		scene;
	
public:
	//------------------------------共通処理------------------------------------//
	//	初期化・解放
	UI( void );
	~UI( void );
	bool	Initialize(void );
	void	Release( void );
	void	Load( void );

	//	各シーン初期化
	void	TitleInitialize( void );
	void	MainInitialize( void );
	
	//	各シーン解放
	void	TitleRelease( void );
	void	MainRelease( void );

	//	更新・描画
	void	Update( const int& mode );
	void	Render( const int& mode );

	//	各シーン更新
	void	MainUpdate( int mode );

	//	各シーン描画
	void	MainRender( int mode );

	//-------------------------------メイン処理-------------------------------------//
	//	メイン動作初期化
	void	TimerInitialize( void );
	void	StartAndTimeUpInitialize( void );
	void	StartPlayerNumInitialize( void );
	void	AlertInitialize( void );
	void	HurryUpInitialize( void );
	void	LastProductionInitialize( void );
	void	PlayerNumberInitialize( void );
	void	LifeInitialize( void );
	void	CrownInitialize( void );
	void	CoinNumberInitialize( void );
	void	RoundInitialize( void );
	void	EventInitialize( void );

	//	メイン動作更新
	void	TimerUpdate( void );
	void	StartUpdate( void );
	bool	StartMove( void );
	void	StartPlayerNumUpdate( void );
	void	FinishUpdate( void );
	void	AlertUpdate( void );
	void	HurryUpdate( void );
	void	LastProduction( void );
	void	PlayerNumberUpdate( void );
	void	LifeUpdate( void );
	void	CrownUpdate( void );
	void	CoinNumberUpdate( void );
	void	FaceImageUpdate( int num, int mode );
	void	CoinImageInfoUpdate(NUMBERIMAGE_INFO& numImageinfo, NUMBER_INFO& numinfo, const int& num);
	void	EventUpdate( void );
	void	ParticleUpdate( void );

	//	メイン描画
	void	TimerRender( void );
	void	StartRender( void );
	void	StartPlayerNumRender( void );
	void	FinishRender( void );
	void	AlertRender( void );
	void	LastProductionRender( void );
	void	PlayerNumberRender( void );
	void	LifeRender( void );
	void	CrownRender( void );
	void	CoinNumberRender( void );
	void	EventRender( void );
	void	ParticleRender( int value );
	void	RenderTargetParticle( void );

	//	メイン動作関数
	void StateImageControl( void );
	void CoinCounter( int coin , int num);
	void	RoundRender( void );

	//------------------------------情報取得・設定------------------------------------//
	//	情報設定
	void	SetChangeFlag( const bool& flag );
	void	SetAlertInfo( bool flag, int type );
	void	SetHurryFlag( bool flag );
	void	SetImageSrcPos( int mode );
	void	SetImageSrcPos( int sx, int sy );
	void	SetCoinImageInfo(NUMBERIMAGE_INFO& numImageinfo, NUMBER_INFO& numinfo, const int& num);
	void	SetNumberInfo(NUMBER_INFO& nomber, int coin);
	void	SetEventInfoMode(int mode);

	//	情報取得
	bool	GetChangeFlag( void );
	bool	GetMoveState( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__