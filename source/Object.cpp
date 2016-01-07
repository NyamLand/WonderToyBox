
#include	"iextreme.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Sound.h"
#include	"CharacterManager.h"
#include	"Stage.h"
#include	"Object.h"

//*******************************************************************************
//
//	Object�N���X
//
//*******************************************************************************

#define	MOVE_HEIGHT			5.0f
#define	DURABLE_VALUE		3		//	�ϋv�l
#define	UNRIVALEDTIME		30		//	���G����

//------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Object::Object( void ) : obj( nullptr ), collisionObj( nullptr ),
		pos(0.0f, 0.0f, 0.0f), angle(0.0f, 0.0f, 0.0f), scale(0.0f, 0.0f, 0.0f), tempPos(0.0f, 0.0f, 0.0f), move(0.0f, 0.0f, 0.0f),
		moveHeight(0.0f), flashParam(0.0f),
		state(false), unrivaled(false),
		moveType(MOVE_TYPE::FIX_BOX), durableValue(DURABLE_VALUE), objectType(OBJECT_TYPE::BASE), id(1), unrivaledTime(0)
	{
		
	}

	//	�f�X�g���N�^
	Object::~Object( void )
	{
		SafeDelete( obj );
		SafeDelete( collisionObj );
	}

	//	������
	bool	Object::Initialize( void )
	{
		move = Vector3( 0.0f, 0.0f, 0.0f );
		moveHeight = 0.0f;
		flashParam = 0.0f;
		unrivaled = false;
		durableValue = DURABLE_VALUE;
		unrivaledTime = 0;
		return	true;
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void	Object::Update( void )
	{
		//	����
		Move();

		//	�ړ��ʉ��Z
		pos += move;

		//	���X�V
		obj->SetScale( scale );
		if ( objectType == OBJECT_TYPE::DESK_BASE ) 	obj->SetAngle( angle.y + D3DX_PI );
		else																			obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->Update();
		
		//	���X�V
		collisionObj->SetScale( scale );
		collisionObj->SetAngle( angle );
		collisionObj->SetPos( pos );
		collisionObj->Update();
	}

	//	�`��
	void	Object::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader != nullptr && technique != nullptr )
		{
			if ( moveType == MOVE_TYPE::BREAK_OBJECT )
			{
				shader3D->SetValue( "flashParam", 0.5f + 0.5f * sinf( flashParam) );
			}
			obj->Render( shader, technique );
			shader3D->SetValue( "flashParam", 0.0f );
		}
		else
		{
			obj->Render();
		}
	}

//------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------

	//	����
	void	Object::Move( void )
	{
		switch ( moveType )
		{
		case MOVE_TYPE::FIX_BOX:
			//	�Œ�Ȃ̂łȂɂ����Ȃ�
			break;

		case MOVE_TYPE::BREAK_OBJECT:
			//	�ϋv�l�O�Ŕj��
			if ( durableValue <= 0 )
			{
				particle->BlueFlame( pos, 3.0f );
				state = false;
			}

			//	���G���ԉ���
			if ( unrivaled )
			{
				unrivaledTime++;
				
				if ( unrivaledTime >= UNRIVALEDTIME )
				{
					unrivaledTime = 0;
					unrivaled = false;
				}
			}
			
			//	�d�͓K�p
			move.y += GRAVITY;

			//	�ڒn����
			StageCollisionCheck();

			//	�_��
			flashParam += D3DX_PI / 180 * 1.0f;
			if ( flashParam >= 2 * D3DX_PI )	flashParam = 0.0f;

			//	�U�������蔻��
			//	�G�t�F�N�g( �q�b�g���G�t�F�N�g�A�j��G�t�F�N�g )
			break;

		case	MOVE_TYPE::MOVE_BOX_HIEGHT:
			//	�㉺�Ɉړ�
			MoveHeight();
			break;

		case	MOVE_TYPE::MOVE_BOX_SIDE:
			//	���E�Ɉړ�
			MoveSide();
			break;
		}
	}

	//	�㉺�ړ�
	void	Object::MoveHeight( void )
	{
		static	float	param = 0.0f;
		param += D3DX_PI * 0.01f;
		if ( param >= D3DX_PI * 2.0f )	param = 0.0f;

		//	��b�ňꉝ��
		move.y = MOVE_HEIGHT / 60.0f * sinf( param );
	}

	//	���E�ړ�
	void	Object::MoveSide( void )
	{
		static	float	param = 0.0f;
		static	float	speed = 0.6f;
		param += D3DX_PI * 0.01f;
		if ( param >= D3DX_PI * 2.0f )	param = 0.0f;

		//	��b�ňꉝ��
		move.x = MOVE_HEIGHT / 60.0f * sinf( param );
	}

	//	�X�e�[�W�����蔻��
	void	Object::StageCollisionCheck( void )
	{
		float	work = 0.0f;
		float	objectWork = 0.0f;
		Vector3	tempPos;
		int	outId;

		work = stage->GetHeight( pos );
		objectWork = stage->GetHeightToObject( pos, tempPos, outId, id );
		if ( pos.y < work || pos.y < objectWork )
		{
			if ( pos.y < objectWork )
			{
				pos.y = objectWork;
				pos += tempPos;
			}
			if ( pos.y < work )			pos.y = work;
			move.y = 0.0f;
		}
	}

	//	�����蔻��
	void	Object::HitCheck( void )
	{

	}

	//	�ϋv�l����
	void	Object::SubDurableValue( void )
	{
		durableValue--;
	}

