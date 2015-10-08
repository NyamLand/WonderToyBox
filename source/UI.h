
#ifndef __UI_H__
#define	__UI_H__

//*********************************************************************************
//
//	UI�N���X
//
//*********************************************************************************

//	UI�N���X
class UI
{
private:
	//	�e�摜
	iex2DObj*	timer;
	iex2DObj*	coinbar;
	iex2DObj*	face;
	ImageObj	countImage;

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
	
public:
	//	�������E���
	UI( void );
	~UI( void );
	bool	Initialize( void );
	void	CoinBarInitialize( void );
	void	TimerInitialize( void );
	void	NewsBarInitialize( void );
	void	StartAndTimeUpInitialize( void );
	void	DonketsuDirectionInitialize( void );
	void	Release( void );

	//	�X�V
	void	Update( const int& mode );
	void	TimerUpdate( void );
	void	CoinBarUpdate( void );
	void	NewsBarUpdate( void );
	void	StartUpdate( void );
	void	FinishUpdate( void );
	void	DonketsuDirectionUpdate( void );

	//	�`��
	void	Render( const int& mode );
	void	TimerRender( void );
	void	CoinBarRender( void );
	void	NewsBarRender( void );
	void	StartRender( void );
	void	FinishRender( void );
	void	DonketsuDirectionRender( void );

	//	����֐�
	void BarControl( void );
	void StateImageControl( void );

	//	���擾�E�ݒ�
	void	SetChangeFlag( const bool& flag );
	bool	GetChangeFlag( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__