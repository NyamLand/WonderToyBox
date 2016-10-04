
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
#include	"StartFinUI.h"
#include	"FallLife.h"
#include	"PlayerNumUI.h"
#include	"CoinNumUI.h"
#include	"FaceUI.h"
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
	ImageObj	alertImage;
	ImageObj	alert_coinImage;
	ImageObj	startNum[PLAYER_MAX];
	ImageObj	pNumImage[PLAYER_MAX];
	ImageObj	roundImage;
	ImageObj	redAlert;
	iex2DObj*	playerNumber;
	iex2DObj*	startNumber;

private:
	//	�萔
	Vector3 PAR_POS;

	//	�J�E���g�_�E���E�X�^�[�g�E�^�C���A�b�v���o
	CountInfo			countInfo;
	
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

	//	�v���C���[�ԍ�
	PlayerNumUI*	playerNumUI;

	//	�R�C������UI���
	CoinNumUI*		coinNumUI;

	//	�w�i�i��j
	FaceUI*			faceUI;

	//	�J�n�E�I��UI
	StartFinUI*		startfinUI;

	//�@�C�x���g���
	EventInfo			eventInfo;
		
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
	void	StartPlayerNumInitialize( void );
	void	AlertInitialize( void );
	void	LastProductionInitialize( void );
	void	PlayerNumberInitialize( void );
	void	RoundInitialize( void );
	void	EventInitialize( void );

	//	���C������X�V
	void	TimerUpdate( void );
	void	StartPlayerNumUpdate( void );
	void	AlertUpdate( void );
	void	LastProduction( void );
	void	PlayerNumberUpdate( void );
	void	EventUpdate( void );
	void	FallLifeUpdate( void );

	//	���C���`��
	void	TimerRender( void );
	void	StartPlayerNumRender( void );
	void	AlertRender( void );
	void	LastProductionRender( void );
	void	EventRender( void );
	void	RenderTargetParticle( void );

	//	���C������֐�
	void	CoinCounter( int coin , int num );
	void	RoundRender( void );

	//------------------------------���擾�E�ݒ�------------------------------------//

	//	���ݒ�
	void	SetChangeFlag( const bool& flag );
	void	SetAlertInfo( bool flag, int type );
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