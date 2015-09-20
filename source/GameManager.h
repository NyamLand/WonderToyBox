
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
	//const		int		TIMELIMIT = 1 * MINUTE + 30 * SECOND;
	const		int		TIMELIMIT = 40 * SECOND;
	const		int		PLAYER_NUM = 4;

	namespace GAME_MODE
	{
		enum
		{
			GAMESTART,
			MAINGAME,
			DONKETSU_DIRECTION,
			CLIMAX,
			TIMEUP,
		};
	}
	
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
}

//	クラス
class GameManager
{
private:
	//	ゲーム設定用
	static	int		playerNum;
	static	int		charatype[4];
	static	int		stageType;

	//	ゲーム中情報
	static	int		timer;
	static	int		mode;
	static	int		coinNum[4];
	static	bool	donketsuBoostState;
	static	int		worst;
	static	int		lastBonus;
	static	bool	newsflag;
	
public:

	//	初期化・解放
	GameManager( void );
	~GameManager( void );
	static	bool	Initialize( void );
	static	void	Release( void );
	static	void	InitImage( void );

	//	更新
	static	void	Update( void );

	//	描画
	static	void	Render( void );

	//	動作関数
	static	void	AddCoin( int playerNum );
	static	void	SubCoin( int playerNum );
	static	void	DecideWorst( void );

	//	情報取得
	static	int		GetCharacterType( int num );
	static	int		GetPlayerNum( void );
	static	int		GetStageType( void );
	static	int		GetCoinNum( int num );
	static	bool	GetDonketsuBoostState( void );
	static	int		GetLastBonus( void );
	static	int		GetTimer( void );
	static	bool	GetNewsFlag( void );
	static	int		GetWorst( void );
	static	int		GetMode( void );

	//	情報設定
	static	void	SetCharacterType( int num, int type );
	static	void	SetPlayerNum( int num );
	static	void	SetStageType( int type );
	static	void	SetCoinNum( int num, int param );
	static	void	SetLastBonusNews( void );
	static	void	SetMode( const int& modeNum );
	static	void	SetNewsFlag( const bool& flag );
};


//*******************************************************************************
#endif // !__GAMEMANAGER_H__
