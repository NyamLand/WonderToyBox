
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
	const		float		SHAKE_POWER = 1.5f;
	const		int		SCREEN_STOPTIME = 40;
	const		int		EVENT_INTERVAL = 20 * SECOND;
	const		int		EVENT_TIMING = 15 * SECOND;

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
	bool		itemflg;

	//	ゲーム設定用
	int		startLife[PLAYER_MAX];
	int		playerNum;
	int		charatype[PLAYER_MAX];
	int		stageType;

	//	ゲーム中情報
	int		timer;
	int		timelimit;		//	制限時間
	int		mode;
	int		coinNum[PLAYER_MAX];
	bool		donketsuBoostState;
	int		worst;
	int		lastBonus;
	bool		newsflag;    
	int		timeStop;			//画面一時停止用
	int		round;				//	ラウンド
	int		totalCoin[Round::END][PLAYER_MAX];	//	ラウンドごとのコイン合計値
	bool		coinCollision;	//	trueでON,falseでOFF
	int		eventmode;
	bool		absCoinEventFlag;	//　絶対にコインイベントを発生させるか
	Vector3	playerColor[PLAYER_MAX];
	
public:	
		Vector3	InitPos[PLAYER_MAX];

private:
	//	定数
	enum TIMER_EVENT
	{
		TIME_UP,
		MOVE_CLIMAX = 30 * SECOND,
	};
	
private:
	//	初期化・解放
	GameManager( void );
	~GameManager( void );

public:
	bool	Initialize( void );
	void	Release( void );
	void	RetryInitialize( void );

	//	更新
	void	Update( void );

	//	描画
	void	Render( void );

	//	動作関数
	void	TimerUpdate( void );
	void	TimeUp( void );
	void	AddCoin( int playerNum );
	void	SubCoin( int playerNum );
	void	DecideWorst( void );
	void	EventManagement( void );
	void	ItemAndCoinManagement( void );

	//	情報取得
	Vector3	GetPlayerColor( int playerNum )const;
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

	//	情報設定
	void	SetCharacterType( int num, int type );
	void	SetPlayerNum( int num );
	void	SetStageType( int type );
	void	SetCoinNum( int num, int param );
	void	SetLastBonusNews( void );
	void	SetMode( const int& modeNum );
	void	SetNewsFlag( const bool& flag );
	void	SetShakeCamera(float wide, int timer);
	void	SetTimeStop( int time );
	void	SetTime( int minute, int second );
	void	SetMaxLife( int life );
	void	SetItemFlg( bool itemflg );
	void	SetRound( int round );
	void	SetStartLife( int player, int life );
	void	SetTotalCoin( int round, int player, int total );
	void	SetCoinCollision( bool state );
};
#define	gameManager ( GameManager::GetInstance() )

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
