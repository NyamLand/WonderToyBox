
#ifndef __UI_H__
#define	__UI_H__

//*********************************************************************************
//
//	UI�N���X
//
//*********************************************************************************

#include	"iextreme.h"
#include	"GameManager.h"
#include	"AirPlane.h"
#include	"Camera.h"
#include	<memory>

namespace
{
	namespace UI_MODE
	{
		enum
		{
			TITLE,
			MAIN,
			RESULT,
		};
	}
	
	namespace ALERT_TYPE_INFO
	{
		enum
		{
			JAM,
			COIN,
			MISSION
		};
	}

	//�@�C�x���g�̎��
	namespace EVENT_TEX_INFO
	{
		enum
		{
			C_FALL,
			C_WAVE,
			C_DUBBLE,
			J_CAMERA,
			J_SLIP,
			J_GETAWAY
		};
	}

}

//	UI�N���X
class UI
{
private:
	struct ALERT_INFO
	{
		bool	flag;
		int		timer;
		float	alpha;
		float	param;
		int		type;
	};

	struct TITLE_INFO
	{
		ImageObj	textImage;
		int		step;
		AirPlane*	airPlane;
	};

	struct DONKETSU_DIRECT_INFO
	{
		const int WAIT_MAX = 5 * SECOND + 30;

		//�@��֘A
		ImageObj	face;
		int		f;
		int		roulette;
		int		face_step;

		//�@DonketsuBoooooost!!!
		ImageObj	DB;
		int			DB_step;

		//�@�HP
		ImageObj	P;
		int			P_step;

		//�@FightLast
		ImageObj	fight;
		int			fight_step;
	};

	struct HURRY_INFO
	{
		bool	flag;
		int		timer;
		float	alpha;
		float	param;
	};

	/*struct AIRPLANE_INFO
	{
		int IN_START_POS_X;
		int IN_START_POS_Y;
		int IN_END_POS_X;
		int IN_END_POS_Y;
		int OUT_START_POS_X;
		int OUT_END_POS_X;
		int OUT_END_POS_Y;
		int ROLL_POINT_ADJUST_X;
	};*/

	//	�R�C���o�[���
	struct COINBAR_INFO
	{
		float	param[4];
		float	sub[4];
		float	scale[4];
		int		coinNum[4];
	};

	//	�R�C�������\����
	struct NUMBERIMAGE_INFO
	{
		int				scale;
		POINT			pos;
		ImageObj		hundred;			//	�R�C���O����
		ImageObj		ten;				//	�R�C���񌅖�
		ImageObj		one;				//	�R�C���ꌅ��
		bool			hundredRenderFlag;	//	�S�̈ʃ����_�[�p�t���O
	};

	//	�������
	struct NUMBER_INFO
	{
		int		hundred;		//�R�C���O����
		int		ten;			//�R�C���񌅖�
		int		one;			//�R�C���ꌅ��
		bool	H_flg;			//�S�̈ʃ����_�[�p�t���O
	};
	
	//	���C�t���
	struct LIFE_INFO
	{
		int	life;
		ImageObj	lifeImage;
	};

	//	�������
	struct CROWN_INFO
	{
		bool	state;
		ImageObj	crownImage;
	};

	//�@�C�x���g���
	struct EVENT_INFO
	{
		int mode;
		int step;
		int state;
		ImageObj	texture;	//�@��s�@
		AirPlane*	airPlane;
	};

private:
	//	�e�摜
	ImageObj	timer;
	iex2DObj*	coinbar;
	ImageObj		frame;
	ImageObj		backgauge;
	ImageObj		gauge;
	iex2DObj*	face;
	ImageObj		faceImage[PLAYER_MAX];
	ImageObj		countImage;
	ImageObj		alertImage;
	ImageObj		alert_coinImage;
	iex2DObj*	playerNumber;
	iex2DObj*	startNumber;
	ImageObj		startNum[PLAYER_MAX];
	ImageObj		pNumImage[PLAYER_MAX];
	iex2DObj*	pCoinNumImage;
	iex2DObj*	life;
	iex2DObj*	crown;
	ImageObj		roundImage;
	ImageObj	finishImage;

private:
	//	�萔
	Vector3 PAR_POS;

	//	�V�X�e��
	bool	changeflag;

	//	�J�E���g�_�E���E�X�^�[�g�E�^�C���A�b�v���o
	int		count;
	Vector3 start_pos;
	Vector3 finish_pos;
	float	start_t;
	int		start_step;
	int		waitTimer;

	//	�^�C�}�[
	int		x, y, sx, sy;
	int		time;
	int		second[2], minute;

	//	�R�C���o�[
	static	const		int		NUM_BAR = 4;
	int		frame_x, frame_y, frame_sx, frame_sy;
	int		bar_x[4], bar_y[4], bar_sx[4], bar_sy[4];
	int		state_x[4];
	int		state_type[4];

	//	�R�C������
	int		coinNum[PLAYER_MAX];
	Vector3	coinColor[PLAYER_MAX];

	//�@�L�������
	int		charatype[PLAYER_MAX];

	//	���X�g����
	bool	last_state;
	float	last_alpha;
	float	last_t;

	//	�j���[�X
	NewsBar	newsbar;

	//	�x���p�����[�^
	ALERT_INFO	alertInfo;

	//	�^�C�g���p�����[�^
	TITLE_INFO	titleInfo;

	//�@�ǂ񂯂��o�p�����[�^
	DONKETSU_DIRECT_INFO ddInfo;

