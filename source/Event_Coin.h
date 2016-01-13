
#pragma	once

//*******************************************************************************
//
//	Event_Coinクラス　（コインイベント）
//
//*******************************************************************************

//	include

//	class
class Event_Coin
{
private:
	
	struct SACK
	{
		iex3DObj*	sack;
		bool	eventflag;
		int		count;
		int		step;
	};

	struct FALL
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct SPLASH
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct JUELBOX
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct WAVE
	{
		bool	eventflag;
		int		count;
		int		step;
	};

public:
	struct DUBBLE
	{
		bool	eventflag;
		int		count;
		int		step;
	};

private:
	int		eventflag;
	
	SACK		m_Sack;
	FALL		m_Fall;
	SPLASH		m_Splash;
	JUELBOX		m_Juelbox;
	WAVE		m_Wave;
	DUBBLE		m_Dubble;

public:
	//	初期化・解放
	bool	Initialize(void);
	void	Release(void);

	//	更新・描画
	void	Update(void);
	void	Render(void);

	//	動作関数
	void	Sack(void);
	void	Fall(void);
	void	Splash(void);
	void	Juelbox(void);
	void	Wave(void);
	void	Dubble(void);

	//	情報取得
	static	Event_Coin*	GetInstance(void);
	int		GetEvent(void)const;
	DUBBLE	GetDubbleInst();

	//	情報設定
	void	SetEvent(int eventflag);
};

#define	event_coin	( Event_Coin::GetInstance() )