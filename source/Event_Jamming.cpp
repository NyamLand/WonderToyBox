#include	"../IEX/iextreme.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"Camera.h"
#include	"CoinManager.h"
#include	"CharacterManager.h"
#include	"GameManager.h"
#include	"EventManager.h"

#include	"Event_Jamming.h"




//**************************************************************
//�@�������E���
//**************************************************************
bool	Event_Jamming::Initialize()
{
	//	��ʌX���C�x���g������
	{
		m_Slope.step = 0;
		m_Slope.count = 0;
		m_Slope.param = 0.0f;
		m_Slope.slope = 0.0f;
		m_Slope.eventFlag = false;
	}
	
	//	�R�C�������C�x���g������
	{
		m_CoinGetAway.step = 0;
		m_CoinGetAway.count = 0;
		m_CoinGetAway.eventflag = false;
	}
	
	//�@�X���b�v�C�x���g������
	{
		m_Slip.eventflag = 0;
		m_Slip.count = 0;
		m_Slip.step = 0;
	}

	return true;
}


void	Event_Jamming::Release(void)
{

}

//**************************************************************
//	�X�V�E�`��
//**************************************************************
void	Event_Jamming::Update(void)
{
	//	�J�����X��
	if (m_Slope.eventFlag)		Slope();

	//	�R�C������
	if (m_CoinGetAway.eventflag)	CoinGetAway();

	//	�X���b�v
	if (m_Slip.eventflag)		Slip();
}

void	Event_Jamming::Render(void)
{

}

//**************************************************************
//	����֐�
//**************************************************************
void	Event_Jamming::Slope(void)
{
	//	�p�����[�^����
	bool	isEnd;
	static	float	slopeAngle = 0.0f;

	switch (m_Slope.step)
	{
	case 0:
		//	������
		slopeAngle = Random::GetFloat(D3DX_PI / 180.0f * -90.0f, D3DX_PI / 180.0f * 90.0f);
		m_Slope.step++;
		break;

	case 1:
		//	�p�����[�^���Z
		m_Slope.param += 0.01f;

		//	�p�����[�^���x�ݒ�
		if (m_Slope.param >= 1.0f)
		{
			m_Slope.param = 1.0f;
		}

		//	��ʂ��S�T�x�X����
		isEnd = CubicFunctionInterpolation(m_Slope.slope, 0.0f, slopeAngle, m_Slope.param);

		//	�J�����֐ݒ�
		mainView->SetSlope(m_Slope.slope);

		//	�X���I���Ǝ��̃X�e�b�v��
		if (isEnd)
		{
			m_Slope.step++;
		}
		break;

	case 2:
		//	���b�҂�
		m_Slope.count++;

		if (m_Slope.count >= SLOPE_COUNT_MAX)
		{
			m_Slope.count = 0;
			m_Slope.param = 0.0f;
			m_Slope.step++;
		}
		break;

	case 3:
		//	�p�����[�^���Z
		m_Slope.param += 0.01f;

		//	�p�����[�^���x�ݒ�
		if (m_Slope.param >= 1.0f)
		{
			m_Slope.param = 1.0f;
		}

		//	��ʂ��X����
		isEnd = CubicFunctionInterpolation(m_Slope.slope, slopeAngle, 0.0f, m_Slope.param);

		//	�J�����֐ݒ�
		mainView->SetSlope(m_Slope.slope);

		//	�X���I���Ǝ��̃X�e�b�v��
		if (isEnd)
		{
			m_Slope.step = 0;
			m_Slope.eventFlag = false;
			m_Slope.slope = 0.0f;
			eventflag = EVENT_MODE::NONE;
		}
		break;
	}
}

void	Event_Jamming::CoinGetAway(void)
{
	switch (m_CoinGetAway.step)
	{
	case 0:
		//	�R�C���̓���؂�ւ�
		coinManager->SetCoinGetAwayFlag(true);
		m_CoinGetAway.step++;
		break;

	case 1:
		m_CoinGetAway.count++;
		if (m_CoinGetAway.count >= COIN_GETAWAY_COUNT_MAX)
		{
			m_CoinGetAway.step++;
			m_CoinGetAway.count = 0;
		}
		break;

	case 2:
		coinManager->SetCoinGetAwayFlag(false);
		m_CoinGetAway.step = 0;
		m_CoinGetAway.eventflag = false;
		eventflag = EVENT_MODE::NONE;
		break;
	}
}

void	Event_Jamming::Slip(void)
{
	switch (m_Slip.step)
	{
	case 0:
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			characterManager->SetParameterInfo(i, PARAMETER_STATE::SLIP);
		}
		m_Slip.step++;
		break;

	case 1:
		m_Slip.eventflag = false;
		m_Slip.count = 0;
		m_Slip.step = 0;
		break;
	}
}




//**************************************************************
//	���擾�E�ݒ�
//**************************************************************
Event_Jamming*	Event_Jamming::GetInstance(void)
{
	static	Event_Jamming	out;
	return	&out;
}

int		Event_Jamming::GetEvent(void)const
{
	return	eventflag;
}

void	Event_Jamming::SetEvent(int eventflag)
{
	switch (eventflag)
	{
	case EVENT_MODE::JAM_SLOPE_CAMERA:
		m_Slope.eventFlag = true;
		break;

	case EVENT_MODE::JAM_SLIP:
		m_Slip.eventflag = true;
		break;

	case EVENT_MODE::JAM_FALL_BOMB:
		break;

	case EVENT_MODE::JAM_UFO:
		break;

	case EVENT_MODE::JAM_COIN_GETAWAY:
		m_CoinGetAway.eventflag = true;
		break;
	}
}