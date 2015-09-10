
#ifndef __GAMEMANAGER_H__
#define	__GAMEMANAGER_H__

//*******************************************************************************
//
//	GameManager�N���X
//
//*******************************************************************************

//	�j���[�X�\����
struct NewsBar
{
	int		left;
	int		top;
	int		right;
	int		bottom;
	float	alpha;
	Vector3	color;
	LPSTR	text;
	int		textleft;
	int		step;
};

class GameManager
{
private:
	//	�萔
	static	const		int		SECOND = 60;			//	�P�b
	static	const		int		MINUTE = 60 * 60;	//	�P��
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
	static	int		lastBonus;
	static	bool	newsflag;
	static	NewsBar	newsbar;
public:

	//	�������E���
	GameManager( void );
	~GameManager( void );
	static	bool	Initialize( void );
	static	void	Release( void );

	//	�X�V�E�`��
	static	void	Update( void );
	static	void	Render( void );
	static	void	MainGameUpdate( void );
	static	void	MainGameInfoRender( void );
	static	void	DonketsuDirectionUpdate( void );
	static	void	DonketsuDirectionRender( void );
	static	void	TimeUpUpdate( void );
	static	void	TimeUpRender( void );
	static	void	NewsRender( void );

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
