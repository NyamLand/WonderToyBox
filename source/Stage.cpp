
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Particle.h"
#include	"Collision.h"
#include	"Stage.h"

//**********************************************************************
//
//	Stage�N���X
//
//**********************************************************************

//----------------------------------------------------------------------------
//	�O���[�o��
//----------------------------------------------------------------------------

#define	COLLISION_LENGTH	15.0f

namespace 
{
	namespace STAGE_TYPE
	{
		enum
		{
			DESK,
			FOREST,
			BLOCK,
		};
	}
}

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Stage::Stage( void ) : objectID( 0 ), dirLightVec( 0.0f, 0.0f, 0.0f ), adjustV(0.0f), stageType(0)
	{
		FOR( 0, OBJECT_TYPE::END )
		{
			org[value] = nullptr;
			collisionObj[value] = nullptr;
			forestRiver = nullptr;
		}

		//	�����ڃ��f���ǂݍ���
		org[OBJECT_TYPE::BASE] = new iexMesh( "DATA/Object/Box/back.imo" );
		org[OBJECT_TYPE::BLUE_BLOCK] = new iexMesh( "DATA/Object/Box/blueBox.imo" );
		org[OBJECT_TYPE::GREEN_BLOCK] = new iexMesh( "DATA/Object/Box/greenBox.imo" );
		org[OBJECT_TYPE::RED_BLOCK] = new iexMesh( "DATA/Object/Box/redBox.imo" );
		org[OBJECT_TYPE::YELLOW_BLOCK] = new iexMesh( "DATA/Object/Box/yellowBox.imo" );
		org[OBJECT_TYPE::DESK_BASE] = new iexMesh( "DATA/BG/stage-desk/stage.IMO" );
		org[OBJECT_TYPE::FOREST_BASE] = new iexMesh( "DATA/BG/Forest/model/forest_base.IMO" );
		forestRiver = new iexMesh( "DATA/BG/Forest/model/forest_river.IMO" );

		//	�����蔻��p���f���ǂݍ���
		collisionObj[OBJECT_TYPE::BASE] = new iexMesh( "DATA/Object/Box/back.imo" );
		collisionObj[OBJECT_TYPE::BLUE_BLOCK] = new iexMesh( "DATA/Object/Box/blueBox.imo" );
		collisionObj[OBJECT_TYPE::GREEN_BLOCK] = new iexMesh( "DATA/Object/Box/greenBox.imo" );
		collisionObj[OBJECT_TYPE::RED_BLOCK] = new iexMesh( "DATA/Object/Box/redBox.imo" );
		collisionObj[OBJECT_TYPE::YELLOW_BLOCK] = new iexMesh( "DATA/Object/Box/yellowBox.imo" );
		collisionObj[OBJECT_TYPE::DESK_BASE] = new iexMesh( "DATA/BG/stage-desk/Collision.IMO" );
		collisionObj[OBJECT_TYPE::FOREST_BASE] = new iexMesh( "DATA/BG/Forest/Collision/collision_forest.IMO" );

		//	���X�g������
		objList.clear();
	}

	//	�f�X�g���N�^
	Stage::~Stage( void )
	{
		Release();
	}

	//	������
	bool	Stage::Initialize( void )
	{
		objectID = 0;
		stageType = gameManager->GetStageType();
		switch ( stageType )
		{
		case STAGE_TYPE::DESK:
			Append( Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 1.0f, 1.0f ), MOVE_TYPE::FIX_BOX, OBJECT_TYPE::DESK_BASE );
			Append( Vector3( 10.0f, 20.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::BREAK_OBJECT, OBJECT_TYPE::RED_BLOCK );
			Append( Vector3( -10.0f, 10.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::MOVE_BOX_HIEGHT, OBJECT_TYPE::RED_BLOCK );
			Collision::Initiallize( collisionObj[OBJECT_TYPE::DESK_BASE] );
			iexLight::DirLight( shader3D, 0, &dirLightVec, 1.5f, 1.5f, 1.5f );
			break;

		case STAGE_TYPE::FOREST:
			Append( Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 1.0f, 1.0f ), MOVE_TYPE::FIX_BOX, OBJECT_TYPE::FOREST_BASE );
			Collision::Initiallize( collisionObj[OBJECT_TYPE::FOREST_BASE] );
			iexLight::DirLight( shader3D, 0, &dirLightVec, 0.5f, 0.5f, 0.5f );
			break;

		case STAGE_TYPE::BLOCK:
			Append( Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 1.0f, 1.0f ), MOVE_TYPE::FIX_BOX, OBJECT_TYPE::BASE );
			Append( Vector3( 20.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::FIX_BOX, OBJECT_TYPE::BLUE_BLOCK );
			Append( Vector3( -10.0f, 2.5f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::MOVE_BOX_HIEGHT, OBJECT_TYPE::RED_BLOCK );
			Append( Vector3( -10.0f, 20.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::BREAK_OBJECT, OBJECT_TYPE::RED_BLOCK );
			iexLight::DirLight( shader3D, 0, &dirLightVec, 1.5f, 1.5f, 1.5f );
			break;
		}
		return	true;
	}

	//	���
	void	Stage::Release( void )
	{
		//	�����ډ��
		SafeDelete( forestRiver );

		//	�I���W�i�����
		for ( int i = 0; i < OBJECT_TYPE::END; i++ )
		{
			SafeDelete( org[i] );
			SafeDelete( collisionObj[i] );
		}

		//	���X�g���
		for ( auto it = objList.begin(); it != objList.end(); )
		{
			it = objList.erase( it );
		}
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	Stage::Update( void )
	{
		bool	state = true;

		//	UV�A�j���[�V����
		if ( stageType == STAGE_TYPE::FOREST )
		{
			//�@��̗���iUV�A�j���j
			adjustV -= 0.001f;
		}

		FOR_LIST( objList.begin(), objList.end() )
		{
			( *it )->Update();

			//	�����`�F�b�N
			state = ( *it )->GetState();
			if ( !state )
			{
				it = objList.erase( it );
				break;
			}
		}
	}

	//	�`��
	void	Stage::Render( iexShader* shader, LPSTR technique )
	{
		//	�X�X�e�[�W��
		if ( stageType == STAGE_TYPE::FOREST )
		{
			shader3D->SetValue( "adjustV", adjustV );
			forestRiver->Render( shader, "effect_add" );
		}

		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( shader != nullptr && technique != nullptr )		( *it )->Render( shader, technique );
			else																			( *it )->Render();
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	���X�g�ǉ�
	void	Stage::Append( Vector3 pos, Vector3 angle, Vector3 scale, int moveType, int objType )
	{
		Object*	object = nullptr;
		object = new Object();

		//	�p�����[�^�ݒ�
		object->SetMoveType( moveType );
		object->SetObjectType( objType );
		object->SetMesh( org[objType]->Clone() );
		object->SetCollisionModel( collisionObj[objType]->Clone() );
		object->SetPos( pos );
		object->SetAngle( angle );
		object->SetScale( scale );
		object->SetOriginHeight( pos.y );
		//object->InitTempPos();
		object->Update();
		object->SetId( objectID );
		objectID++;

		//	���X�g�ɒǉ�
		objList.push_back( object );
	}

	//	�X�e�[�W�����蔻��i ���� �j�i �������߂����̂��������蔻�� �j
	bool	Stage::GetReflect( Vector3& outPos, Vector3& outVec, float rate )
	{
		//	�ϐ��ޔ�
		Vector3	pos = outPos;
		Vector3	vec = outVec;

		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;

		//	�o��
		bool			out = false;
		int	outType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			outType = ( *it )->GetObjectType();
			if ( outType != OBJECT_TYPE::BASE && outType != OBJECT_TYPE::FOREST_BASE && outType != OBJECT_TYPE::DESK_BASE )
			{
				//	�����v�Z
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;

				out = Collision::GetReflect( ( *it )->GetMesh(), pos, vec, rate );
			}
			else
			{
				out = Collision::GetRefrectFix( ( *it )->GetMesh(), pos, vec, rate );
			}
		}

		//	���ʂ𔽉f
		outPos = pos;
		outVec = vec;

		return	out;
	}

	//	�X�e�[�W�����蔻��i �� �j
	bool	Stage::CheckWall( const Vector3& p_pos, Vector3& outVec )
	{
		//	�ϐ��ޔ�
		Vector3	pos = p_pos;
		Vector3	vec = outVec;

		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;

		//	�o��
		bool			out = false;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType= ( *it )->GetObjectType();
			
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE ) 
			{
				out = Collision::CheckWall( ( *it )->GetMesh(), pos, vec );
			}
		}

		//	���ʂ𔽉f
		outVec = vec;

		return	out;
	}
	
	//	�X�e�[�W�����蔻��i �n�� �j
	bool	Stage::CheckDown( const Vector3& p_pos, float& outHeight )
	{
		//	�ϐ��ޔ�
		Vector3	pos = p_pos;
		Vector3	vec = Vector3( 0.0f, -1.0f, 0.0f );
		float			height = p_pos.y;

		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		bool			out = false;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->GetObjectType() != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	����������������X�L�b�v
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					if ( length <= minLength )
					{
						minLength = length;
						out = Collision::CheckDown( ( *it )->GetMesh(), pos, height );
					}
				}
			}
			out = Collision::CheckDown( ( *it )->GetMesh(), pos, height );
		}
		outHeight = height;
		return	out;
	}

	//	�X�e�[�W�����擾(�I�u�W�F�N�g)
	float	Stage::GetHeightToObject( const Vector3& pos, Vector3& outTempPos, int& outId, int id  )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		Vector3	resultPos = Vector3( 0.0f, 0.0f, 0.0f );
		float			out = -1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	�����Ύ����̓X���[
			if ( id != 0 && ( *it )->GetID() == id )	continue;
			objType = ( *it )->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE )
			{
				//	�����v�Z
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetHeight( ( *it )->GetMesh(), pos );
						outTempPos = ( *it )->GetTempPos();
						outId = ( *it )->GetID();
					}
				}
			}
		}
		return	out;
	}

	//	�X�e�[�W�O���擾(�I�u�W�F�N�g)
	float	Stage::GetFrontToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = 1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	�����Ύ����̓X���[
			if ( id != 0 && (*it)->GetID() == id )	continue;
			objType = (*it)->GetObjectType();

			//	�x�[�X�ӊO
			if (objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE)
			{
				//	�����v�Z
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetFront( ( *it )->GetMesh(), pos, outHitPos );
						outId = ( *it )->GetID();
					}
				}
			}
		}

		return	out;
	}

	//	�X�e�[�W����擾(�I�u�W�F�N�g)
	float	Stage::GetBackToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = -1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	�����Ύ����̓X���[
			if ( id != 0 && ( *it )->GetID() == id )	continue;
			objType = ( *it )->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE )
			{
				//	�����v�Z
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetBack( ( *it )->GetMesh(), pos, outHitPos );
						outId = ( *it )->GetID();
					}
				}
			}
		}

		return	out;
	}

	//	�X�e�[�W�E���擾(�I�u�W�F�N�g)
	float	Stage::GetRightToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = 1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	�����Ύ����̓X���[
			if ( id != 0 && ( *it )->GetID() == id )	continue;
			objType = ( *it )->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE )
			{
				//	�����v�Z
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetRight( ( *it )->GetMesh(), pos, outHitPos );
						outId = ( *it )->GetID();
					}
				}
			}
		}

		return	out;
	}
	
	//	�X�e�[�W�����擾(�I�u�W�F�N�g)
	float	Stage::GetLeftToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id )
	{
		//	�����p
		Vector3	v = Vector3(0.0f, 0.0f, 0.0f);
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = -1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	�����Ύ����̓X���[
			if ( id != 0 && ( *it )->GetID() == id )	continue;
			objType = ( *it )->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE && objType != OBJECT_TYPE::DESK_BASE && objType != OBJECT_TYPE::FOREST_BASE )
			{
				//	�����v�Z
				v = ( *it )->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetLeft( ( *it )->GetMesh(), pos, outHitPos );
						outId = ( *it )->GetID();
					}
				}
			}
		}

		return	out;
	}

	//	�X�e�[�W�����擾
	float	Stage::GetHeight( const Vector3& pos )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = -1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	�x�[�X
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetHeight( ( *it )->GetMesh(), pos );
			}
		}

		return	out;
	}

	//	�X�e�[�W�O���擾
	float	Stage::GetFront( const Vector3& pos )
	{
		//	�o��
		float			out = 1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetFront( ( *it )->GetMesh(), pos );
			}
		}

		return	out;
	}
	
	//	�X�e�[�W����擾
	float	Stage::GetBack( const Vector3& pos )
	{
		//	�o��
		float			out = -1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetBack( ( *it )->GetMesh(), pos );
			}
		}
		return	out;
	}

	//	�X�e�[�W�E���擾
	float	Stage::GetRight( const Vector3& pos )
	{
		//	�o��
		float			out = 1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetRight( ( *it )->GetMesh(), pos );
			}
		}

		return	out;
	}

	//	�X�e�[�W�����擾
	float	Stage::GetLeft( const Vector3& pos )
	{
		//	�o��
		float			out = -1000.0f;
		int			objType;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			objType = ( *it )->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType == OBJECT_TYPE::BASE || objType == OBJECT_TYPE::DESK_BASE || objType == OBJECT_TYPE::FOREST_BASE )
			{
				out = Collision::GetLeft( ( *it )->GetMesh(), pos );
			}
		}

		return	out;
	}

	//	�ϋv�l����
	void	Stage::SubDurableValue( int id )
	{
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->GetID() == id ) 
			{
				(*it)->SubDurableValue();
				break;
			}
		}
	}

