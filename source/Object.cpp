
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Sound.h"
#include	"CharacterManager.h"
#include	"Object.h"

//*******************************************************************************
//
//	Object�N���X
//
//*******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Object::Object( void ) : obj( nullptr )
	{
	
	}

	//	�f�X�g���N�^
	Object::~Object( void )
	{
		Release();
	}

	//	������
	bool	Object::Initialize( void )
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
	
	//	���
	void	Object::Release( void )
	{
		SafeDelete( obj );
	}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

	//	�X�V
	void	Object::Update()
	{
		//	����
		Move();

		//	���薳������
		if ( judgeTimer > 0 )	judgeTimer--;
		else							activate = true;

		//	�ړ��l���Z
		pos += move;
		
		//	�e�����蔻��
		StageCollisionCheck();
		PlayerCollisionCheck();

		//	���X�V
		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	�`��
	void	Object::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader == nullptr || technique == nullptr )
		{
			obj->Render();
		}
		else
		{
			obj->Render( shader, technique );
		}
	}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	����
	void	Object::Move( void )
	{
		//	�d�͉��Z
		move.y += GRAVITY;

		//	��]
		angle += 0.05f;

		//	����������Ĕz�u
		if ( GetPos().y <= -3.0f )
		{
			float x = Random::GetFloat( -10.0f, 10.0f );
			float y = Random::GetFloat( 0.0f, 50.0f );
			float z = Random::GetFloat( -10.0f, 10.0f );
			SetPos( Vector3( x, y, z ) );
		}
	}

	//	�X�e�[�W�����蔻��
	void	Object::StageCollisionCheck( void )
	{
		float	work = Collision::GetHeight( pos );

		if ( pos.y < work )
		{
			pos.y = work;
			move.y = 0;
		}
	}

	//	�v���C���[�����蔻��
	void	Object::PlayerCollisionCheck( void )
	{
		Vector3	p_pos[PLAYER_MAX];
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			if ( characterManager->GetUnrivaled( i ) )	continue;
			p_pos[i] = characterManager->GetPos( i );
			bool	isHit = Collision::CapsuleVSSphere( p_pos[i], Vector3( p_pos[i].x, p_pos[i].y + 3.0f, p_pos[i].z ), 1.0f, Vector3( pos.x, pos.y + 0.5f, pos.z ), 0.5f );

			if ( isHit )
			{
				HitDuaringTheOperation( p_pos[i], i );
			}
		}
	}

	//	�q�b�g������
	void	Object::HitDuaringTheOperation( const Vector3& pos, const int& num )
	{
		state = false;
		float	effectScale = 0.2f;
		particle->Spark( pos, effectScale );
		gameManager->AddCoin( num );
		sound->PlaySE( SE::COIN_SE );
	}

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------

	//	���W�擾
	Vector3	Object::GetPos( void )const
	{
		return	this->pos;
	}

	//	�����擾
	float		Object::GetAngle( void )const
	{
		return	this->angle;
	}

	//	�X�P�[���擾
	bool		Object::GetState( void )const
	{
		return	this->state;
	}

//-------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------

	//	���b�V���ݒ�
	void	Object::SetObj( iexMesh* obj )
	{
		this->obj = obj;
	}

	//	���W�ݒ�
	void	Object::SetPos( Vector3 pos )
	{
		this->pos = pos;
	}

	//	�����ݒ�
	void	Object::SetAngle( float angle )
	{
		this->angle = angle;
	}

	//	�X�P�[���ݒ�
	void	Object::SetScale( float scale )
	{
		this->scale = scale;
	}

	//	

