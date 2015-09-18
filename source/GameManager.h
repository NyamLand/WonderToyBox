
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

//	�N���X�O���錾
class Image;

//	�N���X
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

	//	�������E���
	GameManager( void );
	~GameManager( void );
	static	bool	Initialize( void );
	static	void	Release( void );
	static	void	InitImage( void );

	//	�X�V
	static	void	Update( void );
	static	void	MainGameUpdate( void );
	static	void	DonketsuDirectionUpdate( void );
	static	void	TimeUpUpdate( void );
	static	void	GameStartUpdate( void );
	
	//	�`��
	static	void	Render( void );
	static	void	MainGameInfoRender( void );
	static	void	DonketsuDirectionRender( void );
	static	void	TimeUpRender( void );
	static	void	NewsRender( void );
	static	void	GameStartRender( void );

	//	����֐�
	static	void	AddCoin( int playerNum );
	static	void	SubCoin( int playerNum );
	static	void	DecideWorst( void );

	//	���擾
	static	bool	GetChangeSceneFlag( void );
	static	int		GetCharacterType( int num );
	static	int		GetPlayerNum( void );
	static	int		GetStageType( void );
	static	int		GetCoinNum( int num );
	static	bool	GetDonketsuBoostState( void );
	static	int		GetWorst( void );
	static	int		GetLastBonus( void );

	//	���ݒ�
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

//*******************************************************************************
#endif // !__GAMEMANAGER_H__
