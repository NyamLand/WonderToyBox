#pragma once

//*******************************************************************************
//
//	Event_Jammingクラス　（妨害イベント）
//
//*******************************************************************************

#define	SLOPE_COUNT_MAX	300
#define	COIN_GETAWAY_COUNT_MAX	600
#define	SLIP_COUNT_MAX	600

// include
#include	"EventManager.h"

//	class
class Event_Jamming : public EventManager
{
private:
	struct SLOPE
	{
		bool	eventFlag;
		float	param;
		float	slope;
		int		step;
	};

	struct COINGETAWAY
	{
		bool	eventflag;
		int		step;
	};

	struct SLIP
	{
		bool	eventflag;
		int		step;
	};

	
private:
	int		eventflag;
	SLOPE			m_Slope;
	COINGETAWAY		m_CoinGetAway;
	SLIP			m_Slip;

public:
	//	初期化・解放
	bool	Initialize(void);
	void	Release(void);

	//	更新・描画
	void	Update(void);
	void	Render(void);

	//	動作関数
	void	Slope(void);
	void	CoinGetAway(void);
	void	Slip(void);
	
	int		GetEvent(void)const;

	//	情報設定
	static	Event_Jamming*	GetInstance(void);
	void	SetEvent(int eventflag);
};

#define	event_jamming	(Event_Jamming::GetInstance())