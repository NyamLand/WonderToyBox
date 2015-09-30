
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

private:
	iexView*	view;
	RESULT_INFO	resultInfo[4];
	
	//	ソート用
	char	str[256];

	//	パラメータ
	int		coinNum[4];
	int		lastBonus;
	int		step;
	int		playerNum;
	int		wait;
	iex2DObj*	back; //背景
	PlayerManager	chara[4]; //キャラ情報

public:
	//	初期化・解放
	sceneResult( void );
	~sceneResult( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	BubbleSort( void );
	void	SetLastBonus( void );
};

//*******************************************************************************
#endif // !__SCENERESULT_H__
