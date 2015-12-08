
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Thief.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"GameManager.h"
#include	"BulletManager.h"

//*********************************************************************************
//
//	Thief�N���X
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
Thief::Thief(void) : BaseChara()
{
	//	�p�����[�^������
	speed = 0.25f;
	scale = 0.05f;
	diffence = -1;
	stayTime = 0;
	absorb_length = DEFAULT_ABSORB_LENGTH;
	isGround = true;
}

//	�f�X�g���N�^
Thief::~Thief(void)
{

}

//	������
bool	Thief::Initialize(int playerNum, Vector3 pos)
{
	//	�e�̏�����
	BaseChara::Initialize(playerNum, pos);

	//	���f���ǂݍ���
	if (obj == nullptr)
		obj = new iex3DObj("DATA/CHR/Thief/Thief.IEM");

	if (obj == nullptr)	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

//	�`��
void	Thief::Render(iexShader* shader, LPSTR technique)
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
	sprintf_s(str, "��\n��\n��");
	DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

//	�N�C�b�N�A�[�c
bool	Thief::QuickArts(void)
{
	static int time = 0;

	//	�s�񂩂���擾
	Vector3	up = GetUp();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//	���ݒ�
	Vector3	vec[3] =
	{
		up * 5.0f + right * 3.0f,
		up * 5.0f,
		up * 5.0f + right * -3.0f
	};
	p_pos.y += 3.0f;
	float	 bulletSpeed = 0.5f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			m_BulletManager->Set(BULLET_TYPE::THIEF_01 , new Thief_Bullet01, p_pos, vec[i], bulletSpeed, playerNum);
		}
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
bool	Thief::PowerArts(void)
{
	static int time = 0;

	//	�s�񂩂���擾
	Vector3	front = GetFront();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//	���ݒ�
	p_pos.y += 1.0f;
	Vector3	vec = front * 1.0f;

	float	 bulletSpeed = 0.8f;
	int leanpower = 30;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		m_BulletManager->Set(BULLET_TYPE::THIEF_02, new Thief_Bullet02, p_pos, vec, bulletSpeed, playerNum);
	}
		time++;

		if (time >= 1 * SECOND)
		{
			time = 0;
			return true;
		}
	return	false;
}

//	�n�C�p�[�A�[�c
bool	Thief::HyperArts(void)
{
	power = HYPER;
	unrivaled = true;
	move = Vector3(0, 0 - GRAVITY, 0);	//�����Ă�Ԃ͐Î~������

	static int time = 0;

	//	�s�񂩂���擾
	Vector3	front = GetFront();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//	���ݒ�
	Vector3	vec[5] =
	{
		front * 5.0f + right * 8.0f,
		front * 5.0f + right * 3.0f,
		front * 5.0f,
		front * 5.0f + right * -3.0f,
		front * 5.0f + right * -8.0f
	};
	Vector3 b_angle[5] =
	{
		{ 0, angle + GetAngle(vec[0], front), 0 },
		{ 0, angle + GetAngle(vec[1], front), 0 },
		{ 0, angle, 0 },
		{ 0, angle - GetAngle(vec[3], front), 0 },
		{ 0, angle - GetAngle(vec[4], front), 0 }
	};
	float	 bulletSpeed = 0.5f;
	int playerNum = GetPlayerNum();

	if (time == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			m_BulletManager->Set(BULLET_TYPE::THIEF_03, new Thief_Bullet03, p_pos, vec[i], b_angle[i],  bulletSpeed, playerNum);
		}
	}
	time++;

	if (time >= 150)
	{
		time = 0;
		return true;
	}
	return	false;
}


//	���[�V�����Ǘ�
void	Thief::MotionManagement(int motion)
{
	switch (motion)
	{
	//case MOTION_NUM::STAND:
	//	obj->SetMotion( MOTION_DATA::STAND );
	//	break;

	case MOTION_NUM::POSTURE:
		obj->SetMotion( MOTION_DATA::POSTURE );
		break;

	case MOTION_NUM::JUMP:
		obj->SetMotion(MOTION_DATA::JUMP);
		break;

	case MOTION_NUM::GUARD:
		obj->SetMotion(MOTION_DATA::GUARD);
		break;

	//case MOTION_NUM::LANDING:
	//	obj->SetMotion(MOTION_DATA::POSTURE);
	//	break;

	case MOTION_NUM::RUN:
		obj->SetMotion(MOTION_DATA::RUN);
		break;

	case MOTION_NUM::ATTACK1:
		obj->SetMotion(MOTION_DATA::QUICK);
		break;

	case MOTION_NUM::ATTACK2:
		obj->SetMotion(MOTION_DATA::POWER);
		break;

	case MOTION_NUM::ATTACK3:
		obj->SetMotion(MOTION_DATA::HYPER);
		break;
	}
}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

//	�U���p�p�����[�^�ݒ�
void	Thief::SetAttackParam(int attackKind)
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