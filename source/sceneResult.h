
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
	RESULT_INFO	resultInfo[4];
	
	//	ソート用
	char	str[256];

	//	パラメータ
	int		coinNum[4];
	int		lastBonus;
	int		step;
	int		playerNum;
	int		wait;
	int		ten[4];	//コイン二桁目
	int		one[4];	//コイン一桁目
	int		resultcount;	//	演出用カウント
	iex2DObj*	back; //背景
	iex2DObj*	r_number; //コインの枚数
	iexMesh*	collision;

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
	//	演出用関数
	void	Production( void );	
	void	Production_Rotation( int start );
	void	Production_Coin_hand_off(int chara);
};
//*******************************************************************************
#endif // !__SCENERESULT_H__
