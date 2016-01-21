#include	"../IEX/iextreme.h"
#include	"GameManager.h"
#include	"Random.h"
#include	"CoinManager.h"
#include	"Coin.h"

#include	"Event_Coin.h"

//**************************************************************
//�@�������E���
//**************************************************************
bool	Event_Coin::Initialize()
{
	//�@�R�C����
	{
		m_Sack.eventflag = false;
		m_Sack.step = 0;
	}

	//�@�R�C���̑�
	{
		m_Fall.eventflag = false;
		m_Fall.step = 0;
	}

	//�@�R�C�����b�V���@�A
	{
		m_Splash.eventflag = false;
		m_Splash.step = 0;
	}

	//�@��Δ�
	{
		m_Juelbox.eventflag = false;
		m_Juelbox.step = 0;
	}

	//�@�R�C���̃E�F�[�u
	{
		m_Wave.eventflag = false;
		m_Wave.step = 0;
	}

	//�@�R�C���Q�{
	{
		m_Dubble.eventflag = false;
		m_Dubble.step = 0;
	}

	return true;
}


void	Event_Coin::Release(void)
{

}

//**************************************************************
//	�X�V�E�`��
//**************************************************************
void	Event_Coin::Update(void)
{
	//	�R�C���̑�
	if (m_Fall.eventflag)
	{
		Fall();
		eventManager->SetEventFlag(m_Fall.eventflag);
	}

	//�@�R�C���̑���
	if (m_Wave.eventflag)
	{
		Wave();
		eventManager->SetEventFlag(m_Wave.eventflag);
	}
	
	//�@�R�C���Q�{�^�C��
	if (m_Dubble.eventflag)
	{
		Dubble();
		eventManager->SetEventFlag(m_Dubble.eventflag);
	}
}

void	Event_Coin::Render(void)
{

}

//**************************************************************
//	����֐�
//**************************************************************
void	Event_Coin::Sack(void)
{
	//�@���낢��Z�b�g�E���Z�b�g
	CommonSetting(m_Sack.eventflag, m_Sack.step, SACK_COUNT_MAX);
}

//�@��
void	Event_Coin::Fall(void)
{
	//�@�R�C������
	if (count % SECOND == 0)
	{
		for (int i = 0; i < 50; i++)
		{
			Vector3	pos = Vector3(Random::GetFloat(-20.0f, 20.0f), COIN_APPEAR_HEIGHT, Random::GetFloat(-20.0f, 12.0f));
			static	Vector3	vec = Vector3(0.0f, -1.0f, 0.0f);
			static	float	power = 1.0f;
			coinManager->Append(pos, vec, power);
		}
	}

	CommonSetting(m_Fall.eventflag, m_Fall.step, FALL_COUNT_MAX);
}

void	Event_Coin::Splash(void)
{

}

void	Event_Coin::Juelbox(void)
{

}

//�@����
void	Event_Coin::Wave(void)
{
	//�@���낢���`�i�}�N���Ă��Ȃ�j
	const float STAGE_X_MAX = 20.0f;
	const float STAGE_Z_MAX = 20.0f;
	const float COIN_SPACE_RATE = 0.5f;
	const float distZ = 1.5f;
	const float power = 0.2f;
	const Vector3	vec = Vector3(0.0f, -0.1f, 0.0f);
	const int POINT_MAX = 8;
	const Vector3 point[] = {
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//�@����
		Vector3( STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//�@�E��
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, -STAGE_Z_MAX),	//�@����
		Vector3( STAGE_X_MAX, COIN_APPEAR_HEIGHT, -STAGE_Z_MAX),	//�@�E��
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//�@����
		Vector3( STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//�@�E��
		Vector3( STAGE_X_MAX, COIN_APPEAR_HEIGHT, -STAGE_Z_MAX),	//�@�E��
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, -STAGE_Z_MAX),	//�@����
		Vector3(-STAGE_X_MAX, COIN_APPEAR_HEIGHT, STAGE_Z_MAX),		//�@����
	};

	//---------------------------------------------
	//�@�g�̂悤�ɑ����g��ō~���Ă���
	//---------------------------------------------
	static  int index = 0;	
	static  Vector3 pos1 = point[index];
	Vector3 pos2 = pos1;
	pos2.z += distZ;
	
	//�@�K�p
	switch (count % 8)
	{
	case 0:
		coinManager->Append(pos1, vec, power);
		break;

	case 1:
		coinManager->Append(pos2, vec, power);
		break;

	default:	break;
	}

	//�@�~�炷�ʒu�X�V
	if ((point[index + 1] - pos1).LengthSq() < 1.0f)	index++;
	Vector3 addVec = (index < POINT_MAX) ? (point[index + 1] - pos1) : (point[0] - pos1);
	addVec.Normalize();
	addVec *= COIN_SPACE_RATE;
	pos1 += addVec;


	//�@���낢��Z�b�g�E���Z�b�g
	CommonSetting(m_Wave.eventflag, m_Wave.step, WAVE_COUNT_MAX);
	if (count >= WAVE_COUNT_MAX || index >= POINT_MAX) index = 0;
}

//�@�R�C���Q�{�^�C��
void	Event_Coin::Dubble(void)
{
	//�@���낢��Z�b�g�E���Z�b�g
	CommonSetting(m_Dubble.eventflag, m_Dubble.step, DUBBLE_COUNT_MAX);
}

//�@�t���O�E��ԂȂǂ�ݒ�
void	Event_Coin::CommonSetting(bool& eventflag,int& step, int countMax)
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

//**************************************************************
//	���擾�E�ݒ�
//**************************************************************
Event_Coin*	Event_Coin::GetInstance(void)
{
	static	Event_Coin	out;
	return	&out;
}

int		Event_Coin::GetEvent(void)const
{
	return	eventflag;
}

Event_Coin::DUBBLE	Event_Coin::GetDubbleInst(void)const
{
	return m_Dubble;
}

void	Event_Coin::SetEvent(int eventflag)
{
	switch (eventflag)
	{
	case EVENT_MODE::COIN_SACK:
		m_Sack.eventflag = true;
		break;

	case EVENT_MODE::COIN_FALL:
		m_Fall.eventflag = true;
		break;

	case EVENT_MODE::COIN_SPLASH:
		m_Sack.eventflag = true;
		break;

	case EVENT_MODE::COIN_JUELBOX:
		m_Juelbox.eventflag = true;
		break;

	case EVENT_MODE::COIN_WAVE:
		m_Wave.eventflag = true;
		break;

	case EVENT_MODE::COIN_DUBBLE:
		m_Dubble.eventflag = true;
		break;

	}
}
