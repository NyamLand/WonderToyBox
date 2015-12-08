
#ifndef __THIEF_H__
#define	__THIEF_H__

//*********************************************************************************
//
//	Thief�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Thief : public		BaseChara
{
private:
	enum MOTION_DATA
	{
		POSTURE = 0,
		RUN,
		JUMP,
		GUARD,
		QUICK,
		POWER,
		HYPER,
		DAMAGE,
		DOWN,
		GUTS,
		HAPPY,
		SAD,
		ANGRY
	};
	const float DEFAULT_ABSORB_LENGTH = 3.0f;

private:
	int		stayTime;	//�Z�֘A�̍d���o�ߎ���
	float	absorb_length;		//�R�C�����z�����߂鋗��
	void	MotionManagement(int motion)override;

public:
	//	�������E���
	Thief(void);
	~Thief(void);
	bool	Initialize(int playerNum, Vector3 pos)override;

	//	�X�V�E�`��
	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr)override;

	//	����֐�
	bool	QuickArts(void)override;
	bool	PowerArts(void)override;
	bool	HyperArts(void)override;

	//	���ݒ�
	void	SetAttackParam(int attackKind)override;
};

//*********************************************************************************
#endif // !__MAID_H__
