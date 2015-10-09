
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
			SLOPE_CAMERA,
			COIN_GETAWAY,
			NONE,
			SLIP,
			FALL_BOMB,
			UFO,
		};
	}
}

//	class
class EventManager
{
private:
	struct EVENT_SLOPE
	{
		bool	eventFlag;
		int		count;
		float	param;
		float	slope;
		int		step;
	};
	struct EVENT_COIN_GETAWAY
	{
		bool	eventflag;
		int		count;
		int		step;
	};
private:
	int		eventflag;
	EVENT_SLOPE				eventSlope;
	EVENT_COIN_GETAWAY	eventCoinGetAway;

public:
	//	初期化・解放
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	EventSlope( void );
	void	EventCoinGetAway( void );

	//	情報取得
	static	EventManager*	GetInstance( void );
	int		GetEvent( void )const;

	//	情報設定
	void	SetEvent( int eventflag );
};

#define	eventManager	( EventManager::GetInstance() )