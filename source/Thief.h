
#ifndef __THIEF_H__
#define	__THIEF_H__

//*********************************************************************************
//
//	Thief�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace THIEF
{
	//���[�V�����ԍ�
	enum MOTION_DATA
	{
		POSTURE = 0,
		RUN,
		JUMP,
		GUARD,
		QUICKARTS,
		POWERARTS,
		HYPERARTS,
		DAMAGE,
		DOWN,
		GUTS,
		HAPPY,
		SAD,
		ANGRY
	};

	//���[�V�����Œ�A�؂�ւ��Ɏg���t���[��
	namespace MOTION_FRAME
	{
		const int POWERARTS_END = 277;
		const int HYPERARTS_ATTACKSTART = 339;
		const int HYPERARTS_ATTACKEND = 399;
		const int DAMAGE_END = 408;
	}
}

//	class
class Thief : public		BaseChara
{
private:
	//��Ń}�W�b�N�i���o�[����
	int HyperStep = 0;
	float HyperRate = 0;


	const float DEFAULT_ABSORB_LENGTH = 3.0f;

private:
	iexMesh*	arm;	//�n�C�p�[�A�[�c�U���p
	iexMesh*	hand;	//�n�C�p�[�A�[�c�U���p

	bool		armRenderflag;

	int		stayTime;	//�Z�֘A�̍d���o�ߎ���
	int		artsTimer;
	float	absorb_length;		//�R�C�����z�����߂鋗��
//	void	MotionManagement(int motion)override;

public:
	//	�������E���
	Thief(void);
	~Thief(void);
	bool	Initialize(int playerNum, Vector3 pos)override;
	void	AttackParamInitialize( void )override;


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
