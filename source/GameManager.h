
#ifndef __GAMEMANAGER_H__
#define	__GAMEMANAGER_H__

//*******************************************************************************
//
//	GameManagerクラス
//
//*******************************************************************************

namespace
{
	//	定数
	const		int		SECOND = 60;			//	１秒
	const		int		MINUTE = 60 * 60;	//	１分
	const		int		TIMELIMIT = 1 * MINUTE + 30 * SECOND;
	//static	const		int		TIMELIMIT = 35 * SECOND;

	namespace GAME_MODE
	{
		enum
		{
			GAMESTART,
			MAINGAME,
			DONKETSU_DIRECTION,
			TIMEUP
		};
	}
}

//	クラス前方宣言
class Image;

//	クラス
class GameManager
{
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
	static	int		worst;
	static	int		lastBonus;
	static	bool	newsflag;
	static	NewsBar	newsbar;
	static	Image*		countDown;
	static	int		count;
	

public:

	//	初期化・解放
	GameManager( void );
	~GameManager( void );
	static	bool	Initialize( void );
	static	void	Release( void );
	static	void	InitImage( void );

	//	更新
	static	void	Update( void );
	static	void	MainGameUpdate( void );
	static	void	DonketsuDirectionUpdate( void );
	static	void	TimeUpUpdate( void );
	static	void	GameStartUpdate( void );
	
	//	描画
	static	void	Render( void );
	static	void	MainGameInfoRender( void );
	static	void	DonketsuDirectionRender( void );
	static	void	TimeUpRender( void );
	static	void	NewsRender( void );
	static	void	GameStartRender( void );

	//	動作関数
	static	void	AddCoin( int playerNum );
	static	void	SubCoin( int playerNum );
	static	void	DecideWorst( void );

	//	情報取得
	static	bool	GetChangeSceneFlag( void );
	static	int		GetCharacterType( int num );
	static	int		GetPlayerNum( void );
	static	int		GetStageType( void );
	static	int		GetCoinNum( int num );
	static	bool	GetDonketsuBoostState( void );
	static	int		GetWorst( void );
	static	int		GetLastBonus( void );

	//	情報設定
	static	void	SetCharacterType( int num, int type );
	static	void	SetPlayerNum( int num );
	static	void	SetStageType( int type );
	static	void	SetCoinNum( int num, int param );
	static	void	SetLastBonusNews( void );
};

namespace GameInfo
{
	const LPSTR	LastBonus[] =
	{
		"ラストボーナス１",
		"ラストボーナス２",
		"ラストボーナス３",
		"ラストボーナス４",
		"ラストボーナス５",
		"ラストボーナス６",
	};

	const	 LPSTR	NewsText[] =
	{
		"ラストボーナス１になりそうです",
		"ラストボーナス２になりそうです",
		"ラストボーナス３になりそうです",
		"ラストボーナス４になりそうです",
	};
}

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
