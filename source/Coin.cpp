
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Sound.h"
#include	"Particle.h"

#include	"Coin.h"

//******************************************************************************
//
//	Coin�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------

#define	GETAWAY_LENGTH		3.0f	//	�����锻�苗��
#define	MAX_HEIGHT					50.0f	//	�z�肵�Ă��鍂���ő�l

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Coin::Coin( void ) : obj( nullptr )
	{
		
	}

	//	�f�X�g���N�^
	Coin::~Coin( void )
	{
		SafeDelete( obj );
		SafeDelete( shadow.obj );
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
		getAwayflag = false;

		//	�e�\���̏�����
		{
			shadow.obj = new iex2DObj( "DATA/Effect/shadow.png" );
			shadow.pos = pos;
			shadow.scale = 1.0f;
			SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, 0xFFFFFFFF );
		}

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

		//	������
		if ( getAwayflag )	GetAway();

		//	�}�O�l�b�g
		Magnet();

		//	���薳�����Ԍ��Z
		if ( judgeTimer > 0 )	judgeTimer--;
		else							activate = true;

		//	�ړ��l���Z
		pos += move;

		//	�e���W�ݒ�
		shadow.pos = pos;

		//	�[������
		move.x *= 0.9f;
		move.z *= 0.9f;

		//	�����蔻��
		StageCollisionCheck();
		PlayerCollisionCheck();
		
		//	�e���X�V
		ShadowUpdate();

		//	���X�V
		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	�e���X�V
	void	Coin::ShadowUpdate( void )
	{
		//	�X�P�[���v�Z( �����ɉ����ĉe�̃X�P�[���𒲐��A�e�̑傫���̍ő�l�̓��f���̑傫����2.5�{�ɐݒ� )
		float	t = pos.y / MAX_HEIGHT;
		static	float	maxScale = scale * 2.5f;
		shadow.scale = maxScale - ( maxScale * t );

		//	���_�Z�b�g
		static	DWORD	vertexColor = 0xFFFFFFFF;
		SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, vertexColor );
		SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, vertexColor );
	}

	//	�`��i �����̂ǂ��炩����ł��ݒ肵�Ȃ�������ʏ�`�� �j
	void	Coin::Render( iexShader* shader, LPSTR technique )
	{
		//	�e�`��
		iexPolygon::Render3D( shadow.v, 2, shadow.obj, shader3D, "alpha" );

		//	���f���`��
		if ( shader == nullptr || technique == nullptr )
			obj->Render();
		else
			obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	�X�e�[�W�����蔻��`�F�b�N
	void	Coin::StageCollisionCheck( void )
	{
		//	�z�肵�Ă�����������Ƃ�ł�����X�L�b�v
		if ( pos.y >= MAX_HEIGHT )	return;
		float work = Collision::GetHeight( pos );
		shadow.pos.y = work + 0.1f;
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
			if ( characterManager->GetUnrivaled( i ) )	continue;
			p_pos[i] = characterManager->GetPos( i );
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
			SetPos( Vector3( Random::GetFloat( -10.0f, 10.0f ), Random::GetFloat( 0.0f, MAX_HEIGHT ), Random::GetFloat( -10.0f, 10.0f ) ) );
		}
	}

	//	�q�b�g������
	void	Coin::Hitduringtheoperation( const Vector3& pos, const int& Num )
	{
		state = false;
		float	effectScale = 0.2f;
		particle->Spark( pos, effectScale );
		gameManager->AddCoin( Num );
		sound->PlaySE( SE::COIN_SE );
	}

	//	����
	void	Coin::GetAway( void )
	{
		Vector3	p_pos[4];
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			p_pos[i] = characterManager->GetPos( i );
			p_pos[i].y = pos.y;

			//	�v���C���[�ւ̃x�N�g���擾
			Vector3	vec = p_pos[i] - pos;
			float	length = vec.Length();

			//	�߂���Γ�����
			if ( length <= GETAWAY_LENGTH )
			{
				vec.Normalize();
				move = -vec * 0.1f;
			}
		}
	}

	//	�}�O�l�b�g
	void	Coin::Magnet( void )
	{
		Vector3	p_pos[4];
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			if ( !characterManager->GetParameterState( i, PARAMETER_STATE::MAGNET ) )	continue;
			p_pos[i] = characterManager->GetPos( i );
			p_pos[i].y = pos.y;

			//	�v���C���[�ւ̃x�N�g���擾
			Vector3	vec = p_pos[i] - pos;
			float	length = vec.Length();

			//	�߂���΋z���񂹂���
			if ( length <= GETAWAY_LENGTH )
			{
				vec.Normalize();
				move = vec * 0.01f;
			}
		}
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

	//	�����t���O�ݒ�
	void	Coin::SetGetAwayFlag( bool flag )
	{
		this->getAwayflag = flag;
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
	bool		Coin::GetState( void )
	{
		return	state;
	}