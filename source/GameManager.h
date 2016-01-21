
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
	const		int		TIMELIMIT = 3* SECOND;
	const		int		PLAYER_MAX = 4;
	const		float		GRAVITY = -0.01f;
	const		int		CHARATYPE_MAX = 4;
	const		int		SHAKE_TIME = 30;
	const		float	SHAKE_POWER = 1.5f;
	const		int		SCREEN_STOPTIME = 40;

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
		//	ラストボーナス予想テキスト
		namespace
		{
			const	 LPSTR	NewsText[] =
			{
				"ラストボーナス１になりそうです",
				"ラストボーナス２になりそうです",
				"ラストボーナス３になりそうです",
				"ラストボーナス４になりそうです",
			};
		}
	}

	namespace Round
	{
		enum
		{
			ROUND1,
			ROUND2,
			ROUND_FINAL,
			END,
		};
	}

	namespace LIFE_MAX_NUM
	{
		enum
		{
			LIFE_3,
			LIFE_4,
			LIFE_5,
			END,
		};
	}
}

//	クラス
class GameManager
{
private:
	//オプション情報
	int		maxlife;
	bool	itemflg;

	//	ゲーム設定用
	int		startLife[4];
	int		playerNum;
	int		charatype[4];
	int		stageType;

	//	ゲーム中情報
	int		timer;
	int		timelimit;		//	制限時間
	int		mode;
	int		coinNum[4];
	bool	donketsuBoostState;
	int		worst;
	int		lastBonus;
	bool	newsflag;    
	int    timeStop;			//画面一時停止用
	int	round;				//	ラウンド
	int	totalCoin[3][4];	//	ラウンドごとのコイン合計値
	bool	coinCollision;	//	trueでON,falseでOFF

	//	ラストボーナス用カウント
	int	maxCoinNum[4];
	int	fallStageNum[4];
	int	coin77[4]; 
	int	minCoinNum[4];
	int	hitAttackNum[4];

public:	
		Vector3	InitPos[4];
public:
	
private:
	//	初期化・解放
	GameManager( void );
	~GameManager( void );

public:
	bool	Initialize( void );
	bool	InitializeDebug( void );
	void	Release( void );
	void	RetryInitialize( void );

	//	更新
	void	Update( void );

	//	描画
	void	Render( void );

	//	動作関数
	void	AddCoin( int playerNum );
	void	SubCoin( int playerNum );
	void	DecideWorst( void );
	void	LoadTextData( void );
	
	//	ラストボーナス用カウント
	void	CalcMaxCoin( int player );
	void	CalcSubCoin77( int player );
	void	AddFallStage( int player );
	void	AddTotalCoinNum( int player );
	void	AddHitAttackCount( int player );

	//	情報取得
	int		GetCharacterType( int num )const;
	int		GetPlayerNum( void )const;
	int		GetStageType( void )const;
	int		GetCoinNum( int num )const;
	bool		GetDonketsuBoostState( void )const;
	int		GetLastBonus( void )const;
	int		GetTimer( void )const;
	bool		GetNewsFlag( void )const;
	int		GetWorst( void )const;
	int		GetMode( void )const;
	int		GetRank(int player)const;   
	int		GetTimeStop(void)const;
	int		GetMaxLife(void)const;
	int		GetStartLife( int player )const;
	int		GetRound( void )const;
	int		GetTotalCoin( int round, int player )const;
	bool		GetItemFlg( void )const;
	bool		GetCoinCollision( void )const;
	static	GameManager*	GetInstance( void );

	//	ラストボーナス用情報取得
	int	GetMaxCoinNum( int player )const;
	int	GetFallStageNum( int player )const;
	int	GetSubCoin77( int player )const;
	int	GetTotalCoinNum( int player )const;
	int	GetHitAttackNum( int player )const;

	//	情報設定
	void	SetCharacterType( int num, int type );
	void	SetPlayerNum( int num );
	void	SetStageType( int type );
	void	SetCoinNum( int num, int param );
	void	SetLastBonusNews( void );
	void	SetMode( const int& modeNum );
	void	SetNewsFlag( const bool& flag );
	void	SetShakeCamera(float wide, int timer);
	void	SetTimeStop(int time);
	void	SetTime(int minute, int second);
	void	SetMaxLife(int life);
	void	SetItemFlg(bool itemflg);
	void	SetRound( int round );
	void	SetStartLife( int player, int life );
	void	SetTotalCoin( int round, int player, int total );
	void	SetCoinCollision( bool state );
};
#define	gameManager ( GameManager::GetInstance() )

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
