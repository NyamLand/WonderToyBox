
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"CoinManager.h"
#include	"CharacterManager.h"

#include	"EventManager.h"

//*******************************************************************************
//
//	EventManager�N���X
//
//*******************************************************************************

//--------------------------------------------------------------------------------
//	�O���[�o��
//--------------------------------------------------------------------------------

#define	SLOPE_COUNT_MAX	300
#define	COIN_GETAWAY_COUNT_MAX	600

//--------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------

	//	������
	bool	EventManager::Initialize( void )
	{
		//	�ϐ�������
		eventflag = EVENT_MODE::JAM_NONE;

		//	�\���̏�����
		{
			//-------------------------------------------
			//�@�W�Q�C�x���g�֌W
			//-------------------------------------------

			//	��ʌX���C�x���g������
			{
				event_Jam_Slope.step = 0;
				event_Jam_Slope.count = 0;
				event_Jam_Slope.param = 0.0f;
				event_Jam_Slope.slope = 0.0f;
				event_Jam_Slope.eventFlag = false;
			}

			//	�R�C�������C�x���g������
			{
				event_Jam_CoinGetAway.step = 0;
				event_Jam_CoinGetAway.count = 0;
				event_Jam_CoinGetAway.eventflag = false;
		}

			//	�X���b�v�C�x���g������
			{
				event_Jam_Slip.step = 0;
				event_Jam_Slip.count = 0;
				event_Jam_Slip.eventflag = false;
			}

			//-------------------------------------------
			//�@�R�C���C�x���g�֌W
			//-------------------------------------------

			//�@�R�C����
			{
				event_Coin_Sack.eventflag = false;
				event_Coin_Sack.count = 0;
				event_Coin_Sack.step = 0;
			}

			//�@�R�C���̑�
			{
				event_Coin_Fall.eventflag = false;
				event_Coin_Fall.count = 0;
				event_Coin_Fall.step = 0;
			}

			//�@�R�C�����b�V���@�A
			{
				event_Coin_Splash.eventflag = false;
				event_Coin_Splash.count = 0;
				event_Coin_Splash.step = 0;
			}

			//�@��Δ�
			{
				event_Coin_Juelbox.eventflag = false;
				event_Coin_Juelbox.count = 0;
				event_Coin_Juelbox.step = 0;
			}

			//�@�R�C���̃E�F�[�u
			{
				event_Coin_Wave.eventflag = false;
				event_Coin_Wave.count = 0;
				event_Coin_Wave.step = 0;
			}

			//�@�R�C���Q�{
			{
				event_Coin_Dubble.eventflag = false;
				event_Coin_Dubble.count = 0;
				event_Coin_Dubble.step = 0;
			}
		}

		return	true;
	}

	//	���
	void	EventManager::Release( void )
	{

	}

//--------------------------------------------------------------------------------
//	�X�V�E�`��
//--------------------------------------------------------------------------------

	//	�X�V
	void	EventManager::Update( void )
	{
		//	�J�����X��
		if (event_Jam_Slope.eventFlag)		Event_Jam_Slope();

		//	�R�C������
		if (event_Jam_CoinGetAway.eventflag)	Event_Jam_CoinGetAway();

		//	�X���b�v
		if (event_Jam_Slip.eventflag)		Event_Jam_Slip();
	}

	//	�`��
	void	EventManager::Render( void )
	{

	}

