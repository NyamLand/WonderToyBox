
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
	const		float	GRAVITY = -0.01f;
	const		int		CHARATYPE_MAX = 4;

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
		// ラストボーナステキスト
		namespace
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
		}

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
}

//	クラス
class GameManager
{
private:
	//	ゲーム設定用
	int		playerNum;
	int		charatype[4];
	int		stageType;

	//	ゲーム中情報
	int		timer;
	int		timelimit;	//	制限時間
	int		mode;
	int		coinNum[4];
	bool	donketsuBoostState;
	int		worst;
	int		lastBonus;
	bool	newsflag;    
	int        timeStop;    //画面一時停止用

public:
	Vector3	InitPos[4];
	
public:

	//	初期化・解放
	GameManager( void );
	~GameManager( void );
	bool	Initialize( void );
	bool	InitializeDebug( void );
	void	Release( void );
	void	InitImage( void );

	//	更新
	void	Update( void );

	//	描画
	void	Render( void );

	//	動作関数
	void	AddCoin( int playerNum );
	void	SubCoin( int playerNum );
	void	DecideWorst( void );
	void	LoadTextData( void );

	//	情報取得
	int		GetCharacterType( int num );
	int		GetPlayerNum( void );
	int		GetStageType( void );
	int		GetCoinNum( int num );
	bool	GetDonketsuBoostState( void );
	int		GetLastBonus( void );
	int		GetTimer( void );
	bool	GetNewsFlag( void );
	int		GetWorst( void );
	int		GetMode( void );
	int		GetRank(int player);   
	int        GetTimeStop(void);
	static	GameManager*	GetInstance( void );

	//	情報設定
	void	SetCharacterType( int num, int type );
	void	SetPlayerNum( int num );
	void	SetStageType( int type );
	void	SetCoinNum( int num, int param );
	void	SetLastBonusNews( void );
	void	SetMode( const int& modeNum );
	void	SetNewsFlag( const bool& flag );
	void    SetShakeCamera(float wide, int timer);
	void    SetTimeStop(int time);
};
#define	gameManager ( GameManager::GetInstance() )

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
