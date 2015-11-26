
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Pirate.h"
#include	"GameManager.h"
#include	"BulletManager.h"

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

namespace
{
	enum OFFENSIVE_POWER
	{
		QUICK = 1,
		POWER = 5,
		HYPER = 15,
	};
}

//-----------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------

//	�R���X�g���N�^
Pirate::Pirate(void) : BaseChara()
{
	//	�p�����[�^������
	power = 2;/*��*/
	speed = 0.25f;
	scale = 0.02f;
	diffence = -1;
	isGround = true;
}

//	�f�X�g���N�^
Pirate::~Pirate(void)
{

}

//	������
bool	Pirate::Initialize(int playerNum, Vector3 pos)
{
	//	�e�̏�����
	BaseChara::Initialize(playerNum, pos);

	//	���f���ǂݍ���
	if (obj == nullptr)
		obj = new iex3DObj("DATA/CHR/Y2009/Y2009.IEM");

	//	�X�P�[���ݒ�
	obj->SetScale(0.02f);
	obj->Update();

	if (obj == nullptr)	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

//	�`��
void	Pirate::Render(iexShader* shader, LPSTR technique)
{
	BaseChara::Render(shader, technique);

	//	�f�o�b�O�p
	if (!debug)	return;
	DrawSphere(attackInfo.pos, attackInfo.r, 0xFFFFFFFF);
	particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z - attackInfo.r), 0.3f);
	particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z + attackInfo.r), 0.3f);
	particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);
	particle->BlueFlame(Vector3(attackInfo.pos.x - attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);

	char	str[256];
	Vector3	stringPos;
	WorldToClient(pos, stringPos, matView* matProjection);
	stringPos.y -= 150.0f;
	sprintf_s(str, "�C\n��\n��");
	DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

//	�N�C�b�N�A�[�c
bool	Pirate::QuickArts(void)
{
	static int time = 0;

	//	�s�񂩂���擾
	Vector3	front = GetFront();
	front.Normalize();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));
	Vector3 vec = front;

	p_pos.y += 1.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		m_BulletManager->Set(BULLET_TYPE::PIRATE_01, new Pirate_Bullet01, p_pos, vec, bulletSpeed, playerNum);
	}
	time++;

	//��b�ԍd��
	if (time >= 1 * SECOND)
	{
		time = 0;
		return true;
	}
	return	false;
}

//	�p���[�A�[�c
bool	Pirate::PowerArts(void)
{
	power = POWER;
	Vector3	p_pos = GetPos();
	attackInfo.pos = Vector3(p_pos.x, p_pos.y + 1.5f, p_pos.z);
	SetMove(Vector3(0.0f, 0.0f, 0.0f));

	//	�͈͊g��
	Lerp(attackInfo.r, 0.0f, 3.0f, attackInfo.t);

	//	�p�����[�^���Z
	attackInfo.t += 0.02f;

	//	���G���
	if (attackInfo.t <= 0.5f)		unrivaled = true;
	else									unrivaled = false;

	if (attackInfo.t >= 1.0f)	return	true;
	return	false;
}

//	�n�C�p�[�A�[�c
bool	Pirate::HyperArts(void)
{
	power = HYPER;

	static	int		num = 0;	//	��
	SetMove(Vector3(0.0f, 0.0f, 0.0f));
	Vector3	p_pos = GetPos();
	attackInfo.pos = Vector3(p_pos.x, p_pos.y + 1.5f, p_pos.z);

	//	�͈͊g��
	float t = GetBezier(ePrm_t::eSlow_Lv4, ePrm_t::eRapid_Lv1, attackInfo.t);
	Lerp(attackInfo.r, 0.0f, 50.0f, t);

	//	�p�����[�^���Z
	attackInfo.t += 0.02f;

	if (attackInfo.t >= 1.0f)
	{
		switch (num)
		{
		case 0:
			num++;
			attackInfo.t = 0.0f;
			break;

		case 1:
			num = 0;
			return	true;
			break;
		}
	}
	return	false;
}

//	���[�V�����Ǘ�
void	Pirate::MotionManagement(int motion)
{
	switch (motion)
	{
	case MOTION_NUM::STAND:
		obj->SetMotion(MOTION_DATA::STAND);
		break;

	case MOTION_NUM::POSTURE:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::JUMP:
		obj->SetMotion(MOTION_DATA::JUMP);
		break;

	case MOTION_NUM::GUARD:
		obj->SetMotion(MOTION_DATA::GUARD);
		break;

	case MOTION_NUM::LANDING:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::RUN:
		obj->SetMotion(MOTION_DATA::RUN);
		break;

	case MOTION_NUM::ATTACK1:
		obj->SetMotion(MOTION_DATA::ATTACK1);
		break;

	case MOTION_NUM::ATTACK2:
		obj->SetMotion(MOTION_DATA::ATTACK2);
		break;

	case MOTION_NUM::ATTACK3:
		obj->SetMotion(MOTION_DATA::ATTACK3);
		break;
	}
}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

//	�U���p�p�����[�^�ݒ�
void	Pirate::SetAttackParam(int attackKind)
{
	switch (attackKind)
	{
	case MODE_STATE::QUICKARTS:
		knockBackInfo.isUp = false;
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		if (attackInfo.t < 0.6) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hit�܂ł͋�����̂�
		if (attackInfo.t >= 0.6) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit�ڂ���͐�����΂�����
		break;

	case MODE_STATE::POWERARTS:
		knockBackInfo.isUp = true;
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
		break;

	case MODE_STATE::HYPERARTS:
		knockBackInfo.isUp = false;
		attackInfo.type = Collision::SPHEREVSCYRINDER;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}