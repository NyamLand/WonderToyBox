
#ifndef __SCENERESULT_H__
#define	__SCENERESULT_H__

//*******************************************************************************
//
//	sceneResultクラス
//
//*******************************************************************************
class sceneResult : public	Scene
{
private:
	struct ORIGIN_INFO
	{
		int		coinNum[4];
		int		rank[4];
	};

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
		bool	H_flg;			//百の位レンダー用フラグ
	};

	struct RANK
	{
		int		rank;
		bool	rankflg;		//	順位用フラグ
	};
private:
	RESULT_INFO	resultInfo[4];
	NUMBER		number[4];
	NUMBER		BonusNumber[4];
	NUMBER		totalNumber[4];
	RANK		rank[4];

	//	ソート用
	char	str[256];

	//	パラメータ
	int		coinNum[4];
	int		totalCoinNum[4];
	int		playerNum[4];
	int		lastBonus;
	int		lastBonusNum[4];
	int		step;
	int		wait;
	int		resultcount;	//	演出用カウント
	int		waitTimer;		//	待機用タイマー
	int		mode;			//	モード分け用変数
	int		Sy;				//	セレクト画面ポジション
	int		StringPos_Y;	//	セレクト画面の文字ポジション
	
	//	フラグ関係
	bool	Modeflg;		//	モード用フラグ
	bool	bonusflg;		//	ボーナス値用フラグ
	bool	addCoinflg;		//	コイン合算値用フラグ


	//	画像データ関係
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

	void	ResultRender(NUMBER& number, Vector3 Pos);
	void	RankRender(int ranking);
	void	SelectRender( void );
	
	//	動作関数
	void	BubbleSort( void );
	void	SetLastBonus( void );

	//	演出用関数
	void	SetRank( void );
	void	Production( void );						//リザルトの演出用関数
	void	ProductionRotation( int playerNum );	//コイン枚数回転関数
	void	ProductionCoinHandOff(NUMBER& number, int coinNum);	//コイン枚数引き渡し
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
