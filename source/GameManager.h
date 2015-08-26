
#ifndef __GAMEMANAGER_H__
#define	__GAMEMANAGER_H__

//*******************************************************************************
//
//	GameManagerクラス
//
//*******************************************************************************
class GameManager
{
private:
	//	定数
	static	const		int		SECOND = 60;			//	１秒
	static	const		int		MINUTE = 60 * 60;	//	１分
	static	const		int		TIMELIMIT = 1 * MINUTE + 30 * SECOND;

	enum Mode
	{
		MAINGAME, 
		TIMEUP,
	};

private:
	static	int		timer;
	static	int		waitTimer;
	static	int		mode;
	static	bool	changeSceneflag;
	static	int		charatype[4];
	static	int		playerNum;
	static	int		stageType;
	static	int		coinNum[4];
	static	bool	donketsuBoostState;

public:

	//	初期化・解放
	GameManager( void );
	~GameManager( void );
	static	bool	Initialize( void );
	static	void	Release( void );

	//	更新・描画
	static	void	Update( void );
	static	void	Render( void );
	static	void	MainGameInfoRender( void );
	static	void	TimeUpRender( void );

	//	動作関数
	static	void	MainGameUpdate( void );
	static	void	TimeUpUpdate( void );
	static	void	AddCoin( int playerNum );
	static	void	SubCoin( int playerNum );

	//	情報取得
	static	bool	GetChangeSceneFlag( void );
	static	int		GetCharacterType( int num );
	static	int		GetPlayerNum( void );
	static	int		GetStageType( void );
	static	int		GetCoinNum( int num );

	//	情報設定
	static	void	SetCharacterType( int num, int type );
	static	void	SetPlayerNum( int num );
	static	void	SetStageType( int type );
	static	void	SetCoinNum( int num, int param );
};

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
