
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
#include	"StartFinUI.h"
#include	"FallLife.h"
#include	"PlayerNumUI.h"
#include	"CoinNumUI.h"
#include	"FaceUI.h"
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
	ImageObj	alertImage;
	ImageObj	alert_coinImage;
	ImageObj	startNum[PLAYER_MAX];
	ImageObj	pNumImage[PLAYER_MAX];
	ImageObj	roundImage;
	ImageObj	redAlert;
	iex2DObj*	playerNumber;
	iex2DObj*	startNumber;

private:
	//	定数
	Vector3 PAR_POS;

	//	カウントダウン・スタート・タイムアップ演出
	CountInfo			countInfo;
	
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

	//	コイン枚数UI情報
	CoinNumUI*		coinNumUI;

	//	背景（顔）
	FaceUI*			faceUI;

	//	開始・終了UI
	StartFinUI*		startfinUI;

	//　イベント情報
	EventInfo			eventInfo;
		
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
	void	StartPlayerNumInitialize( void );
	void	AlertInitialize( void );
	void	LastProductionInitialize( void );
	void	PlayerNumberInitialize( void );
	void	RoundInitialize( void );
	void	EventInitialize( void );

	//	メイン動作更新
	void	TimerUpdate( void );
	void	StartPlayerNumUpdate( void );
	void	AlertUpdate( void );
	void	LastProduction( void );
	void	PlayerNumberUpdate( void );
	void	EventUpdate( void );
	void	FallLifeUpdate( void );

	//	メイン描画
	void	TimerRender( void );
	void	StartPlayerNumRender( void );
	void	AlertRender( void );
	void	LastProductionRender( void );
	void	EventRender( void );
	void	RenderTargetParticle( void );

	//	メイン動作関数
	void	CoinCounter( int coin , int num );
	void	RoundRender( void );

	//------------------------------情報取得・設定------------------------------------//

	//	情報設定
	void	SetChangeFlag( const bool& flag );
	void	SetAlertInfo( bool flag, int type );
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