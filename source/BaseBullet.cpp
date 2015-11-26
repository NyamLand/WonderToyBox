
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"CoinManager.h"

#include	"BaseBullet.h"

//******************************************************************************
//
//	Bullet�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
BaseBullet::BaseBullet(void) :
obj(NULL),
pos(0, 0, 0), move(0, 0, 0),
limitTimer(0),  number(0), leanpower(0), playerNum(0), liveTime(0), step(0), 
radius(0),
scale(0,0,0), angle(0,0,0),
state(false), activate(false)
	{
	}

	//	�f�X�g���N�^
	BaseBullet::~BaseBullet(void)
	{
		SafeDelete( obj );
	}

	//	������
	//bool	BaseBullet::Initialize(void)
	//{
	//	obj = NULL;
	//	angle = 0.0f;
	//	pos = Vector3( 0.0f, 0.0f, 0.0f );
	//	move = Vector3( 0.0f, 0.0f, 0.0f );
	//	scale = 0.05f;
	//	judgeTimer = 0;
	//	limitTimer = 0;
	//	activate = false;
	//	state = false;
	//	number = 0;
	//	leanpower = 0;

	//	return	true;
	//}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------



	//	�`��
	void	BaseBullet::Render(void)
	{
		obj->Render();
		DrawSphere( Vector3( pos.x, pos.y, pos.z ), scale.y * radius, 0xFFFF0000 );
	}

	//	�V�F�[�_�[�t���`��
	void	BaseBullet::Render(iexShader* shader, LPSTR technique)
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	�X�e�[�W�����蔻��`�F�b�N
	bool	BaseBullet::StageCollisionCheck(void)
	{
		float work = Collision::GetHeight( pos );

		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
			return true;
		}
	return false;
	}

	//	�v���C���[�Ƃ̂�����`�F�b�N
	bool	BaseBullet::PlayerCollisionCheck(void)
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( !activate )	continue;
			if ( characterManager->GetUnrivaled( i ) )	continue;
			
			//	�v���C���[���ݒ�
			Vector3	p_pos_bottom = characterManager->GetPos(i);
			Vector3	p_pos_top = Vector3( p_pos_bottom.x, p_pos_bottom.y + 3.0f, p_pos_bottom.z );
			float		p_r = 1.0f;
			
			//	�o���b�g���ݒ�
			Vector3	bulletPos = GetPos();
			//bulletPos.y += 0.5f;
			float		bullet_r = scale.y * radius;

			bool isHit = Collision::CapsuleVSSphere( p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r );

			if ( isHit )
			{
				//	�G�t�F�N�g����
				state = false;
				float	effectScale = 0.2f;
				particle->Spark( p_pos_top, effectScale );

				//	�m�b�N�o�b�N
				Vector3	knockBackVec = bulletPos - p_pos_bottom;
				knockBackVec.y = p_pos_bottom.y;
				knockBackVec.Normalize();
				Vector3	color = characterManager->GetDamageColor(i);
				characterManager->SetPassColor(i,color);
				characterManager->SetKnockBackVec(i, -knockBackVec);
				characterManager->SetLeanFrame(i, leanpower);
				characterManager->SetMode( i, MODE_STATE::DAMAGE_LEANBACKWARD );

				//	�R�C���΂�܂������ݒ�
				std::uniform_real_distribution<float>	vecrand( -1.0f, 1.0f );
				Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
				vec.Normalize();

				//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
				float	power = 0.2f;
				int		p2_Num = characterManager->GetPlayerNum( i );
				int		p2_coinNum = gameManager->GetCoinNum( p2_Num );

				//	�R�C��������΂΂�܂�
				if ( p2_coinNum > 0 )
				{
					coinManager->Append( p_pos_top, vec, power );
					gameManager->SubCoin( p2_Num );
				}
				return true;
			}
		}
	return false;
	}

//-------------------------------------------------------------------------------
//	���ݒ�E�擾
//-------------------------------------------------------------------------------

	//	�ݒ�
	void	BaseBullet::SetPos(Vector3 pos){ this->pos = pos; }
	void	BaseBullet::SetAngle(Vector3 angle){ this->angle = angle; }
	void	BaseBullet::SetScale(Vector3 scale){ this->scale = scale; }

	//	�擾
	Vector3	BaseBullet::GetPos(void){ return	this->pos; }
	Vector3		BaseBullet::GetAngle(void){ return	this->angle; }


	//	�O���擾
	Vector3	BaseBullet::GetFront(void)
	{
		Matrix	mat = obj->TransMatrix;
		Vector3	out = Vector3(mat._31, mat._32, mat._33);
		out.Normalize();
		return	out;
	}

	//	�E���擾
	Vector3	BaseBullet::GetRight(void)
	{
		Matrix	mat = obj->TransMatrix;
		Vector3	out = Vector3(mat._11, mat._12, mat._13);
		out.Normalize();
		return	out;
	}

	//	����擾
	Vector3	BaseBullet::GetUp(void)
	{
		Matrix	mat = obj->TransMatrix;
		Vector3	out = Vector3(mat._21, mat._22, mat._23);
		out.Normalize();
		return	out;
	}


	float BaseBullet::GetDegreeAngle(Vector3 vec1, Vector3 vec2)
	{
		float out;
		out = Vector3Dot(vec1, vec2) / (vec1.Length() * vec2.Length());
		out = acos(out);
		out = out * 180.0f / PI;
		return out;
	}