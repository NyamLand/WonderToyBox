
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"CoinManager.h"

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
		eventflag = EVENT_MODE::NONE;

		//	�\���̏�����
		{
			//	��ʌX���C�x���g������
			{
				eventSlope.step = 0;
				eventSlope.count = 0;
				eventSlope.param = 0.0f;
				eventSlope.slope = 0.0f;
				eventSlope.eventFlag = false;
			}

			//	�R�C�������C�x���g������
			{
				eventCoinGetAway.step = 0;
				eventCoinGetAway.count = 0;
				eventCoinGetAway.eventflag = false;
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
		if ( eventSlope.eventFlag )		EventSlope();

		//	�R�C������
		if ( eventCoinGetAway.eventflag )	EventCoinGetAway();
	}

	//	�`��
	void	EventManager::Render( void )
	{

	}

//--------------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------------

	//	��ʂ��X��
	void	EventManager::EventSlope( void )
	{
		//	�p�����[�^����
		bool	isEnd;
		static	float	slopeAngle = 0.0f;

		switch ( eventSlope.step )
		{
		case 0:
			//	������
			slopeAngle = Random::GetFloat( D3DX_PI / 180.0f * -90.0f, D3DX_PI / 180.0f * 90.0f );
			eventSlope.step++;
			break;

		case 1:
			//	�p�����[�^���Z
			eventSlope.param += 0.01f;

			//	�p�����[�^���x�ݒ�
			if ( eventSlope.param >= 1.0f )
			{
				eventSlope.param = 1.0f;
			}

			//	��ʂ��S�T�x�X����
			isEnd = CubicFunctionInterpolation( eventSlope.slope, 0.0f, slopeAngle, eventSlope.param );

			//	�J�����֐ݒ�
			m_Camera->SetSlope( eventSlope.slope );

			//	�X���I���Ǝ��̃X�e�b�v��
			if ( isEnd )
			{
				eventSlope.step++;
			}
			break;

		case 2:
			//	���b�҂�
			eventSlope.count++;

			if ( eventSlope.count >= SLOPE_COUNT_MAX )
			{
				eventSlope.count = 0;
				eventSlope.param = 0.0f;
				eventSlope.step++;
			}
			break;

		case 3:
			//	�p�����[�^���Z
			eventSlope.param += 0.01f;

			//	�p�����[�^���x�ݒ�
			if ( eventSlope.param >= 1.0f )
			{
				eventSlope.param = 1.0f;
			}

			//	��ʂ��X����
			isEnd = CubicFunctionInterpolation( eventSlope.slope, slopeAngle, 0.0f, eventSlope.param );
			
			//	�J�����֐ݒ�
			m_Camera->SetSlope( eventSlope.slope );
			
			//	�X���I���Ǝ��̃X�e�b�v��
			if ( isEnd )
			{
				eventSlope.step = 0;
				eventSlope.eventFlag = false;
				eventSlope.slope = 0.0f;
				eventflag = EVENT_MODE::NONE;
			}
			break;
		}
	} 

	//	�R�C������
	void	EventManager::EventCoinGetAway( void )
	{
		switch ( eventCoinGetAway.step )
		{
		case 0:
			//	�R�C���̓���؂�ւ�
			m_CoinManager->SetCoinGetAwayFlag( true );
			eventCoinGetAway.step++;
			break;

		case 1:
			eventCoinGetAway.count++;
			if ( eventCoinGetAway.count >= COIN_GETAWAY_COUNT_MAX )
			{
				eventCoinGetAway.step++;
				eventCoinGetAway.count = 0;
			}
			break;

		case 2:
			m_CoinManager->SetCoinGetAwayFlag( false );
			eventCoinGetAway.step = 0;
			eventCoinGetAway.eventflag = false;
			eventflag = EVENT_MODE::NONE;
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
		case EVENT_MODE::SLOPE_CAMERA:
			eventSlope.eventFlag = true;
			break;

		case	EVENT_MODE::SLIP:
			break;

		case EVENT_MODE::FALL_BOMB:
			break;

		case EVENT_MODE::UFO:
			break;

		case EVENT_MODE::COIN_GETAWAY:
			eventCoinGetAway.eventflag = true;
			break;
		}
	}