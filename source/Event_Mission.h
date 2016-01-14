
#pragma	once

//*******************************************************************************
//
//	Event_Missionクラス　（コインイベント）
//
//*******************************************************************************

//	include
#include	"EventManager.h"

//	class
class Event_Mission : public EventManager
{
private:
	struct COLLECT
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct STAY
	{
		bool	eventflag;
		int		count;
		int		step;
	};

private:
	int		eventflag;
	bool	success;
	COLLECT		m_Collect;
	STAY		m_Stay;

public:
	//	初期化・解放
	bool	Initialize(void);
	void	Release(void);

	//	更新・描画
	void	Update(void);
	void	Render(void);

	//	動作関数
	void	Collect(void);
	void	Stay(void);
	
	//	情報取得
	static	Event_Mission*	GetInstance(void);
	int		GetEvent(void)const;

	//	情報設定
	void	SetEvent(int eventflag);
};

#define	event_mission	( Event_Mission::GetInstance() )