
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Thief.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"BulletManager.h"

//*********************************************************************************
//
//	Thief�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------

namespace OFFENSIVE_POWER
{
	enum 
	{
		QUICK = 0,
		POWER = 0,
		HYPER = 1,
	};
}

namespace DROP_POWER
{
	enum 
	{
		QUICK = 0,
		POWER = 0,
		HYPER = 20,
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
	scale = 0.035f;
	diffence = -1;
	stayTime = 0;
	artsTimer = 0;
	absorb_length = DEFAULT_ABSORB_LENGTH;
	isGround = true;
	armRenderflag = false;
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
	arm = new iexMesh("DATA/Effect/k_arm/k_arm4.imo");
	hand = new iexMesh("DATA/Effect/kq_hand(��)/hand.IMO");

	arm->SetScale(0.1f);

	if (obj == nullptr)	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

//	�`��
void	Thief::Render(iexShader* shader, LPSTR technique)
{
	//SetArmTransform();
	BaseChara::Render(shader, technique);

	if (armRenderflag) arm->Render(shader, technique);

	////	�f�o�b�O�p
	//if (!debug)	return;
	//DrawCapsule(attackInfo.bottom, attackInfo.top, attackInfo.r, 0xFFFFFFFF);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z - attackInfo.r), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z + attackInfo.r), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x - attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);

	//char	str[256];
	//Vector3	stringPos;
	//WorldToClient(pos, stringPos, matView* matProjection);
	//stringPos.y -= 150.0f;
	//sprintf_s(str, "��\n��\n��");
	//DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

//	�N�C�b�N�A�[�c
bool	Thief::QuickArts(void)
{
	static int time = 0;
	static	bool	initflag = false;
	if ( !initflag )
	{
		sound->PlaySE( SE::KAITO_QUICK );
		initflag = true;
	}
	Move();
	//	�s�񂩂���擾
	Vector3	up = GetUp();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//���[�V�����A�g�f�i�I�X
	SetMotion(4);
	//if (obj->GetFrame() >= 237) obj->SetMotion(237);

	//	���ݒ�
	Vector3	vec[3] =
	{
		up * 5.0f + right * 3.0f,
		up * 5.0f,
		up * 5.0f + right * -3.0f
	};
	p_pos.y += 3.0f;
	float	 bulletSpeed = 0.5f;
	int playerNum = GetPlayerNum();


	{	//���������ɓ�������x�~�܂�p�^�[��
		//					//���[�V�����A�g�f�i�I�X(���傤�ǂ��������̃t���[����������e����)
		//if (time == 0 /* obj->GetFrame() == ���� */ )
		//{
		//	for (int i = 0; i < 3; i++)
		//	{
		//		m_BulletManager->Set(BULLET_TYPE::THIEF_01 , new Thief_Bullet01, p_pos, vec[i], bulletSpeed, playerNum);
		//	}
		//}
		//time++;

		////���[�V�����A�g�f�i�I�X(�I���̃��[�V������������I��)
		//if (obj->GetFrame() == 237)
		//{
		//	time = 0;
		//	return true;
		//}
	}

	{	//���������ɓ������~�܂�Ȃ��p�^�[��
		for (int i = 0; i < 3; i++)
		{
			m_BulletManager->Set(BULLET_TYPE::THIEF_01 , new Thief_Bullet01, p_pos, vec[i], bulletSpeed, playerNum);
		}
		attackInfo.Interval = 60;
		initflag = false;
		return true;

	}
	return	false;
}

//	�p���[�A�[�c
bool	Thief::PowerArts(void)
{
	if ( !initflag )
	{
		artsTimer = 0;
		sound->PlaySE( SE::KAITO_POWER );
		initflag = true;
	}
	//���[�V�����A�g�f�i�I�X
	SetMotion(5);
	//if (obj->GetFrame() >= 277) obj->SetFrame(277);

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

	//���[�V�����A�g�f�i�I�X(���傤�ǂ��������̃t���[����������e����)
	if (artsTimer == 0/* obj->GetFrame() == ���� */)
	{
		m_BulletManager->Set(BULLET_TYPE::THIEF_02, new Thief_Bullet02, p_pos, vec, bulletSpeed, playerNum);
	}
	artsTimer++;

		//���[�V�����A�g�f�i�I�X
		if (obj->GetFrame() == 277)
		{
			artsTimer = 0;
			initflag = false;
			return true;
		}
	return	false;
}

//	�n�C�p�[�A�[�c
bool	Thief::HyperArts(void)
{
	if ( !initflag )
	{
		sound->PlaySE( SE::HYPER_ATTACK );
		sound->PlaySE( SE::KAITO_HYPER );
		initflag = true;
	}


	//���[�V�����A�g�f�i�I�X
	SetMotion(6);

	attackInfo.power = OFFENSIVE_POWER::HYPER;
	attackInfo.dropPower = DROP_POWER::HYPER;
	attackInfo.coinDropType = DROP_TYPE::SUCTION;

	SetParameterState(PARAMETER_STATE::UNRIVALED);
	move = Vector3(0, 0 - GRAVITY, 0);	//�����Ă�Ԃ͐Î~������


	//	�s�񂩂���擾
	Vector3	front = GetFront();
	Vector3	right = GetRight();
	Vector3	p_pos = GetPos();
	SetMove(Vector3(0.0f, move.y, 0.0f));

	//	���ݒ�
	SetArmTransform();

	//���[�V�����A�g�f�i�I�X(���傤�ǂ��������̃t���[����������U���J�n)
	if (obj->GetFrame() >= 339 && obj->GetFrame() < 399)
	{
		float t = GetBezier(ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attackInfo.t);
		Vector3 f = front * (2.0f * sinf(D3DX_PI * t));
		Vector3 r = -right * (2.0f * cosf(D3DX_PI * t));
		attackInfo.bottom = p_pos + f + r;

		switch (HyperStep)
		{
		case 0:
			//	�����蔻��̃p�����[�^��^����
			attackInfo.top = attackInfo.bottom + r * HyperRate;
			attackInfo.r = 2.5f;
			HyperRate += 1.0;
			if (HyperRate > 20.0f) HyperStep++;
			break;
		case 1:
			//	�p�����[�^���Z
			attackInfo.top = attackInfo.bottom + f * HyperRate + r * HyperRate;
			attackInfo.t += 0.03f;
			if (attackInfo.t >= 1.0f)
			{
				HyperStep++;
			}
			break;
		case 2:
			//attackInfo.top = attackInfo.bottom + r * rate;
			attackInfo.t = 0.0f;
			HyperRate -= 1.0f;
			break;
		}
		armRenderflag = true;
	}

	Vector3 v1, v2;
	v1 = front;
	v2 = attackInfo.top - attackInfo.bottom;
	v1.Normalize();
	v2.Normalize();
	float armAngle = GetAngle(v1, v2);
	Vector3 cross;
	Vector3Cross(cross, v1, v2);
	if (cross.y < 0) armAngle = -armAngle;

	arm->SetPos(pos);
	arm->SetAngle(angle.y + armAngle);
	arm->SetScale(Vector3(0.03f, 0.03f, HyperRate * 0.01f));
	arm->Update();

	//���[�V�����A�g�f�i�I�X(�I���̃��[�V������������I��)
	if (/*obj->GetFrame() == 399*/ HyperRate < 0)
	{
		HyperStep = 0;
		HyperRate = 0;
		armRenderflag = false;
		initflag = false;
		return true;
	}
	return	false;
}


//	���[�V�����Ǘ�
/*void	Thief::MotionManagement(int motion)
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
}*/

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------
void	Thief::SetArmTransform(void)
{

}

void	Thief::SetHandTransform(void)
{

}

//	�U���p�p�����[�^�ݒ�
void	Thief::SetAttackParam(int attackKind)
{
	switch (attackKind)
	{
	case MODE_STATE::QUICKARTS:
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		if (attackInfo.t < 0.6) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hit�܂ł͋�����̂�
		if (attackInfo.t >= 0.6) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit�ڂ���͐�����΂�����
		break;

	case MODE_STATE::POWERARTS:
		attackInfo.type = Collision::SPHEREVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
		break;

	case MODE_STATE::HYPERARTS:
		attackInfo.type = Collision::CAPSULEVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}