//--------------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------------

	//	��ʂ��X��
	void	EventManager::Event_Jam_Slope(void)
	{
		//	�p�����[�^����
		bool	isEnd;
		static	float	slopeAngle = 0.0f;

		switch ( event_Jam_Slope.step )
		{
		case 0:
			//	������
			slopeAngle = Random::GetFloat( D3DX_PI / 180.0f * -90.0f, D3DX_PI / 180.0f * 90.0f );
			event_Jam_Slope.step++;
			break;

		case 1:
			//	�p�����[�^���Z
			event_Jam_Slope.param += 0.01f;

			//	�p�����[�^���x�ݒ�
			if ( event_Jam_Slope.param >= 1.0f )
			{
				event_Jam_Slope.param = 1.0f;
			}

			//	��ʂ��S�T�x�X����
			isEnd = CubicFunctionInterpolation( event_Jam_Slope.slope, 0.0f, slopeAngle, event_Jam_Slope.param );

			//	�J�����֐ݒ�
			mainView->SetSlope( event_Jam_Slope.slope );

			//	�X���I���Ǝ��̃X�e�b�v��
			if ( isEnd )
			{
				event_Jam_Slope.step++;
			}
			break;

		case 2:
			//	���b�҂�
			event_Jam_Slope.count++;

			if ( event_Jam_Slope.count >= SLOPE_COUNT_MAX )
			{
				event_Jam_Slope.count = 0;
				event_Jam_Slope.param = 0.0f;
				event_Jam_Slope.step++;
			}
			break;

		case 3:
			//	�p�����[�^���Z
			event_Jam_Slope.param += 0.01f;

			//	�p�����[�^���x�ݒ�
			if ( event_Jam_Slope.param >= 1.0f )
			{
				event_Jam_Slope.param = 1.0f;
			}

			//	��ʂ��X����
			isEnd = CubicFunctionInterpolation( event_Jam_Slope.slope, slopeAngle, 0.0f, event_Jam_Slope.param );
			
			//	�J�����֐ݒ�
			mainView->SetSlope( event_Jam_Slope.slope );
			
			//	�X���I���Ǝ��̃X�e�b�v��
			if ( isEnd )
			{
				event_Jam_Slope.step = 0;
				event_Jam_Slope.eventFlag = false;
				event_Jam_Slope.slope = 0.0f;
				eventflag = EVENT_MODE::JAM_NONE;
			}
			break;
		}
	} 

	//	�R�C������
	void	EventManager::Event_Jam_CoinGetAway(void)
	{
		switch ( event_Jam_CoinGetAway.step )
		{
		case 0:
			//	�R�C���̓���؂�ւ�
			m_CoinManager->SetCoinGetAwayFlag( true );
			event_Jam_CoinGetAway.step++;
			break;

		case 1:
			event_Jam_CoinGetAway.count++;
			if ( event_Jam_CoinGetAway.count >= COIN_GETAWAY_COUNT_MAX )
			{
				event_Jam_CoinGetAway.step++;
				event_Jam_CoinGetAway.count = 0;
			}
			break;

		case 2:
			m_CoinManager->SetCoinGetAwayFlag( false );
			event_Jam_CoinGetAway.step = 0;
			event_Jam_CoinGetAway.eventflag = false;
			eventflag = EVENT_MODE::JAM_NONE;
			break;
		}
	}

	//	�X���b�v
	void	EventManager::Event_Jam_Slip(void)
	{
		switch ( event_Jam_Slip.step )
		{
		case 0:
			for ( int i = 0; i < PLAYER_MAX; i++ )
			{
				characterManager->SetParameterInfo( i, PARAMETER_STATE::SLIP );
			}
			event_Jam_Slip.step++;
			break;

		case 1:
			event_Jam_Slip.eventflag = false;
			event_Jam_Slip.count = 0;
			event_Jam_Slip.step = 0;
			break;
		}
	}
	
//--------------------------------------------------------------------------------
//	���擾
//--------------------------------------------------------------------------------

	//	���̎擾
	EventManager*	EventManager::GetInstance( void )
	{
		static	EventManager	out;
		return	&out;
	}

	//	�������̃C�x���g���擾
	int		EventManager::GetEvent( void )const
	{
		return	eventflag;
	}

//--------------------------------------------------------------------------------
//	���ݒ�
//--------------------------------------------------------------------------------

	//	�C�x���g�t���O�ݒ�
	void	EventManager::SetEvent( int eventflag )
	{
		this->eventflag = eventflag;
		switch ( eventflag )
		{
		case EVENT_MODE::JAM_SLOPE_CAMERA:
			event_Jam_Slope.eventFlag = true;
			break;

		case EVENT_MODE::JAM_SLIP:
			event_Jam_Slip.eventflag = true;
			break;

		case EVENT_MODE::JAM_FALL_BOMB:
			break;

		case EVENT_MODE::JAM_UFO:
			break;

		case EVENT_MODE::JAM_COIN_GETAWAY:
			event_Jam_CoinGetAway.eventflag = true;
			break;

		case EVENT_MODE::COIN_SACK:

			break;

		case EVENT_MODE::COIN_FALL: 
			break;

		case EVENT_MODE::COIN_SPLASH: 
			break;

		case EVENT_MODE::COIN_JUELBOX: 
			break;

		case EVENT_MODE::COIN_WAVE: 
			break;

		case EVENT_MODE::COIN_DUBBLE: 
			break;

		}
	}