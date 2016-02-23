
#ifndef		__UI_H__
#define	__UI_H__

//*********************************************************************************
//
//	UI�N���X
//
//*********************************************************************************

#include	"iextreme.h"
#include	"GameManager.h"
#include	"AirPlane.h"
#include	"LifeUI.h"
#include	"Crown.h"
#include	"Camera.h"
#include	"CoinUIEffect.h"
#include	"CoinNumUI.h"
#include	"FallLife.h"
#include	<memory>

namespace
{	
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
	//	�x��
	struct AlertInfo
	{
		bool	flag;
		int		timer;
		float	alpha;
		float	param;
		int		type;
	};

	//	�R�C�������\����
	struct NumberImageInfo
	{
		int				scale;
		POINT			pos;
		ImageObj		hundred;			//	�R�C���O����
		ImageObj		ten;				//	�R�C���񌅖�
		ImageObj		one;				//	�R�C���ꌅ��
		bool			hundredRenderFlag;	//	�S�̈ʃ����_�[�p�t���O
	};

	//	�������
	struct NumberInfo
	{
		int		hundred;	//�R�C���O����
		int		ten;			//�R�C���񌅖�
		int		one;			//�R�C���ꌅ��
		bool		H_flg;		//�S�̈ʃ����_�[�p�t���O
	};

	//�@�C�x���g���
	struct EventInfo
	{
		int mode;
		int step;
		int state;
		ImageObj	texture;	//�@��s�@
		AirPlane*	airPlane;
	};

	//	�^�C�}�[���
	struct TimerInfo
	{
		int		x, y, sx, sy;
		int		time;
		int		second[2], minute;
	};

	//	���X�g�^�C�}�[���
	struct LastTimerInfo
	{
		bool	state;
		float	alpha;
		float	t;
	};

	//	�J�E���g�_�E�����
	struct CountInfo
	{
		int		count;
		Vector3 start_pos;
		Vector3 finish_pos;
		float	start_t;
		int		start_step;
		int		waitTimer;
	};

private:
	//	�e�摜
	ImageObj	timer;
	ImageObj	faceImage[PLAYER_MAX];
	ImageObj	countImage;
	ImageObj	alertImage;
	ImageObj	alert_coinImage;
	ImageObj	startNum[PLAYER_MAX];
	ImageObj	pNumImage[PLAYER_MAX];
	ImageObj	roundImage;
	ImageObj	finishImage;
	iex2DObj*	face;
	iex2DObj*	playerNumber;
	iex2DObj*	startNumber;
	iex2DObj*	pCoinNumImage;

private:
	//	�萔
	Vector3 PAR_POS;

	//	�V�X�e��
	bool	changeflag;

	//	�J�E���g�_�E���E�X�^�[�g�E�^�C���A�b�v���o
	CountInfo			countInfo;

	//	�R�C������
	int		coinNum[PLAYER_MAX];
	Vector3	coinColor[PLAYER_MAX];

	//�@�L�������
	int		charatype[PLAYER_MAX];

	//	���X�g����
	LastTimerInfo		lasttimerInfo;
	
	//	�^�C�}�[
	TimerInfo			timerInfo;
	
	//	�x���p�����[�^
	AlertInfo			alertInfo;

	//	���C�t
	LifeUI*			lifeUI;

	//	�������C�t
	FallLife*		fallLife;

	//	����
	Crown*		crown;

	//	�R�C���������
	NumberImageInfo	coinNumInfo[PLAYER_MAX];
	NumberInfo			numInfo[PLAYER_MAX];

	//�@�C�x���g���
	EventInfo			eventInfo;

	//	�p�[�e�B�N���p
	bool		coin_flg[PLAYER_MAX];
	int			coin_timer[PLAYER_MAX];
	
	//	�p�[�e�B�N���p�o�b�N�o�b�t�@
	std::unique_ptr<iex2DObj>	target_par;

	//	�p�[�e�B�N���p�J����
	std::unique_ptr<Camera>		particle_camera;
		
	//	�p�����[�^
	int		scene;
	
public:
	//	�������E���
	UI( void );
	~UI( void );
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( const int& mode );
	void	Render( const int& mode );

	//-------------------------------���C������-------------------------------------//
	//	���C�����쏉����
	void	TimerInitialize( void );
	void	StartAndTimeUpInitialize( void );
	void	StartPlayerNumInitialize( void );
	void	AlertInitialize( void );
	void	LastProductionInitialize( void );
	void	PlayerNumberInitialize( void );
	void	CoinNumberInitialize( void );
	void	FaceImageInitialize( void );
	void	RoundInitialize( void );
	void	EventInitialize( void );

	//	���C������X�V
	void	TimerUpdate( void );
	void	StartUpdate( void );
	bool	StartMove( void );
	void	StartPlayerNumUpdate( void );
	void	FinishUpdate( void );
	void	AlertUpdate( void );
	void	LastProduction( void );
	void	PlayerNumberUpdate( void );
	void	CoinNumberUpdate( void );
	void	FaceImageUpdate( int num, int mode );
	void	CoinImageInfoUpdate( NumberImageInfo& numImageinfo, NumberInfo& numinfo, const int& num );
	void	EventUpdate( void );
	void	ParticleUpdate( void );
	void	FallLifeUpdate( void );

	//	���C���`��
	void	TimerRender( void );
	void	StartRender( void );
	void	StartPlayerNumRender( void );
	void	FinishRender( void );
	void	AlertRender( void );
	void	LastProductionRender( void );
	void	CoinNumberRender( void );
	void	EventRender( void );
	void	ParticleRender( int value );
	void	RenderTargetParticle( void );

	//	���C������֐�
	void	CoinCounter( int coin , int num );
	void	RoundRender( void );

	//------------------------------���擾�E�ݒ�------------------------------------//

	//	���ݒ�
	void	SetChangeFlag( const bool& flag );
	void	SetAlertInfo( bool flag, int type );
	void	SetCoinImageInfo( NumberImageInfo& numImageinfo, NumberInfo& numinfo, const int& num );
	void	SetNumberInfo( NumberInfo& nomber, int coin );
	void	SetEventInfoMode( int mode );
	void	SetDamageFlag( int playerNum, bool flag, int culLife );

	//	���擾
	bool	GetChangeFlag( void );
	bool	GetMoveState( void );
};

//	extern
extern	UI*	ui;

//*********************************************************************************
#endif // !__UI_H__