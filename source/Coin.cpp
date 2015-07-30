
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"

#include	"Coin.h"

//******************************************************************************
//
//	Coin�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Coin::Coin( void ) : obj( NULL )
	{
		
	}

	//	�f�X�g���N�^
	Coin::~Coin( void )
	{
		SafeDelete( obj );
	}

	//	������
	bool	Coin::Initialize( void )
	{
		obj = NULL;
		angle = 0.0f;
		pos = Vector3( 0.0f, 0.0f, 0.0f );
		move = Vector3( 0.0f, 0.0f, 0.0f );
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
	void	Coin::Update( void )
	{
		//	����
		Move();

		if ( judgeTimer > 0 )	judgeTimer--;
		else							activate = true;

		pos += move;
		StageCollisionCheck();
		PlayerCollisionCheck();

		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	�`��
	void	Coin::Render( void )
	{
		obj->Render();
		DrawSphere( Vector3( pos.x, pos.y + 0.5f, pos.z ), 0.5f, 0xFFFF0000 );
	}

	//	�V�F�[�_�[�t���`��
	void	Coin::Render( iexShader* shader, LPSTR technique )
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	�X�e�[�W�����蔻��`�F�b�N
	void	Coin::StageCollisionCheck( void )
	{
		float work = Collision::GetHeight( pos );

		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
		}
	}

	//	�v���C���[�Ƃ̂�����`�F�b�N
	void	Coin::PlayerCollisionCheck( void )
	{
		Vector3	p_pos[4];
		for ( int i = 0; i < 4; i++ )
		{
			if ( !activate )	continue;
			p_pos[i] = m_Player->GetPos( i );
			bool isHit = Collision::CapsuleVSSphere( p_pos[i],Vector3( p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z ), 1.0f, Vector3( pos.x, pos.y + 0.5f, pos.z ), 0.5f );

			if ( isHit )
			{
				state = false;
				float	effectScale = 0.2f;
				Particle::Spark( p_pos[i], effectScale );
				m_Player->AddCoin( i );
			}
		}
	}

	//	����
	void	Coin::Move( void )
	{
		//	�d�͉��Z
		move.y += GRAVITY;
		
		//	��]
		angle += 0.05f;

		// ����( �X�e�[�W )	
		static float rate = 0.4f;
		Collision::GetReflect( pos, move, rate );
	}

//-------------------------------------------------------------------------------
//	���ݒ�E�擾
//-------------------------------------------------------------------------------

	//	�ݒ�
	void	Coin::SetPos( Vector3 pos ){ this->pos = pos; }
	void	Coin::SetAngle( float angle ){ this->angle = angle; }
	void	Coin::SetScale( float scale ){ this ->scale = scale; }

	//	�擾
	Vector3	Coin::GetPos( void ){ return	this->pos; }
	float		Coin::GetAngle( void ){ return	this->angle; }
	