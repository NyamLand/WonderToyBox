
#ifndef __UI_H__
#define	__UI_H__

//前方定義
class CoinBar;

//--------------------------------
//	UI
//--------------------------------
class HeadUpDisplay
{
private:
	CoinBar*	coinbar;
public:
	bool Initialize();
	HeadUpDisplay();
	~HeadUpDisplay();
	void Update();
	void Render();

};

extern HeadUpDisplay* m_UI;

//----------------------------------
//コインバー
//---------------------------------
class CoinBar
{
private:
	const int NUM_BAR = 4;
private:
	iex2DObj *img;
	int frame_x, frame_y, frame_sx, frame_sy;
	int bar_x[4], bar_y[4], bar_sx[4], bar_sy[4];

	void BarControl();
public:
	CoinBar();
	~CoinBar();

	void Update();
	void Render();
};


#endif // !__UI_H__