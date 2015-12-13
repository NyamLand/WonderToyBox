
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
	//const		int		COIN_MAX = 50;

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
}

//	�N���X
class GameManager
{
private:
	//�I�v�V�������
	int		coinMax;
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
	int		GetCharacterType( int num );
	int		GetPlayerNum( void );
	int		GetStageType( void );
	int		GetCoinNum( int num );
	bool		GetDonketsuBoostState( void );
	int		GetLastBonus( void );
	int		GetTimer( void );
	bool		GetNewsFlag( void );
	int		GetWorst( void );
	int		GetMode( void );
	int		GetRank(int player);   
	int		GetTimeStop(void);
	int		GetCoinMax(void)const;
	int		GetStartLife( int player )const;
	int		GetRound( void )const;
	bool		GetItemFlg(void)const;
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
	void	SetCoinMax(int coinmax);
	void	SetItemFlg(bool itemflg);
	void	SetRound( int round );
	void	SetStartLife( int player, int life );
};
#define	gameManager ( GameManager::GetInstance() )

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
