
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"

#include	"Bullet.h"

//******************************************************************************
//
//	Bullet�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

//	�R���X�g���N�^
Bullet::Bullet(void) : obj(NULL)
{

}

//	�f�X�g���N�^
Bullet::~Bullet(void)
{
	SafeDelete(obj);
}

//	������
bool	Bullet::Initialize(void)
{
	obj = NULL;
	angle = 0.0f;
	pos = Vector3(0.0f, 0.0f, 0.0f);
	move = Vector3(0.0f, 0.0f, 0.0f);
	scale = 0.05f;
	judgeTimer = 0;
	limitTimer = 0;
	activate = false;
	state = false;

	return	true;
}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

//	�X�V
void	Bullet::Update(void)
{
	//	����
	Move();

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	limitTimer++;

	pos += move;
	StageCollisionCheck();
	PlayerCollisionCheck();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}

//	�`��
void	Bullet::Render(void)
{
	obj->Render();
	DrawSphere(Vector3(pos.x, pos.y + 0.5f, pos.z), scale * 5, 0xFFFF0000);
}

//	�V�F�[�_�[�t���`��
void	Bullet::Render(iexShader* shader, LPSTR technique)
{
	obj->Render(shader, technique);
}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

//	�X�e�[�W�����蔻��`�F�b�N
void	Bullet::StageCollisionCheck(void)
{
	float work = Collision::GetHeight(pos);

	if (pos.y <= work)
	{
		pos.y = work;
		move.y = 0;
	}
}

//	�v���C���[�Ƃ̂�����`�F�b�N
void	Bullet::PlayerCollisionCheck(void)
{
	Vector3	p_pos[4];
	for (int i = 0; i < 4; i++)
	{
		if (!activate)	continue;
		if (m_Player->GetUnrivaled(i))	continue;
		p_pos[i] = m_Player->GetPos(i);
		bool isHit = Collision::CapsuleVSSphere(p_pos[i], Vector3(p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z), 1.0f, Vector3(pos.x, pos.y + 0.5f, pos.z), scale * 5);

		if (isHit)
		{
			state = false;
			float	effectScale = 0.2f;
			Particle::Spark(p_pos[i], effectScale);
			printf("a");
		}
	}
}

//	����
void	Bullet::Move(void)
{

	// ����( �X�e�[�W )	
	static float rate = 0.4f;
	Collision::GetReflect(pos, move, rate);
}

//-------------------------------------------------------------------------------
//	���ݒ�E�擾
//-------------------------------------------------------------------------------

//	�ݒ�
void	Bullet::SetPos(Vector3 pos){ this->pos = pos; }
void	Bullet::SetAngle(float angle){ this->angle = angle; }
void	Bullet::SetScale(float scale){ this->scale = scale; }

//	�擾
Vector3	Bullet::GetPos(void){ return	this->pos; }
float		Bullet::GetAngle(void){ return	this->angle; }
