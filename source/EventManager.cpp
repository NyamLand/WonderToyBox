
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
bool EventManager::eventFlag = false;
int  EventManager::count = 0;
int  EventManager::state = EVENT_STATE::INTRO;

//--------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------

	//	������
	bool	EventManager::Initialize( void )
	{
		//	�ϐ�������
		count = 0;
		eventMode = EVENT_MODE::NONE;
		eventFlag = false;
		
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
		
		//RenderImage()

	}

//--------------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------------
	//�@�t���O�E��ԂȂǂ�ݒ�
	void	EventManager::CommonSetting(bool& eventflag, int& step, int countMax)
	{
		if (count >= FLYING_COUNT)	state = EVENT_STATE::ACTIVE;
		if (count >= countMax - FLYING_COUNT)	state = EVENT_STATE::END;

		//�@���Z�b�g
		if (count >= countMax)
		{
			eventflag = false;
			step = 0;

			count = 0;
			state = EVENT_STATE::INTRO;
			return;
		}

		count++;
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
		return	eventMode;
	}

	//�@�C�x���g���������ǂ����̃t���O���擾
	bool	EventManager::GetEventFlag(void)const
	{
		return	eventFlag;
	}

	//�@��Ԏ擾
	int		EventManager::GetState(void)const
	{
		return state;
	}

//--------------------------------------------------------------------------------
//	���ݒ�
//--------------------------------------------------------------------------------

	//	�C�x���g���[�h�ݒ�
	void	EventManager::SetEvent( int eventMode )
	{
		this->eventMode = eventMode;
		if (EVENT_MODE::JAM_SLOPE_CAMERA <= eventMode && eventMode <= EVENT_MODE::JAM_SLIP)	event_jamming->SetEvent(eventMode);
		else if (EVENT_MODE::COIN_FALL <= eventMode && eventMode <= EVENT_MODE::COIN_DUBBLE)	event_coin->SetEvent(eventMode);
		else event_mission->SetEvent(eventMode);
	}

	//	�C�x���g�t���O�ݒ�
	void	EventManager::SetEventFlag(bool eventFlag)
	{
		this->eventFlag = eventFlag;
	}