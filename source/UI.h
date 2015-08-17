
#ifndef __UI_H__
#define	__UI_H__

//前方定義
class CoinBar;
class Timer;

//--------------------------------
//	UI
//--------------------------------
class HeadUpDisplay
{
private:
	CoinBar*	c_Coinbar;
	Timer*		c_Timer;
private:

public:
	bool Initialize();
	HeadUpDisplay();
	~HeadUpDisplay();
	void Update();
	void Render();

	//	情報設定
	void	SetTimer( int time );
};

extern HeadUpDisplay* m_UI;

//----------------------------------
//コインバー
//---------------------------------
class CoinBar
{
private:
	const int NUM_BAR = 4;
	enum PlayerState
	{
		Normal = 0,
		Good = 1,
		Bad = 2
	};

private:
	iex2DObj *img_bar;
	iex2DObj *img_state;
	int frame_x, frame_y, frame_sx, frame_sy;
	int bar_x[4], bar_y[4], bar_sx[4], bar_sy[4];
	int state_x[4];
	int state_type[4];

private:
	void BarControl();
	void StateImageControl();
public:
	CoinBar();
	~CoinBar();

	void Update();
	void Render();
};

class Timer
{
private:
	const int SECOND = 60;	//60フレーム
	const int MINUTE = SECOND * 60;	//秒 x 60
	const int TIMELIMIT = 10 * SECOND;
private:
	iex2DObj* img;
	int x, y, sx, sy;
	int sec, minute[2];
	int timer;

	void Calculate_Time();

public:
	Timer();
	~Timer();

	void Update();
	void Render();

public:
	int GetTimer(){ return timer; };
	int	 SetTimer( int timer ){ return		this->timer = timer; }
};



#endif // !__UI_H__