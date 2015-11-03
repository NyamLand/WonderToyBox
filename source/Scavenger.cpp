
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Scavenger.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"GameManager.h"

//*********************************************************************************
//
//	Scavenger�N���X
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
Scavenger::Scavenger(void) : BaseChara()
{
	//	�p�����[�^������
	power = 3;	/*��*/
	speed = 0.25f;
	scale = 0.05f;
	diffence = -1;
	stayTime = 0;
	absorb_length = DEFAULT_ABSORB_LENGTH;
	isGround = true;
}

//	�f�X�g���N�^
Scavenger::~Scavenger(void)
{

}

//	������
bool	Scavenger::Initialize(int playerNum, Vector3 pos)
{
	//	�e�̏�����
	BaseChara::Initialize( playerNum, pos );

	//	���f���ǂݍ���
	if ( obj == nullptr )
		obj = new iex3DObj( "DATA/CHR/Knight/Knight_Dammy.IEM" );

	//	�X�P�[���ݒ�
	obj->SetScale( 0.05f );
	obj->Update();

	if ( obj == nullptr )	return	false;
	return	true;
}

//-----------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------

//	�`��
void	Scavenger::Render(iexShader* shader, LPSTR technique)
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
	sprintf_s(str, "�|\n��\n��\n��");
	DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

//	�N�C�b�N�A�[�c
bool	Scavenger::QuickArts(void)
{
	//���G�����؂�Ȃ��Ƃ��������R�C�����W�߂��Ȃ��̂Ŗ��G�؂��Ă܂��B
	//���Ȃ猾���Ă�������
	unrivaled = false;

	if (absorb_length < 15.0f) absorb_length += 0.1f;		//�z�����ޔ͈͂����X�Ɋg��
	Vector3 p_front = Vector3(sinf(this->angle), 0, cosf(this->angle));

	Coin* coin = m_CoinManager->GetCoin();
	for (int i = 0; i < 200; i++)
	{
		Vector3 toCoinVec = coin[i].GetPos() - this->pos;
		float pVecLength = p_front.Length();	
		float cVecLength = toCoinVec.Length();
		float dot = Vector3Dot(p_front, toCoinVec) / (pVecLength * cVecLength);
		dot = acos(dot);
		dot = dot * 180.0f / PI;

		if (coin[i].GetState() == true)
		{
			Vector3 vec = coin[i].GetPos() - this->pos;
			float length = vec.Length();
			
			vec.Normalize();
			if ( dot < 45 && length < absorb_length )
			{
				coin[i].SetMove( -vec * 0.2f );
			}
		}
	}
	if ( input->Get( KEY_D ) == 2 )
	{
		absorb_length = DEFAULT_ABSORB_LENGTH;
		return true;
	}

	return	false;
}

//	�p���[�A�[�c
bool	Scavenger::PowerArts(void)
{	
	//���G�����؂�Ȃ��Ƃ��������R�C�����W�߂��Ȃ��̂Ŗ��G�؂��Ă܂��B
	//���Ȃ猾���Ă�������
	unrivaled = false;
	absorb_length = 5.0f;
	Vector3 p_front = Vector3(sinf(this->angle), 0, cosf(this->angle));
	p_front.Normalize();
	float speed = 0.5f;

	Coin* coin = m_CoinManager->GetCoin();
	for (int i = 0; i < 200; i++)
	{
		Vector3 toCoinVec = coin[i].GetPos() - this->pos;
		float pVecLength = p_front.Length();
		float cVecLength = toCoinVec.Length();
		float dot = Vector3Dot(p_front, toCoinVec) / (pVecLength * cVecLength);
		dot = acos(dot);
		dot = dot * 180.0f / PI;

		if (coin[i].GetState() == true)
		{
			Vector3 vec = coin[i].GetPos() - this->pos;
			float length = vec.Length();

			vec.Normalize();
			if (dot < 90 && length < absorb_length && stayTime == 0)
			{
				coin[i].SetMove(-vec * 2.0f);
			}
		}
	}
	if (attackInfo.t < 1.0f) move = p_front * speed;

	//	�p�����[�^���Z
	attackInfo.t += 0.03f;

	if (attackInfo.t >= 1.0f)
	{
		stayTime++;
	}
	
	if (stayTime >= 2 * SECOND)
	{
		stayTime = 0;
		return	true;
	}
	return	false;
}

//	�n�C�p�[�A�[�c
bool	Scavenger::HyperArts(void)
{
	//���G�����؂�Ȃ��Ƃ��������R�C�����W�߂��Ȃ��̂Ŗ��G�؂��Ă܂��B
	//���Ȃ猾���Ă�������
	unrivaled = false;
	absorb_length = 10.0f;
	stayTime++;

	Coin* coin = m_CoinManager->GetCoin();
	for (int i = 0; i < 200; i++)
	{
		if (coin[i].GetState() == true)
		{
			Vector3 vec = coin[i].GetPos() - this->pos;
			vec.Normalize();
			float length = vec.Length();
			if (length < absorb_length)
			{
				coin[i].SetMove(-vec * 1.0f);
			}
		}
	}
	if (stayTime > 2 * SECOND)
	{
		stayTime = 0;
		absorb_length = DEFAULT_ABSORB_LENGTH;
		return true;
	}

	return	false;
}

//	���[�V�����Ǘ�
void	Scavenger::MotionManagement(int motion)
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
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::GUARD:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::LANDING:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::RUN:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::ATTACK1:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::ATTACK2:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::ATTACK3:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;
	}
}

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

//	�U���p�p�����[�^�ݒ�
void	Scavenger::SetAttackParam(int attackKind)
{
	switch (attackKind)
	{
	case MODE_STATE::QUICKARTS:
		knockBackInfo.isUp = false;
		attackInfo.type = COLLISION_TYPE::SPHEREVSCAPSULE;
		if (attackInfo.t < 0.6) knockBackInfo.type = KNOCKBACK_TYPE::LEANBACKWARD;	//2Hit�܂ł͋�����̂�
		if (attackInfo.t >= 0.6) knockBackInfo.type = KNOCKBACK_TYPE::WEAK;		//3hit�ڂ���͐�����΂�����
		break;

	case MODE_STATE::POWERARTS:
		knockBackInfo.isUp = true;
		attackInfo.type = COLLISION_TYPE::SPHEREVSCAPSULE;
		knockBackInfo.type = KNOCKBACK_TYPE::MIDDLE;
		break;

	case MODE_STATE::HYPERARTS:
		knockBackInfo.isUp = false;
		attackInfo.type = COLLISION_TYPE::SPHEREVSCYRINDER;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}