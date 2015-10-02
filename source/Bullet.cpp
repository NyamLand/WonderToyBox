
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"Coin.h"
#include	"CoinManager.h"

#include	"Bullet.h"

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
	Bullet::Bullet( void ) : obj( NULL )
	{

	}

	//	�f�X�g���N�^
	Bullet::~Bullet( void )
	{
		SafeDelete( obj );
	}

	//	������
	bool	Bullet::Initialize( void )
	{
		obj = NULL;
		angle = 0.0f;
		pos = Vector3( 0.0f, 0.0f, 0.0f );
		move = Vector3( 0.0f, 0.0f, 0.0f );
		scale = 0.05f;
		judgeTimer = 0;
		limitTimer = 0;
		activate = false;
		state = false;
		number = 0;
		leanpower = 0;

		return	true;
	}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

	//	�X�V
	void	Bullet::Update( void )
	{
		//	����
		Move();

		if ( judgeTimer > 0 )	judgeTimer--;
		else							activate = true;

		limitTimer++;

		pos += move;
		StageCollisionCheck();
		PlayerCollisionCheck();

		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	�`��
	void	Bullet::Render( void )
	{
		obj->Render();
		DrawSphere( Vector3( pos.x, pos.y + 0.5f, pos.z ), scale * 5, 0xFFFF0000 );
	}

	//	�V�F�[�_�[�t���`��
	void	Bullet::Render( iexShader* shader, LPSTR technique )
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	�X�e�[�W�����蔻��`�F�b�N
	void	Bullet::StageCollisionCheck( void )
	{
		float work = Collision::GetHeight( pos );

		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
		}
	}

	//	�v���C���[�Ƃ̂�����`�F�b�N
	void	Bullet::PlayerCollisionCheck( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			if ( !activate )	continue;
			if ( m_Player->GetUnrivaled( i ) )	continue;
			
			//	�v���C���[���ݒ�
			Vector3	p_pos_bottom = m_Player->GetPos(i);
			Vector3	p_pos_top = Vector3( p_pos_bottom.x, p_pos_bottom.y + 3.0f, p_pos_bottom.z );
			float		p_r = 1.0f;
			
			//	�o���b�g���ݒ�
			Vector3	bulletPos = GetPos();
			bulletPos.y += 0.5f;
			float		bullet_r = scale * 5.0f;

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
				Vector3	color = m_Player->GetDamageColor(i);
				m_Player->SetReceiveColor(i,color);
				m_Player->SetKnockBackVec(i, -knockBackVec);
				m_Player->SetLeanFrame(i, leanpower);
				m_Player->SetMode( i, MODE_STATE::DAMAGE_LEANBACKWARD );

				//	�R�C���΂�܂������ݒ�
				std::uniform_real_distribution<float>	vecrand( -1.0f, 1.0f );
				Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
				vec.Normalize();

				//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
				float	power = 0.2f;
				int		p2_Num = m_Player->GetP_Num( i );
				int		p2_coinNum = gameManager->GetCoinNum( p2_Num );

				//	�R�C��������΂΂�܂�
				if ( p2_coinNum > 0 )
				{
					m_CoinManager->Set( p_pos_top, vec, power );
					gameManager->SubCoin( p2_Num );
				}
			}
		}
	}

	//	����
	void	Bullet::Move( void )
	{
		// ����( �X�e�[�W )	
		static float rate = 0.4f;
		Collision::GetReflect( pos, move, rate );
	}

//-------------------------------------------------------------------------------
//	���ݒ�E�擾
//-------------------------------------------------------------------------------

	//	�ݒ�
	void	Bullet::SetPos( Vector3 pos ){ this->pos = pos; }
	void	Bullet::SetAngle( float angle ){ this->angle = angle; }
	void	Bullet::SetScale( float scale ){ this->scale = scale; }

	//	�擾
	Vector3	Bullet::GetPos( void ){ return	this->pos; }
	float		Bullet::GetAngle( void ){ return	this->angle; }