	//	HurryUp�p�����[�^
	HURRY_INFO	hurryInfo;

	//	��s�@�p�����[�^
	//AIRPLANE_INFO		airPlaneInfo;

	//	�R�C���o�[���
	COINBAR_INFO	coinBarInfo;

	//	���C�t���
	LIFE_INFO			lifeInfo[4];

	//	�������
	CROWN_INFO			crownInfo[4];

	//	�R�C���������
	NUMBERIMAGE_INFO	coinNumInfo[PLAYER_MAX];
	NUMBER_INFO			numInfo[PLAYER_MAX];

	//�@�C�x���g���
	EVENT_INFO	eventInfo;
	
	//	�p�[�e�B�N���p�o�b�N�o�b�t�@
	unique_ptr<iex2DObj>	target_par;

	//	�p�[�e�B�N���p
	bool		coin_flg[PLAYER_MAX];
	int			coin_timer[PLAYER_MAX];

	//	�p�[�e�B�N���p�J����
	unique_ptr<Camera>		particle_camera;
		
	//	�p�����[�^
	int		scene;
	
public:
	//------------------------------���ʏ���------------------------------------//
	//	�������E���
	UI( void );
	~UI( void );
	bool	Initialize( int scene );
	void	Release( void );

	//	�e�V�[��������
	void	TitleInitialize( void );
	void	MainInitialize( void );
	void	ResultInitialize( void );
	
	//	�e�V�[�����
	void	TitleRelease( void );
	void	MainRelease( void );
	void	ResultRelease( void );

	//	�X�V�E�`��
	void	Update( const int& mode );
	void	Render( const int& mode );
	void	DrawDebug();

	//	�e�V�[���X�V
	void	TitleUpdate( int mode );
	void	MainUpdate( int mode );
	void	ResultUpdate( int mode );

	//	�e�V�[���`��
	void	TitleRender( int mode );
	void	MainRender( int mode );
	void	ResultRender( int mode );

	//------------------------------�^�C�g������------------------------------------//
	bool	FlyingIn( void );
	bool	FlyingOut( int startPos );

	//-------------------------------���C������-------------------------------------//
	//	���C�����쏉����
	void	CoinBarInitialize( void );
	void	TimerInitialize( void );
	void	NewsBarInitialize( void );
	void	StartAndTimeUpInitialize( void );
	void	StartPlayerNumInitialize( void );
	void	DonketsuDirectionInitialize( void );
	void	AlertInitialize( void );
	void	HurryUpInitialize( void );
	void	LastProductionInitialize( void );
	void	PlayerNumberInitialize( void );
	void	LifeInitialize( void );
	void	CrownInitialize( void );
	void	CoinNumberInitialize( void );
	void	RoundInitialize( void );
	void	EventInitialize( void );

	//	���C������X�V
	void	TimerUpdate( void );
	void	CoinBarUpdate( void );
	void	NewsBarUpdate( void );
	void	StartUpdate( void );
	bool	StartMove( void );
	void	StartPlayerNumUpdate( void );
	void	FinishUpdate( void );
	void	DonketsuDirectionUpdate( void );
	void	AlertUpdate( void );
	void	MissionDirectionUpdate( void );
	void	HurryUpdate( void );
	void	LastProduction( void );
	void	PlayerNumberUpdate( void );
	void	LifeUpdate( void );
	void	CrownUpdate( void );
	void	CoinNumberUpdate( void );
	void	FaceImageUpdate( int num, int mode );
	void	CoinImageInfoUpdate(NUMBERIMAGE_INFO& numImageinfo, NUMBER_INFO& numinfo, const int& num);
	void	EventUpdate( void );
	void	ParticleUpdate( void );

	//	���C���`��
	void	TimerRender( void );
	void	CoinBarRender( void );
	void	NewsBarRender( void );
	void	StartRender( void );
	void	StartPlayerNumRender( void );
	void	FinishRender( void );
	void	DonketsuDirectionRender( void );
	void	AlertRender( void );
	void	MissionDirectionRender( void );
	void	LastProductionRender( void );
	void	PlayerNumberRender( void );
	void	LifeRender( void );
	void	CrownRender( void );
	void	CoinNumberRender( void );
	void	EventRender( void );
	void	ParticleRender( int value );
	void	RenderTargetParticle( void );

	//	���C������֐�
	void BarControl( void );
	void StateImageControl( void );
	void FaceRoulette( int face_wait );
	void DB_Direction( int wait );
	void P_Direction( int wait );
	void Fight_Direction( int wait );
	void CoinCounter( int coin , int num);
	void	RoundRender( void );

	//	�e�X�g�p
	void	BarControl2( void );
	void	BarRender( void );
	//------------------------------���擾�E�ݒ�------------------------------------//

	//	���ݒ�
	void	SetChangeFlag( const bool& flag );
	void	SetAlertInfo( bool flag, int type );
	void	SetHurryFlag( bool flag );
	void	SetFlyingIn( int type );
	void	SetImageSrcPos( int mode );
	void	SetImageSrcPos( int sx, int sy );
	void	SetCoinImageInfo(NUMBERIMAGE_INFO& numImageinfo, NUMBER_INFO& numinfo, const int& num);
	void	SetNumberInfo(NUMBER_INFO& nomber, int coin);
	void	SetEventInfoMode(int mode);

	//	���擾
	bool	GetChangeFlag( void );
	bool	GetMoveState( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__