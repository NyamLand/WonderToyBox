
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
	const		int		TIMELIMIT = 3 * SECOND;
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
		const LPSTR	LastBonus[] =
		{
			"���X�g�{�[�i�X�P",
			"���X�g�{�[�i�X�Q",
			"���X�g�{�[�i�X�R",
			"���X�g�{�[�i�X�S",
			"���X�g�{�[�i�X�T",
			"���X�g�{�[�i�X�U",
		};

		const	 LPSTR	NewsText[] =
		{
			"���X�g�{�[�i�X�P�ɂȂ肻���ł�",
			"���X�g�{�[�i�X�Q�ɂȂ肻���ł�",
			"���X�g�{�[�i�X�R�ɂȂ肻���ł�",
			"���X�g�{�[�i�X�S�ɂȂ肻���ł�",
		};
	}
}

//	�N���X
class GameManager
{
private:
	//	�Q�[���ݒ�p
	int		playerNum;
	int		charatype[4];
	int		stageType;

	//	�Q�[�������
	int		timer;
	int		mode;
	int		coinNum[4];
	bool	donketsuBoostState;
	int		worst;
	int		lastBonus;
	bool	newsflag;
	
public:

	//	�������E���
	GameManager( void );
	~GameManager( void );
	bool	Initialize( void );
	void	Release( void );
	void	InitImage( void );

	//	�X�V
	void	Update( void );

	//	�`��
	void	Render( void );

	//	����֐�
	void	AddCoin( int playerNum );
	void	SubCoin( int playerNum );
	void	DecideWorst( void );

	//	���擾
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
	static	GameManager*	GetInstance( void );

	//	���ݒ�
	void	SetCharacterType( int num, int type );
	void	SetPlayerNum( int num );
	void	SetStageType( int type );
	void	SetCoinNum( int num, int param );
	void	SetLastBonusNews( void );
	void	SetMode( const int& modeNum );
	void	SetNewsFlag( const bool& flag );
};
#define	gameManager ( GameManager::GetInstance() )

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
