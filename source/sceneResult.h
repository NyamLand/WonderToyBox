
#ifndef __SCENERESULT_H__
#define	__SCENERESULT_H__

//*******************************************************************************
//
//	sceneResultクラス
//
//*******************************************************************************

class	PlayerManager;

class sceneResult : public	Scene
{
private:
	struct RESULT_INFO
	{
		int		p_Coin;
		int		p_num;
	};

	struct NUMBER
	{
		int		hundred;
		int		ten;
		int		one;
	};


private:
	RESULT_INFO	resultInfo[4];
	
	//	ソート用
	char	str[256];

	//	パラメータ
	int		coinNum[4];
	int		rank[4];
	int		lastBonus;
	int		addcoinNum[4];
	int		step;
	int		playerNum;
	int		wait;
	int		hundred[4];		//コイン三桁目
	int		ten[4];			//コイン二桁目
	int		one[4];			//コイン一桁目
	int		resultcount;	//	演出用カウント
	int		Mode;			//	モード分け用変数
	bool	Modeflg;		//モード用フラグ
	float	Sx,Sy;				//セレクト画面ポジション
	iex2DObj*	back;		//背景
	iex2DObj*	r_number;	//コインの枚数
	iex2DObj*	Sback;		//セレクト時の背景
	iex2DObj*	Smenu;		//セレクト時のメニュー
	iexMesh*	collision;
	NUMBER	number[4];

public:
	//	初期化・解放
	sceneResult( void );
	~sceneResult( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	void	ResultUpdate( void );
	void	SelectUpdata( void );

	void	SelectRender( void );
	//	動作関数
	void	BubbleSort( void );
	void	SetLastBonus( void );
	void	SetRank( void );

	//	演出用関数
	void	Production( void );						//リザルトの演出用関数
	void	ProductionRotation( int playerNum );	//コイン枚数回転関数
	void	ProductionCoinHandOff(int playerNum);	//コイン枚数引き渡し
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