//----------------------------------------------------------------------------
//	���ݒ�
//----------------------------------------------------------------------------

	//	���G��Ԑݒ�
	void	Stage::SetUnrivaled( int id, bool state )
	{
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->GetID() == id )
			{
				( *it )->SetUnrivaled( state );
				break;
			}
		}
	}

	//	���s���ݒ�
	void	Stage::SetDirLightVec( Vector3 dir )
	{
		dirLightVec = dir;
	}

//----------------------------------------------------------------------------
//	���擾
//----------------------------------------------------------------------------

	//	���G��Ԏ擾
	bool	Stage::GetUnrivaled( int id )
	{
		bool	out = false;
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->GetID() == id )
			{
				out = ( *it )->GetUnrivaled();
				break;
			}
		}
		return	out;
	}

	//	�ړ��l�擾
	Vector3	Stage::GetTempPos( int id )
	{
		int	objId = 0;
		Vector3	tempPos = Vector3( 0.0f, 0.0f, 0.0f );
		FOR_LIST( objList.begin(), objList.end() )
		{
			objId = ( *it )->GetID();
			if ( objId == id )
			{
				tempPos = ( *it )->GetTempPos();
				break;
			}
		}

		return	tempPos;
	}

	//	�ړ��l�擾
	Vector3	Stage::GetMove( int id )
	{
		int	objId = 0;
		Vector3	out = Vector3( 0.0f, 0.0f, 0.0f );
		FOR_LIST( objList.begin(), objList.end() )
		{
			objId = (*it)->GetID();
			if ( objId == id )
			{
				out = ( *it )->GetMove();
				break;
			}
		}

		return	out;
	}

	//	���̎擾
	Stage*	Stage::GetInstance( void )
	{
		static	Stage	out;
		return	&out;
	}