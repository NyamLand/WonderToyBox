
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
#include	"Event_Coin.h"
#include	"Event_Jamming.h"
#include	"Event_Mission.h"

#include	"EventManager.h"

//*******************************************************************************
//
//	EventManager�N���X
//
//*******************************************************************************

//--------------------------------------------------------------------------------
//	�O���[�o��
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------

	//	������
	bool	EventManager::Initialize( void )
	{
		//	�ϐ�������
		eventflag = EVENT_MODE::NONE;
		
		//�@�e��C�x���g�p�����[�^������
		event_coin->Initialize();
		event_jamming->Initialize();
		event_mission->Initialize();

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
		event_coin->Update();
		event_jamming->Update();
		event_mission->Update();
	}

	//	�`��
	void	EventManager::Render( void )
	{

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
		if (EVENT_MODE::JAM_SLOPE_CAMERA <= eventflag && eventflag <= EVENT_MODE::JAM_UFO)	event_jamming->SetEvent(eventflag);
		else if (EVENT_MODE::COIN_SACK <= eventflag && eventflag <= EVENT_MODE::COIN_DUBBLE)	event_coin->SetEvent(eventflag);
		else event_mission->SetEvent(eventflag);
	}