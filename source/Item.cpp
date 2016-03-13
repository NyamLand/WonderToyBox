
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"Sound.h"
#include	"Particle.h"
#include	"CharacterManager.h"
#include	"GameManager.h"
#include	"Stage.h"
#include	"Effect.h"

#include	"Item.h"

//******************************************************************************
//
//	Item�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------
#define	ITEM_SCALE				2.0f
#define	DELETE_TIME				7
#define	ROTATIONAL_SPEED	0.05f
#define	COLLISION_SCALE		2.0f
#define	MAX_HEIGHT				50.0f
#define	RELOCATION_HEIGHT	-3.0f

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Item::Item( void ) : obj( nullptr ),
		pos( ZERO_VECTOR3 ), move( ZERO_VECTOR3 ),
		scale( ITEM_SCALE ), angle( 0.0f ), stageHeight( 0.0f ),
		state( true ),
		type( 0 ),	time( 0 )
	{

	}

	//	�f�X�g���N�^
	Item::~Item(void)
	{
		SafeDelete( shadow.obj );
	}

	//	������5
	bool	Item::Initialize( void )
	{
		Load();
		return	true;
	}

	//	�ǂݍ���
	void	Item::Load( void )
	{
		shadow.obj = new iex2DObj( "DATA/Effect/shadow.png" );
	}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

	//	�X�V
	void	Item::Update( void )
	{
		//	����
		Move();

		//	�ړ��l���Z
		pos += move;

		//	�e���W�ݒ�
		shadow.pos = pos;

		//	�����蔻��
		StageCollisionCheck();
		PlayerCollisionCheck();

		//	�e���X�V
		ShadowUpdate();

		//	�I�u�W�F�N�g�X�V
		ObjectUpdate();
	}

	//	�`��
	void	Item::Render( iexShader* shader, LPSTR technique )
	{
		//	�e�`��
		iexPolygon::Render3D( shadow.v, 2, shadow.obj, shader3D, "alpha" );

		//	���f���`��
		if ( shader == nullptr || technique == nullptr )
		{
			obj->Render();
		}
		else
		{
			obj->Render( shader, technique );
		}
	}

	//	�I�u�W�F�N�g�X�V
	void	Item::ObjectUpdate( void )
	{
		//	���X�V
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->SetPos( pos );
		obj->Update();
	}

	//	�e�X�V
	void	Item::ShadowUpdate( void )
	{
		//	�X�P�[���v�Z�i �����ɉ����ĉe�̃X�P�[���𒲐��A�e�̑傫���̍ő�l�̓��f���̑傫����2.5�{�ɐݒ� �j
		float	t = pos.y / MAX_HEIGHT;
		static	float		MAX_SCALE = scale * 1.2f;
		shadow.scale = MAX_SCALE - ( MAX_SCALE * t );

		//	���_�Z�b�g
		static	DWORD	vertexColor = 0x55FFFFFF;
		SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, vertexColor );
		SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, vertexColor );
	}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	����
	void	Item::Move( void )
	{
		//	�d�͉��Z
		move.y += GRAVITY;

		//	��]
		angle += ROTATIONAL_SPEED;

		//	�^�C�}�[���Z
		time++;

		//	DELETE_TIME�b��ɏ���
		if ( time >= DELETE_TIME * SECOND )	state = false;
	}

	//	�q�b�g������
	void	Item::Hitduringtheoperation( const Vector3& pos, const int& playerNum )
	{
		state = false;
		static	float	effectScale = 0.2f;
		particle->Spark( pos, effectScale );
		sound->PlaySE( SE::ITEM_SE );
		m_Effect->StateEffectSet( playerNum, type );
	}

	//	�X�e�[�W�Ƃ̓����蔻��
	void	Item::StageCollisionCheck( void )
	{		
		//	�n�ʍ����擾
		stageHeight = stage->GetHeight( pos );

		//	�e�����ݒ�
		shadow.pos.y = stageHeight + 0.1f;

		//	�ڒn����
		if ( pos.y < stageHeight )
		{
			pos.y = stageHeight;
			move.y = 0.0f;
		}

		//	����������Ĕz�u
		if ( GetPos().y <= RELOCATION_HEIGHT )
		{
			SetPos( Vector3( Random::GetFloat( -10.0f, 10.0f ), 
				Random::GetFloat( 0.0f, MAX_HEIGHT ),
				Random::GetFloat( -10.0f, 10.0f ) ) );
		}
	}

	//	�v���C���[�Ƃ̓����蔻��
	void	Item::PlayerCollisionCheck( void )
	{
		//	�R�C���̃R���W�����̗L���������擾
		if ( !gameManager->GetCoinCollision() )	return;

		Vector3	p_pos = ZERO_VECTOR3;
		bool			isHit = false;
		
		FOR( 0, PLAYER_MAX )
		{
			if( characterManager->GetCoinUnrivaled( value ) )	continue;
			p_pos = characterManager->GetPos( value );

			isHit = Collision::CapsuleVSSphere( p_pos,
				Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ),
				2.0f,
				Vector3( pos.x, pos.y + ( scale * 0.5f ), pos.z ), 
				COLLISION_SCALE );

			if ( isHit )	Hitduringtheoperation( p_pos, value );
		}
	}

//-------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------

	//	���W�ݒ�
	void	Item::SetPos( const Vector3& pos )
	{
		this->pos = pos;
	}

	//	�ړ��ʐݒ�
	void	Item::SetMove( const Vector3& move )
	{
		this->move = move;
	}

	//	�X�P�[���ݒ�
	void	Item::SetScale( const float& scale )
	{
		this->scale = scale;
	}

	//	�����ݒ�
	void	Item::SetAngle( const float& angle )
	{
		this->angle = angle;
	}

	//	���f���n��
	void	Item::SetMesh( iexMesh* obj )
	{
		this->obj = obj;
	}

	//	��Ԑݒ�
	void	Item::SetState( bool state )
	{
		this->state = state;
	}

	//	�^�C�v�ݒ�
	void	Item::SetType( const int& type )
	{
		this->type = type;
	}

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------

	//	���W�擾
	Vector3	Item::GetPos( void )const
	{
		return	pos;
	}
	
	//	�����ݒ�
	float		Item::GetAngle( void )const
	{
		return	angle;
	}

	//	�^�C�v�擾
	int		Item::GetType( void )const
	{
		int		out = type;
		return	out;
	}

	//	������ԏK��
	bool	Item::GetState( void )const
	{
		bool		out = state;
		return	out;
	}