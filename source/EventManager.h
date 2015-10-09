
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
			SLIP,
			FALL_BOMB,
			COIN_GETAWAY,
			UFO,
			NONE,
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
private:
	int		mode;
	EVENT_SLOPE	eventSlope;

public:
	//	初期化・解放
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	EventSlope( void );

	//	情報取得
	static	EventManager*	GetInstance( void );
	int		GetEvent( void )const;

	//	情報設定
	void	SetEvent( int eventflag );
};

#define	eventManager	( EventManager::GetInstance() )