//------------------------------------------------------------------------------
//	���ݒ�
//------------------------------------------------------------------------------

	//	���W�ݒ�
	void	Object::SetPos( Vector3 Pos )
	{
		pos = Pos;
	}

	//	�����ݒ�
	void	Object::SetAngle( Vector3 Angle )
	{
		angle = Angle;
	}

	//	�����ݒ�
	void	Object::SetAngle( float Angle )
	{
		angle = Vector3( 0.0f, Angle, 0.0f );
	}

	//	�X�P�[���ݒ�
	void	Object::SetScale( Vector3 Scale )
	{
		scale = Scale;
	}
	
	//	�X�P�[���ݒ�
	void	Object::SetScale( float Scale )
	{
		scale = Vector3( Scale, Scale, Scale );
	}
	
	//	���f���ݒ�
	void	Object::SetMesh( iexMesh* org )
	{
		obj = org;
	}

	//	�����蔻��p���f���ݒ�
	void	Object::SetCollisionModel( iexMesh* org )
	{
		collisionObj = org;
	}
	
	//	����^�C�v�ݒ�
	void	Object::SetMoveType( int MoveType )
	{
		moveType = MoveType;
	}

	//	���f���^�C�v�ݒ�
	void	Object::SetObjectType( int ObjectType )
	{
		objectType = ObjectType;
	}

	//	���̍����ݒ�
	void	Object::SetOriginHeight( float height )
	{
		originHeight = height;
	}

	//	ID�ݒ�
	void	Object::SetId( int ID )
	{
		id = ID;
	}

	//	���G�ݒ�
	void	Object::SetUnrivaled( bool state )
	{
		unrivaled = state;
	}

	//	�����l�ړ��ʏ����ݒ�
	void	Object::InitTempPos( void )
	{
		tempPos = pos;
	}

	//	��Ԑݒ�
	void	Object::SetState( bool state )
	{
		this->state = state;
	}

//------------------------------------------------------------------------------
//	���擾
//------------------------------------------------------------------------------

	//	����^�C�v�擾
	int	Object::GetMoveType( void )const
	{
		return	moveType;
	}

	//	�I�u�W�F�N�g�^�C�v�擾
	int	Object::GetObjectType( void )const
	{
		return	objectType;
	}

	//	���f���擾
	iexMesh*	Object::GetMesh( void )const
	{
		return	collisionObj;
	}
	
	//	���W�擾
	Vector3	Object::GetPos( void )const
	{
		return	pos;
	}

	//	�O�񂩂�̈ړ��ʎ擾
	Vector3	Object::GetTempPos( void )const
	{
		return	tempPos * 2.0f;
	}

	//	�����擾
	float			Object::GetAngle( void )const
	{
		return	angle.y;
	}

	//	�����擾
	void			Object::GetAngle( Vector3& out )const
	{
		out = angle;
	}

	//	�X�P�[���擾
	Vector3	Object::GetScale( void )const
	{
		return	scale;
	}

	//	��Ԏ擾
	bool			Object::GetState( void )const
	{
		return	state;
	}

	//	���G��Ԏ擾
	bool			Object::GetUnrivaled( void )const
	{
		return	unrivaled;
	}

	//	�ϋv�l�擾
	int			Object::GetDurable( void )const
	{
		return	durableValue;
	}

	//	ID�擾
	int			Object::GetID( void )const
	{
		return	id;
	}

	//	�ړ��ʎ擾
	Vector3	Object::GetMove( void )const
	{
		static DWORD last = timeGetTime();
		DWORD elapse = timeGetTime() - last;
		float dt = elapse / 1000.0f;
		last += elapse;

		return	move;
	}
	