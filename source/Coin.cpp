
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Sound.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"

#include	"Coin.h"

//******************************************************************************
//
//	Coin�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------

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
		obj = nullptr;
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

		//	�f�o�b�O�p
		if ( !debug )	return;
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
		if ( pos.y >= 15.0f )		return;
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
			if ( m_Player->GetUnrivaled( i ) )	continue;
			p_pos[i] = m_Player->GetPos( i );
			bool isHit = Collision::CapsuleVSSphere( p_pos[i],Vector3( p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z ), 1.0f, Vector3( pos.x, pos.y + 0.5f, pos.z ), 0.5f );

			if ( isHit )
			{
				Hitduringtheoperation(p_pos[i],i);
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

		//	����������Ĕz�u
		if ( GetPos().y <= -3.0f )
		{
			std::uniform_real_distribution<float> posrand( -10.0f, 10.0f );
			std::uniform_real_distribution<float> heightrand( 0.0f, 50.0f );
			SetPos( Vector3( Random::GetFloat( -10.0f, 10.0f ), Random::GetFloat( 0.0f, 50.0f ), Random::GetFloat( -10.0f, 10.0f ) ) );
		}
	}

	//	�q�b�g������
	void	Coin::Hitduringtheoperation(const Vector3& pos, const int& Num)
	{
		state = false;
		float	effectScale = 0.2f;
		particle->Spark(pos, effectScale);
		GameManager::AddCoin(Num);
		sound->PlaySE( SE::COIN_SE );
	}

//-------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------

	//	���W�ݒ�
	void	Coin::SetPos( const Vector3& pos )
	{
		this->pos = pos; 
	}

	//	�����ݒ�
	void	Coin::SetAngle( const float& angle )
	{
		this->angle = angle; 
	}

	//	�X�P�[���ݒ�
	void	Coin::SetScale( const float& scale )
	{
		this ->scale = scale; 
	}

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------

	//	���W�擾
	Vector3	Coin::GetPos( void )
	{
		Vector3	out = this->pos;
		return	out; 
	}

	//	�����擾
	float		Coin::GetAngle( void )
	{
		float	out = this->angle;
		return	out; 
	}
	
	//	��Ԏ擾
	bool		Coin::GetState()
	{
		return	state;
	}