
#ifndef __UI_H__
#define	__UI_H__

//*********************************************************************************
//
//	UI�N���X
//
//*********************************************************************************

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
	};

	struct TITLE_INFO
	{
		ImageObj	textImage;
		float	param;
		float	t;
		int		step;
		int		mode;
		bool	moveState;
		int		savePos;
	};

	struct HURRY_INFO
	{
		bool	flag;
		int		timer;
		float	alpha;
		float	param;
	};

private:
	//	�e�摜
	ImageObj	timer;
	iex2DObj*	coinbar;
	ImageObj	frame;
	ImageObj	backgauge;
	ImageObj	gauge;
	iex2DObj*	face;
	ImageObj	faceImage;
	ImageObj	countImage;
	ImageObj	alertImage;
	iex2DObj*	playerNumber;
	ImageObj	pNumImage[PLAYER_MAX];

private:
	//	�V�X�e��
	bool	changeflag;

	//	�J�E���g�_�E���E�X�^�[�g�E�^�C���A�b�v���o
	int		count;
	int		waitTimer;

	//	�^�C�}�[
	int		x, y, sx, sy;
	int		time;
	int		second, minute[2];

	//	�R�C���o�[
	static	const		int		NUM_BAR = 4;
	int		frame_x, frame_y, frame_sx, frame_sy;
	int		bar_x[4], bar_y[4], bar_sx[4], bar_sy[4];
	int		state_x[4];
	int		state_type[4];

	//	�j���[�X
	NewsBar	newsbar;
	
	//�@�v���C���[�i��j
	int		charatype[4];
	int		f;
	int		roulette;

	//	�x���p�����[�^
	ALERT_INFO	alertInfo;

	//	�^�C�g���p�����[�^
	TITLE_INFO	titleInfo;

	//	HurryUp�p�����[�^
	HURRY_INFO	hurryInfo;

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
	void	DonketsuDirectionInitialize( void );
	void	AlertInitialize( void );
	void	HurryUpInitialize( void );
	void	PlayerNumberInitialize( void );

	//	���C������X�V
	void	TimerUpdate( void );
	void	CoinBarUpdate( void );
	void	NewsBarUpdate( void );
	void	StartUpdate( void );
	void	FinishUpdate( void );
	void	DonketsuDirectionUpdate( void );
	void	AlertUpdate( void );
	void	HurryUpdate( void );
	void	LastProduction( void );
	void	PlayerNumberUpdate( void );

	//	���C���`��
	void	TimerRender( void );
	void	CoinBarRender( void );
	void	NewsBarRender( void );
	void	StartRender( void );
	void	FinishRender( void );
	void	DonketsuDirectionRender( void );
	void	AlertRender( void );
	void	LastProductionRender( void );
	void	PlayerNumberRender( void );

	//	���C������֐�
	void BarControl( void );
	void StateImageControl( void );

	//------------------------------���擾�E�ݒ�------------------------------------//

	//	���ݒ�
	void	SetChangeFlag( const bool& flag );
	void	SetAlertFlag( bool flag );
	void	SetHurryFlag( bool flag );
	void	SetFlyingIn( int type );
	void	SetImageSrcPos( int mode );
	void	SetImageSrcPos( int sx, int sy );

	//	���擾
	bool	GetChangeFlag( void );
	bool	GetMoveState( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__