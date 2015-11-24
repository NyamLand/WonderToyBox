
#pragma	once

//*******************************************************************************
//
//	EventManagerクラス
//
//*******************************************************************************

//	include

//	data
namespace
{
	namespace EVENT_MODE
	{
		enum
		{
			JAM_SLOPE_CAMERA,
			JAM_COIN_GETAWAY,
			JAM_SLIP,
			JAM_NONE,
			JAM_FALL_BOMB,
			JAM_UFO,

			COIN_SACK,
			COIN_FALL,
			COIN_SPLASH,
			COIN_JUELBOX,
			COIN_WAVE,
			COIN_DUBBLE
		};
	}
}

//	class
class EventManager
{
private:
	struct EVENT_JAM_SLOPE
	{
		bool	eventFlag;
		int		count;
		float	param;
		float	slope;
		int		step;
	};

	struct EVENT_JAM_COINGETAWAY
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct EVENT_JAM_SLIP
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct EVENT_COIN_SACK	
	{
		iex3DObj*	sack;
		bool	eventflag;
		int		count;
		int		step;
	};

	struct EVENT_COIN_FALL
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct EVENT_COIN_SPLASH
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct EVENT_COIN_JUELBOX
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct EVENT_COIN_WAVE
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct EVENT_COIN_DUBBLE
	{
		bool	eventflag;
		int		count;
		int		step;
	};
	
private:
	int		eventflag;
	EVENT_JAM_SLOPE				event_Jam_Slope;
	EVENT_JAM_COINGETAWAY	event_Jam_CoinGetAway;
	EVENT_JAM_SLIP					event_Jam_Slip;

	EVENT_COIN_SACK				event_Coin_Sack;
	EVENT_COIN_FALL					event_Coin_Fall;
	EVENT_COIN_SPLASH			event_Coin_Splash;
	EVENT_COIN_JUELBOX			event_Coin_Juelbox;
	EVENT_COIN_WAVE				event_Coin_Wave;
	EVENT_COIN_DUBBLE			event_Coin_Dubble;

public:
	//	初期化・解放
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	Event_Jam_Slope( void );
	void	Event_Jam_CoinGetAway( void );
	void	Event_Jam_Slip( void );
	void	Event_Coin_Sack( void );
	void	Event_Coin_Fall( void );
	void	Event_Coin_Splash( void );
	void	Event_Coin_Juelbox( void );
	void	Event_Coin_Wave( void );
	void	Event_Coin_Dubble( void );

	//	情報取得
	static	EventManager*	GetInstance( void );
	int		GetEvent( void )const;

	//	情報設定
	void	SetEvent( int eventflag );
};

#define	eventManager	( EventManager::GetInstance() )