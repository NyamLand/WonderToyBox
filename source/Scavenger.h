
#ifndef __SCAVENGER_H__
#define	__SCAVENGER_H__

//*********************************************************************************
//
//	Scavenger�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace SCAVENGER
{
	namespace MOTION_FRAME
	{
		const int QUICKARTS_SUCK = 92;
		const int QUICKARTS_END = 153;
		const int POWER_TO_WAIT = 170;
		const int POWERARTS_END = 200;
		const int HYPER_BEGIN = 203;
		const int HYPER_ABSORB = 240;
		const int HYPER_FINISH = 248;
	}

	//�@�������[�V����
	enum MOTION_DATA
	{
		WAIT,
		RUN,
		QUICK_START,
		QUICK,
		QUICK_END,
		POWER_START,
		POWER_END,
		HYPER_START,
		HYPER_END,
		DAMAGE,
		WIN,
		LOSE
	};
}


//	class
class Scavenger : public		BaseChara
{
private:
	
	
	const float DEFAULT_ABSORB_LENGTH = 3.0f;

private:
	int		stayTime;	//�Z�֘A�̍d���o�ߎ���
	float	absorb_length;		//�R�C�����z�����߂鋗��
	bool		fireBallState;
	int		fireBallInterval;
	int		fireBallStep;
//	void	MotionManagement(int motion)override;

public:
	//	�������E���
	Scavenger(void);
	~Scavenger(void);
	bool	Initialize(int playerNum, Vector3 pos)override;

	//	�X�V�E�`��
	void	Update( void )override;
	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr)override;

	//	����֐�
	bool	QuickArts(void)override;
	bool	PowerArts(void)override;
	bool	HyperArts(void)override;

	void	ShiftMove(void);
	void	RollAngle(void);

	//	���ݒ�
	void	SetAttackParam(int attackKind)override;
};

//*********************************************************************************
#endif // !__MAID_H__
