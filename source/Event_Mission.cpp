
#include	"Event_Mission.h"
//*****************************************************************
//	�������E���
//*****************************************************************
bool	Event_Mission::Initialize(void)
{
	{
		m_Collect.eventflag = false;
		m_Collect.count = 0;
		m_Collect.step = 0;
	}
	
	{
		m_Stay.eventflag = false;
		m_Stay.count = 0;
		m_Stay.step = 0;
	}

	return true;
}

void	Event_Mission::Release(void)
{

}
//*****************************************************************
//	�X�V�E�`��
//*****************************************************************
void	Event_Mission::Update(void)
{
	if (m_Collect.eventflag) Collect();
	if (m_Stay.eventflag) Stay();
}

void	Event_Mission::Render(void)
{

}

//*****************************************************************
//�@����֐�
//*****************************************************************
void	Event_Mission::Collect(void)
{
	//�@��΂��Q�O����I�i���D��S������I�j

	/*
		�P.�~�b�V�������o������
		�Q.���D�o��
		�R.�������Ԍ���n�߂�
		�@�@�E�����F���o �� ���J��
		  �@�E���s�F���o �� �S���̃R�C������
	*/

	//switch (m_Collect.step)
	//{
	//case 0:
	//	//�@
	//
	//
	//}
}

void	Event_Mission::Stay(void)
{
	//�@�G���A�ɂƂǂ܂�I
}

//*****************************************************************
//	���擾
//*****************************************************************
Event_Mission*	Event_Mission::GetInstance(void)
{
	static	Event_Mission	out;
	return	&out;
}

//*****************************************************************
//	���ݒ�
//*****************************************************************
void	Event_Mission::SetEvent(int eventflag)
{
	switch (eventflag)
	{
	//case EVENT_MODE::MISSION_COLLECT:
	//	m_Collect.eventflag = true;
	//	break;
	//
	//case EVENT_MODE::MISSION_STAY:
	//	m_Stay.eventflag = true;
	//	break;
	}
}