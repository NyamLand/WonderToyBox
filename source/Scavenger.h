
#ifndef __SCAVENGER_H__
#define	__SCAVENGER_H__

//*********************************************************************************
//
//	Scavenger�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Scavenger : public		BaseChara
{
private:
	enum MOTION_DATA
	{
		STAND,				//	����
		RUN,				//	����
		POSTURE,				//	�\��
		ATTACK1,				//	�U���P�i�K��
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		JUMP,
		GUARD,
	};
	const float DEFAULT_ABSORB_LENGTH = 3.0f;

private:
	int		stayTime;	//�Z�֘A�̍d���o�ߎ���
	float	absorb_length;		//�R�C�����z�����߂鋗��
//	void	MotionManagement(int motion)override;

public:
	//	�������E���
	Scavenger(void);
	~Scavenger(void);
	bool	Initialize(int playerNum, Vector3 pos)override;

	//	�X�V�E�`��
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
