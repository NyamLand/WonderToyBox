
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
	void	Sack(void);			//　コイン袋
	void	Fall(void);			//　滝
	void	Splash(void);		//　ワッシャァアア
	void	Juelbox(void);		//　宝石
	void	Wave(void);			//　隊列
	void	Dubble(void);		//　２倍タイム

	//	情報取得
	static	Event_Coin*	GetInstance(void);
	int		GetEvent(void)const;
	DUBBLE	GetDubbleInst(void)const;

	//	情報設定
	void	SetEvent(int eventflag);
};

#define	event_coin	( Event_Coin::GetInstance() )