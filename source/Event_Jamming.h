#pragma once

//*******************************************************************************
//
//	Event_Jamming�N���X�@�i�W�Q�C�x���g�j
//
//*******************************************************************************

#define	SLOPE_COUNT_MAX	300
#define	COIN_GETAWAY_COUNT_MAX	600

// include

//	data

//	class
class Event_Jamming
{
private:
	struct SLOPE
	{
		bool	eventFlag;
		int		count;
		float	param;
		float	slope;
		int		step;
	};

	struct COINGETAWAY
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	struct SLIP
	{
		bool	eventflag;
		int		count;
		int		step;
	};

	
private:
	int		eventflag;
	SLOPE			m_Slope;
	COINGETAWAY		m_CoinGetAway;
	SLIP			m_Slip;

public:
	//	�������E���
	bool	Initialize(void);
	void	Release(void);

	//	�X�V�E�`��
	void	Update(void);
	void	Render(void);

	//	����֐�
	void	Slope(void);
	void	CoinGetAway(void);
	void	Slip(void);
	
	int		GetEvent(void)const;

	//	���ݒ�
	static	Event_Jamming*	GetInstance(void);
	void	SetEvent(int eventflag);
};

#define	event_jamming	(Event_Jamming::GetInstance())