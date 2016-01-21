
#pragma	once

//*******************************************************************************
//
//	Event_Coin�N���X�@�i�R�C���C�x���g�j
//
//*******************************************************************************

//	include
#include	"EventManager.h"

#define		SACK_COUNT_MAX		8 * SECOND
#define		FALL_COUNT_MAX		3 * SECOND
#define		DUBBLE_COUNT_MAX	10 * SECOND
#define		WAVE_COUNT_MAX		15 * SECOND

#define		COIN_APPEAR_HEIGHT	50.0f

//	class
class Event_Coin : public EventManager
{
private:
	
	struct SACK
	{
		iex3DObj*	sack;
		bool	eventflag;
		int		step;
	};

	struct FALL
	{
		bool	eventflag;
		int		step;
	};

	struct SPLASH
	{
		bool	eventflag;
		int		step;
	};

	struct JUELBOX
	{
		bool	eventflag;
		int		step;
	};

	struct WAVE
	{
		bool	eventflag;
		int		step;
	};

public:
	struct DUBBLE
	{
		bool	eventflag;
		int		step;
	};

private:
	int		eventflag;
	
	SACK		m_Sack;
	FALL		m_Fall;
	SPLASH		m_Splash;
	JUELBOX		m_Juelbox;
	WAVE		m_Wave;
	DUBBLE		m_Dubble;

public:
	//	�������E���
	bool	Initialize(void);
	void	Release(void);

	//	�X�V�E�`��
	void	Update(void);
	void	Render(void);

	//	����֐�
	void	Sack(void);			//�@�R�C����
	void	Fall(void);			//�@��
	void	Splash(void);		//�@���b�V���@�A�A
	void	Juelbox(void);		//�@���
	void	Wave(void);			//�@����
	void	Dubble(void);		//�@�Q�{�^�C��
	void	CommonSetting(bool& eventflag, int& step, int countMax);

	//	���擾
	static	Event_Coin*	GetInstance(void);
	int		GetEvent(void)const;
	DUBBLE	GetDubbleInst(void)const;

	//	���ݒ�
	void	SetEvent(int eventflag);
};

#define	event_coin	( Event_Coin::GetInstance() )