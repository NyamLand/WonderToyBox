
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"GameManager.h"
#include	"Camera.h"
#include	"Effect.h"
#include	"Scavenger.h"

//*********************************************************************************
//
//	Scavenger�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------
#define	MIN_INPUT_STATE	300	//	�X�e�B�b�N����ŏ��l

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
	scale = 0.02f;
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
		obj = new iex3DObj( "DATA/CHR/majo/majo.IEM" );

	//	�X�P�[���ݒ�
	obj->SetScale( 0.02f );
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

	////	�f�o�b�O�p
	//if (!debug)	return;
	//DrawSphere(attackInfo.pos, attackInfo.r, 0xFFFFFFFF);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z - attackInfo.r), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z + attackInfo.r), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x + attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);
	//particle->BlueFlame(Vector3(attackInfo.pos.x - attackInfo.r, attackInfo.pos.y, attackInfo.pos.z), 0.3f);

	//char	str[256];
	//Vector3	stringPos;
	//WorldToClient(pos, stringPos, matView* matProjection);
	//stringPos.y -= 150.0f;
	//sprintf_s(str, "�|\n��\n��\n��");
	//DrawString(str, (int)stringPos.x, (int)stringPos.y);
}

//-----------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------

//	�N�C�b�N�A�[�c
bool	Scavenger::QuickArts(void)
{
	power = QUICK;

	if (obj->GetFrame() < SCAVENGER::MOTION_FRAME::QUICKARTS_SUCK)	SetMotion(SCAVENGER::MOTION_DATA::QUICK_START);
	//if (obj->GetFrame() >= SCAVENGER::MOTION_FRAME::SUCK)	SetMotion(SCAVENGER::MOTION_DATA::QUICK);

	////���s�ړ�
	ShiftMove();
	//���̏��]
	//RollAngle();

	SetUnrivaled(false);

	if (absorb_length < 15.0f) absorb_length += 0.1f;		//�z�����ޔ͈͂����X�Ɋg��
	Vector3 p_front = Vector3(sinf(this->angle.y), 0, cosf(this->angle.y));

	//	�R�C�����擾
	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		bool	state = ( *it )->GetState();
		if ( state )
		{
			Vector3 toCoinVec = ( *it )->GetPos() - this->pos;
			float pVecLength = p_front.Length();
			float cVecLength = toCoinVec.Length();
			float dot = Vector3Dot( p_front, toCoinVec ) / ( pVecLength * cVecLength );
			dot = acos( dot );
			dot = dot * 180.0f / D3DX_PI;

			Vector3 vec = ( *it )->GetPos() - this->pos;
			float length = vec.Length();

			vec.Normalize();
			if ( dot < 45.0f && length < absorb_length ) 
			{
				( *it )->SetMove( -vec * 0.2f );
			}

			particle->Suck(this->pos, this->pos + GetFront() * absorb_length, GetRight(), absorb_length, 0.5f);
		}
	}

	if ( input->Get( KEY_D ) == 2 )
	{
		SetMotion(SCAVENGER::MOTION_DATA::QUICK_END);
		absorb_length = DEFAULT_ABSORB_LENGTH;
	}


	if(obj->GetFrame() == SCAVENGER::MOTION_FRAME::QUICKARTS_END) return true;

	return	false;
}

//	�p���[�A�[�c
bool	Scavenger::PowerArts( void )
{	
	power = POWER;

	//�U�����[�V�����łȂ���΃��[�V�����ݒ�
	if(obj->GetFrame() < SCAVENGER::MOTION_FRAME::POWER_TO_WAIT) SetMotion(SCAVENGER::MOTION_DATA::POWER_START);


	SetUnrivaled(false);
	absorb_length = 5.0f;
	Vector3 p_front = Vector3(sinf(this->angle.y), 0, cosf(this->angle.y));
	p_front.Normalize();
	float speed = 0.5f;
	

	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		bool	state = ( *it )->GetState();
		if ( state )
		{
			Vector3 toCoinVec = ( *it )->GetPos() - this->pos;
			float pVecLength = p_front.Length();
			float cVecLength = toCoinVec.Length();
			float dot = Vector3Dot( p_front, toCoinVec ) / ( pVecLength * cVecLength );
			dot = acosf( dot );
			dot = dot * 180.0f / D3DX_PI;

			Vector3 vec = ( *it )->GetPos() - this->pos;
			float length = vec.Length();

			vec.Normalize();
			if ( dot < 90.0f && length < absorb_length)
			{
				( *it )->SetMove( -vec * 2.0f );
			}
		}
	}

	if (attackInfo.t < 1.0f)
	{
		//�U�������[�V�����Œ�
		if (obj->GetFrame() >= SCAVENGER::MOTION_FRAME::POWER_TO_WAIT) obj->SetFrame(SCAVENGER::MOTION_FRAME::POWER_TO_WAIT);
		//	���
		//particle->Dust(this->pos, GetFront()*-1, GetRight(), 0.5f);
		particle->Semicircle(this->pos, GetFront(), GetRight(), GetUp(), 0.7f);

	
		move = p_front * speed;
	}
	//	�p�����[�^���Z
	attackInfo.t += 0.03f;

	if (attackInfo.t >= 1.0f)
	{
		SetMotion(SCAVENGER::MOTION_DATA::POWER_END);
	}
	
	//���[�V�����I������MOVE�֖߂�
	if (obj->GetFrame() == SCAVENGER::MOTION_FRAME::POWERARTS_END)
	{
		return	true;
	}
	return	false;
}

