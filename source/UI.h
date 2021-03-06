
#ifndef		__UI_H__
#define	__UI_H__

//*********************************************************************************
//
//	UIクラス
//
//*********************************************************************************

#include	"iextreme.h"
#include	"GameManager.h"
#include	"AirPlane.h"
#include	"LifeUI.h"
#include	"Crown.h"
#include	"Camera.h"
#include	"CoinUIEffect.h"
#include	"CoinNumUI.h"
#include	"FallLife.h"
#include	"PlayerNumUI.h"
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
	//	警告
	struct AlertInfo
	{
		bool	flag;
		int		timer;
		float	alpha;
		float	param;
		int		type;
	};

	//	コイン枚数構造体
	struct NumberImageInfo
	{
		int				scale;
		POINT			pos;
		ImageObj		hundred;			//	コイン三桁目
		ImageObj		ten;				//	コイン二桁目
		ImageObj		one;				//	コイン一桁目
		bool			hundredRenderFlag;	//	百の位レンダー用フラグ
	};

	//	数字情報
	struct NumberInfo
	{
		int		hundred;	//コイン三桁目
		int		ten;			//コイン二桁目
		int		one;			//コイン一桁目
		bool		H_flg;		//百の位レンダー用フラグ
	};

	//　イベント情報
	struct EventInfo
	{
		int mode;
		int step;
		int state;
		ImageObj	texture;	//　飛行機
		AirPlane*	airPlane;
	};

	//	タイマー情報
	struct TimerInfo
	{
		int		x, y, sx, sy;
		int		time;
		int		second[2], minute;
	};

	//	ラストタイマー情報
	struct LastTimerInfo
	{
		bool	state;
		float	alpha;
		float	t;
	};

	//	カウントダウン情報
	struct CountInfo
	{
		int		count;
		Vector3 start_pos;
		Vector3 finish_pos;
		float	start_t;
		int		start_step;
		int		waitTimer;
	};

private:
	//	各画像
	ImageObj	timer;
	ImageObj	faceImage[PLAYER_MAX];
	ImageObj	countImage;
	ImageObj	alertImage;
	ImageObj	alert_coinImage;
	ImageObj	startNum[PLAYER_MAX];
	ImageObj	pNumImage[PLAYER_MAX];
	ImageObj	roundImage;
	ImageObj	finishImage;
	iex2DObj*	face;
	iex2DObj*	playerNumber;
	iex2DObj*	startNumber;
	iex2DObj*	pCoinNumImage;

private:
	//	定数
	Vector3 PAR_POS;

	//	システム
	bool	changeflag;

	//	カウントダウン・スタート・タイムアップ演出
	CountInfo			countInfo;

	//	コイン枚数
	int		coinNum[PLAYER_MAX];
	Vector3	coinColor[PLAYER_MAX];

	//　キャラ情報
	int		charatype[PLAYER_MAX];

	//	ラスト時間
	LastTimerInfo		lasttimerInfo;
	
	//	タイマー
	TimerInfo			timerInfo;
	
	//	警告パラメータ
	AlertInfo			alertInfo;

	//	ライフ
	LifeUI*			lifeUI;

	//	落下ライフ
	FallLife*		fallLife;

	//	王冠
	Crown*		crown;

	//	プレイヤー番号
	PlayerNumUI*	playerNumUI;

	//	コイン枚数情報
	NumberImageInfo	coinNumInfo[PLAYER_MAX];
	NumberInfo			numInfo[PLAYER_MAX];

	//　イベント情報
	EventInfo			eventInfo;

	//	パーティクル用
	bool		coin_flg[PLAYER_MAX];
	int			coin_timer[PLAYER_MAX];
	
	//	パーティクル用バックバッファ
	std::unique_ptr<iex2DObj>	target_par;

	//	パーティクル用カメラ
	std::unique_ptr<Camera>		particle_camera;
		
	//	パラメータ
	int		scene;
	
public:
	//	初期化・解放
	UI( void );
	~UI( void );
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( const int& mode );
	void	Render( const int& mode );

	//-------------------------------メイン処理-------------------------------------//
	//	メイン動作初期化
	void	TimerInitialize( void );
	void	StartAndTimeUpInitialize( void );
	void	StartPlayerNumInitialize( void );
	void	AlertInitialize( void );
	void	LastProductionInitialize( void );
	void	PlayerNumberInitialize( void );
	void	CoinNumberInitialize( void );
	void	FaceImageInitialize( void );
	void	RoundInitialize( void );
	void	EventInitialize( void );

	//	メイン動作更新
	void	TimerUpdate( void );
	void	StartUpdate( void );
	bool	StartMove( void );
	void	StartPlayerNumUpdate( void );
	void	FinishUpdate( void );
	void	AlertUpdate( void );
	void	LastProduction( void );
	void	PlayerNumberUpdate( void );
	void	CoinNumberUpdate( void );
	void	FaceImageUpdate( int num, int mode );
	void	CoinImageInfoUpdate( NumberImageInfo& numImageinfo, NumberInfo& numinfo, const int& num );
	void	EventUpdate( void );
	void	ParticleUpdate( void );
	void	FallLifeUpdate( void );

	//	メイン描画
	void	TimerRender( void );
	void	StartRender( void );
	void	StartPlayerNumRender( void );
	void	FinishRender( void );
	void	AlertRender( void );
	void	LastProductionRender( void );
	void	CoinNumberRender( void );
	void	EventRender( void );
	void	ParticleRender( int value );
	void	RenderTargetParticle( void );

	//	メイン動作関数
	void	CoinCounter( int coin , int num );
	void	RoundRender( void );

	//------------------------------情報取得・設定------------------------------------//

	//	情報設定
	void	SetChangeFlag( const bool& flag );
	void	SetAlertInfo( bool flag, int type );
	void	SetCoinImageInfo( NumberImageInfo& numImageinfo, NumberInfo& numinfo, const int& num );
	void	SetNumberInfo( NumberInfo& nomber, int coin );
	void	SetEventInfoMode( int mode );
	void	SetDamageFlag( int playerNum, bool flag, int culLife );

	//	情報取得
	bool	GetChangeFlag( void );
	bool	GetMoveState( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__