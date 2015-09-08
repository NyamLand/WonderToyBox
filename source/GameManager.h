
#ifndef __GAMEMANAGER_H__
#define	__GAMEMANAGER_H__

//*******************************************************************************
//
//	GameManagerƒNƒ‰ƒX
//
//*******************************************************************************
class GameManager
{
private:
	//	’è”
	static	const		int		SECOND = 60;			//	‚P•b
	static	const		int		MINUTE = 60 * 60;	//	‚P•ª
	//static	const		int		TIMELIMIT = 1 * MINUTE + 30 * SECOND;
	static	const		int		TIMELIMIT = 35 * SECOND;
	enum Mode
	{
		MAINGAME, 
		DONKETSU_DIRECTION,
		TIMEUP
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
	static	int		worst;

public:

	//	‰Šú‰»E‰ğ•ú
	GameManager( void );
	~GameManager( void );
	static	bool	Initialize( void );
	static	void	Release( void );

	//	XVE•`‰æ
	static	void	Update( void );
	static	void	Render( void );
	static	void	MainGameUpdate( void );
	static	void	MainGameInfoRender( void );
	static	void	DonketsuDirectionUpdate( void );
	static	void	DonketsuDirectionRender( void );
	static	void	TimeUpUpdate( void );
	static	void	TimeUpRender( void );

	//	“®ìŠÖ”
	static	void	AddCoin( int playerNum );
	static	void	SubCoin( int playerNum );
	static	void	DecideWorst( void );

	//	î•ñæ“¾
	static	bool	GetChangeSceneFlag( void );
	static	int		GetCharacterType( int num );
	static	int		GetPlayerNum( void );
	static	int		GetStageType( void );
	static	int		GetCoinNum( int num );
	static	bool	GetDonketsuBoostState( void );
	static	int		GetWorst( void );

	//	î•ñİ’è
	static	void	SetCharacterType( int num, int type );
	static	void	SetPlayerNum( int num );
	static	void	SetStageType( int type );
	static	void	SetCoinNum( int num, int param );
};

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
