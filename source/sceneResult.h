
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

	struct NUMBER_INFO
	{
		int		hundred;		//コイン三桁目
		int		ten;			//コイン二桁目
		int		one;			//コイン一桁目
		bool	H_flg;			//百の位レンダー用フラグ
	};

	//	数字描画情報
	struct NUMBER_DRAW_INFO
	{
		NUMBER_INFO	numInfo;		//	数字情報
		POINT		hundredPos;		//	百の位の座標
		POINT		tenPos;			//	十の位の座標
		POINT		onePos;			//	一の位の座標
		POINT		center;			//	中心座標（十の位の座標）
		float		length;			//	中心からの距離
	};
	struct RANK
	{
		int		rank;
		bool	rankflg;		//	順位用フラグ
	};

private:
	RESULT_INFO	resultInfo[4];
	NUMBER_INFO	number[4];
	NUMBER_INFO	BonusNumber[4];
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
	

	//	リザルト描画位置
	Vector3 stringPos[4];
	Vector3	bonusPos[4];
	Vector3 addcoinPos[4];
	Vector3 rankingPos[4];

	//	フラグ関係
	bool	Modeflg;		//	モード用フラグ
	bool	bonusflg;		//	ボーナス値用フラグ
	bool	addCoinflg;		//	コイン合算値用フラグ

	//	画像データ関係
	ImageObj	back;		//	背景
	ImageObj	r_number;	//	コインの枚数
	ImageObj	Sback;		//	セレクト時の背景
	ImageObj	Smenu;		//	セレクト時のメニュー
	ImageObj	result;		//	リザルト
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

	void	ResultRender( NUMBER_INFO& number, Vector3 Pos );
	void	RankRender(int ranking);
	void	SelectRender( void );
	
	//	動作関数
	void	BubbleSort( void );
	void	SetLastBonus( void );

	//	演出用関数
	void	SetRank( void );
	void	Production( void );						//リザルトの演出用関数
	void	ProductionRotation( int playerNum );	//コイン枚数回転関数
	void	ProductionCoinHandOff( NUMBER_INFO& number, int coinNum );	//コイン枚数引き渡し
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
