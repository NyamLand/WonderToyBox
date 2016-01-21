
#ifndef __GAMEMANAGER_H__
#define	__GAMEMANAGER_H__

//*******************************************************************************
//
//	GameManager�N���X
//
//*******************************************************************************

namespace
{
	//	�萔
	const		int		SECOND = 60;			//	�P�b
	const		int		MINUTE = 60 * 60;	//	�P��
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
		//	���X�g�{�[�i�X�\�z�e�L�X�g
		namespace
		{
			const	 LPSTR	NewsText[] =
			{
				"���X�g�{�[�i�X�P�ɂȂ肻���ł�",
				"���X�g�{�[�i�X�Q�ɂȂ肻���ł�",
				"���X�g�{�[�i�X�R�ɂȂ肻���ł�",
				"���X�g�{�[�i�X�S�ɂȂ肻���ł�",
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

//	�N���X
class GameManager
{
private:
	//�I�v�V�������
	int		maxlife;
	bool	itemflg;

	//	�Q�[���ݒ�p
	int		startLife[4];
	int		playerNum;
	int		charatype[4];
	int		stageType;

	//	�Q�[�������
	int		timer;
	int		timelimit;		//	��������
	int		mode;
	int		coinNum[4];
	bool	donketsuBoostState;
	int		worst;
	int		lastBonus;
	bool	newsflag;    
	int    timeStop;			//��ʈꎞ��~�p
	int	round;				//	���E���h
	int	totalCoin[3][4];	//	���E���h���Ƃ̃R�C�����v�l
	bool	coinCollision;	//	true��ON,false��OFF

	//	���X�g�{�[�i�X�p�J�E���g
	int	maxCoinNum[4];
	int	fallStageNum[4];
	int	coin77[4]; 
	int	minCoinNum[4];
	int	hitAttackNum[4];

public:	
		Vector3	InitPos[4];
public:
	
private:
	//	�������E���
	GameManager( void );
	~GameManager( void );

public:
	bool	Initialize( void );
	bool	InitializeDebug( void );
	void	Release( void );
	void	RetryInitialize( void );

	//	�X�V
	void	Update( void );

	//	�`��
	void	Render( void );

	//	����֐�
	void	AddCoin( int playerNum );
	void	SubCoin( int playerNum );
	void	DecideWorst( void );
	void	LoadTextData( void );
	
	//	���X�g�{�[�i�X�p�J�E���g
	void	CalcMaxCoin( int player );
	void	CalcSubCoin77( int player );
	void	AddFallStage( int player );
	void	AddTotalCoinNum( int player );
	void	AddHitAttackCount( int player );

	//	���擾
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

	//	���X�g�{�[�i�X�p���擾
	int	GetMaxCoinNum( int player )const;
	int	GetFallStageNum( int player )const;
	int	GetSubCoin77( int player )const;
	int	GetTotalCoinNum( int player )const;
	int	GetHitAttackNum( int player )const;

	//	���ݒ�
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
