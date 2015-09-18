
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"

#include	"Item.h"

//******************************************************************************
//
//	Item�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

//	�R���X�g���N�^
Item::Item(void) : obj(NULL)
{

}

//	�f�X�g���N�^
Item::~Item(void)
{
	SafeDelete(obj);
}

//	������
bool	Item::Initialize(void)
{
	obj = NULL;
	angle = 0.0f;
	pos = Vector3(0.0f, 0.0f, 0.0f);
	move = Vector3(0.0f, 0.0f, 0.0f);
	scale = 0.5f;
	judgeTimer = 0;
	activate = false;
	state = false;

	return	true;
}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

//	�X�V
void	Item::Update(void)
{
	//	����
	Move();

	if (judgeTimer > 0)	judgeTimer--;
	else							activate = true;

	pos += move;
	StageCollisionCheck();
	PlayerCollisionCheck();

	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->SetScale(scale);
	obj->Update();
}

//	�`��
void	Item::Render(void)
{
	obj->Render();

	//	�f�o�b�O�p
	if (!debug)	return;
	DrawSphere(Vector3(pos.x, pos.y + 0.5f, pos.z), 0.5f, 0xFFFF0000);
}

//	�V�F�[�_�[�t���`��
void	Item::Render(iexShader* shader, LPSTR technique)
{
	obj->Render(shader, technique);
}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

//	�X�e�[�W�����蔻��`�F�b�N
void	Item::StageCollisionCheck(void)
{
	float work = Collision::GetHeight(pos);

	if (pos.y <= work)
	{
		pos.y = work;
		move.y = 0;
	}
}

//	�v���C���[�Ƃ̂�����`�F�b�N
void	Item::PlayerCollisionCheck(void)
{
	Vector3	p_pos[4];
	for (int i = 0; i < 4; i++)
	{
		if (!activate)	continue;
		if (m_Player->GetUnrivaled(i))	continue;
		p_pos[i] = m_Player->GetPos(i);
		bool isHit = Collision::CapsuleVSSphere(p_pos[i], Vector3(p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z), 1.0f, Vector3(pos.x, pos.y + 0.5f, pos.z), 0.5f);

		if (isHit)
		{
			state = false;
			float	effectScale = 0.2f;
			Particle::Spark(p_pos[i], effectScale);
			//m_Player->AddCoin( i );
			GameManager::AddCoin(i);
			IEX_PlaySound(SoundInfo::COIN_SE, false);
		}
	}
}

//	����
void	Item::Move(void)
{
	//	�d�͉��Z
	move.y += GRAVITY;

	//	��]
	angle += 0.05f;

	// ����( �X�e�[�W )	
	static float rate = 0.4f;
	Collision::GetReflect(pos, move, rate);
}

//-------------------------------------------------------------------------------
//	���ݒ�E�擾
//-------------------------------------------------------------------------------

//	�ݒ�
void	Item::SetPos(Vector3 pos){ this->pos = pos; }
void	Item::SetAngle(float angle){ this->angle = angle; }
void	Item::SetScale(float scale){ this->scale = scale; }

//	�擾
Vector3	Item::GetPos(void){ return	this->pos; }
float		Item::GetAngle(void){ return	this->angle; }
