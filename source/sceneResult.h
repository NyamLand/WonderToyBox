
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
		int		p_addCoin;
	};

	struct NUMBER
	{
		int		hundred;		//コイン三桁目
		int		ten;			//コイン二桁目
		int		one;			//コイン一桁目
	};

private:
	RESULT_INFO	resultInfo[4];
	NUMBER		number[4];		//	取得コイン枚数用ナンバー
	NUMBER		addnumber[4];	//	取得コインとボーナスの合算用ナンバー
	//	ソート用
	char	str[256];

	//	パラメータ
	int		coinNum[4];
	int		playerNum[4];
	int		lastBonus;
	int		lastBonusNum[4];
	int		step;
	int		wait;
	int		resultcount;	//	演出用カウント
	int		Mode;			//	モード分け用変数
	int		Sy;				//	セレクト画面ポジション
	int		StringPos_Y;	//	セレクト画面の文字ポジション
	bool	Modeflg;		//	モード用フラグ
	bool	addCoinflg;		//	コイン合算値用フラグ
	bool	rankingflg[4];	//	順位用フラグ
	iex2DObj*	back;		//	背景
	iex2DObj*	r_number;	//	コインの枚数
	iex2DObj*	Sback;		//	セレクト時の背景
	iex2DObj*	Smenu;		//	セレクト時のメニュー
	iex2DObj*	result;		//	リザルト
	iexMesh*	collision;


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
	//	演出用関数
	void	Production( void );						//リザルトの演出用関数
	void	ProductionRotation( int playerNum );	//コイン枚数回転関数
	void	ProductionCoinHandOff(NUMBER& number, int coinNum);	//コイン枚数引き渡し
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
