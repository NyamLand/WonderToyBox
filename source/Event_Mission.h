
#pragma	once

//*******************************************************************************
//
//	Event_Mission�N���X�@�i�R�C���C�x���g�j
//
//*******************************************************************************

//	include
#include	"EventManager.h"

//	class
class Event_Mission : public EventManager
{
private:
	struct COLLECT
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct STAY
	{
		bool	eventflag;
		int		count;
		int		step;
	};

private:
	int		eventflag;
	bool	success;
	COLLECT		m_Collect;
	STAY		m_Stay;

public:
	//	�������E���
	bool	Initialize(void);
	void	Release(void);

	//	�X�V�E�`��
	void	Update(void);
	void	Render(void);

	//	����֐�
	void	Collect(void);
	void	Stay(void);
	
	//	���擾
	static	Event_Mission*	GetInstance(void);
	int		GetEvent(void)const;

	//	���ݒ�
	void	SetEvent(int eventflag);
};

#define	event_mission	( Event_Mission::GetInstance() )