//	�n�C�p�[�A�[�c
bool	Scavenger::HyperArts( void )
{
	power = HYPER;

	if (obj->GetFrame() <= SCAVENGER::MOTION_FRAME::HYPER_BEGIN) SetMotion(SCAVENGER::MOTION_DATA::HYPER_START);

	//���G�����؂�Ȃ��Ƃ��������R�C�����W�߂��Ȃ��̂Ŗ��G�؂��Ă܂��B
	//���Ȃ猾���Ă�������
	SetUnrivaled(false);
	absorb_length = 20.0f;

	m_Effect->StormSet(this->pos + Vector3(0.0f, 2.0f, 0.0f) , 2 * SECOND);
	list<Coin*>	coinList = coinManager->GetList();
	FOR_LIST( coinList.begin(), coinList.end() )
	{
		bool	state = ( *it )->GetState();
		if ( state )
		{
			Vector3 vec = ( *it )->GetPos() - this->pos;
			
			//vec.Normalize();
			float length = vec.Length();
			if ( length < absorb_length )
			{
				(*it)->SetAbsorbedFlag(true);
				//���߂̈�t���[�������͂�^����
				if (obj->GetFrame() == SCAVENGER::MOTION_FRAME::HYPER_BEGIN)
				{
					Vector3 parabola_move;
					Parabola(parabola_move, (*it)->GetPos(), pos, 0.7f, GRAVITY);
					(*it)->SetMove(parabola_move);
				}
			}
		}

	}
	//	�p�����[�^���Z
	attackInfo.t += 0.01f;


	if (attackInfo.t < 1.0f)
	{
		if (obj->GetFrame() >= SCAVENGER::MOTION_FRAME::HYPER_ABSORB) obj->SetFrame(SCAVENGER::MOTION_FRAME::HYPER_ABSORB);
	}

	if (attackInfo.t >= 1.0f)
	{
		absorb_length = DEFAULT_ABSORB_LENGTH;
	}

	if (obj->GetFrame() == SCAVENGER::MOTION_FRAME::HYPER_FINISH)
	{
		return true;
	}

	return	false;
}
void	Scavenger::ShiftMove(void)
{
	//	���X�e�B�b�N�̓��̓`�F�b�N
	float	axisX = (float)input->Get(KEY_AXISX);
	float	axisY = (float)input->Get(KEY_AXISY);
	float	length = sqrtf(axisX * axisX + axisY * axisY);

	//	�J�����̑O�����������߂�
	Vector3	vEye(mainView->GetTarget() - mainView->GetPos());
	float	cameraAngle = atan2f(vEye.x, vEye.z);

	//	���͕��������߂�
	float inputAngle = atan2f(axisX, axisY);

	//	�ڕW�̊p�x�����߂�
	float	targetAngle = cameraAngle + inputAngle;

	Vector3 direction = Vector3(sinf(targetAngle), 0.0f, cosf(targetAngle));

	moveVec = atan2f(direction.x, direction.z);

	if (length > MIN_INPUT_STATE)
	{
		if (!slip.state)
		{
			move.x = sinf(moveVec) * speed / 4;
			move.z = -cosf(moveVec) * speed / 4;
		}
		else
		{
			if (move.Length() < speed)
			{
				move.x += sinf(moveVec) * slipInfo.speed / 4;
				move.z += cosf(moveVec) * slipInfo.speed / 4;
			}
		}
	}
	else
	{
		SetDrag(0.8f);
	}
}


void	Scavenger::RollAngle(void)
{
	//	���͕��������߂�
	float	axisX = (float)input->Get(KEY_AXISX);

	if (axisX > 0)
	{
		angle.y += 0.02f;
	}
	if (axisX < 0)
	{
		angle.y -= 0.02f;
	}
}
//	���[�V�����Ǘ�
/*void	Scavenger::MotionManagement(int motion)
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
		obj->SetMotion( MOTION_DATA::JUMP );
		break;

	case MOTION_NUM::GUARD:
		obj->SetMotion( MOTION_DATA::GUARD );
		break;

	case MOTION_NUM::LANDING:
		obj->SetMotion( MOTION_DATA::STAND );
		break;

	case MOTION_NUM::RUN:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::ATTACK1:
		obj->SetMotion(MOTION_DATA::POSTURE);
		break;

	case MOTION_NUM::ATTACK2:
		obj->SetMotion( MOTION_DATA::RUN );
		break;

	case MOTION_NUM::ATTACK3:
		obj->SetMotion( MOTION_DATA::ATTACK3 );
		break;
	}
}*/

//-----------------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------------

//	�U���p�p�����[�^�ݒ�
void	Scavenger::SetAttackParam(int attackKind)
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
		attackInfo.type = Collision::SPHEREVSCYRINDER;
		knockBackInfo.type = KNOCKBACK_TYPE::STRENGTH;
		break;
	}
}