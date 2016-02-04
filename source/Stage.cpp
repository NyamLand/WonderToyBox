
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
			TOY,
			BLOCK,
		};
	}
}

//	����
Stage*	stage = nullptr;

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Stage::Stage( void ) : objectID( 0 ), dirLightVec( 0.0f, 0.0f, 0.0f ), adjustV( 0.0f ), stageType( 0 )
	{
		//	�|�C���^������
		FOR( 0, OBJECT_TYPE::END )
		{
			org[value] = nullptr;
			collisionObj[value] = nullptr;
			forestRiver = nullptr;
		}

		//	�S�I�u�W�F�N�g������
		FOR( 0, OBJ_MAX )
		{
			object[value] = nullptr;
			object[value] = new Object();
		}

		//	�����ڃ��f���ǂݍ���
		org[OBJECT_TYPE::BASE] = new iexMesh("DATA/Object/Box/back.imo");
		org[OBJECT_TYPE::BLUE_BLOCK] = new iexMesh("DATA/Object/Box/blueBox.imo");
		org[OBJECT_TYPE::GREEN_BLOCK] = new iexMesh("DATA/Object/Box/greenBox.imo");
		org[OBJECT_TYPE::RED_BLOCK] = new iexMesh("DATA/Object/Box/redBox.imo");
		org[OBJECT_TYPE::YELLOW_BLOCK] = new iexMesh("DATA/Object/Box/yellowBox.imo");

		org[OBJECT_TYPE::TREE_TOY] = new iexMesh("DATA/BG/stageobj/tree/tree.IMO");
		org[OBJECT_TYPE::REX_TOY] = new iexMesh("DATA/BG/stageobj/rex/Rex.IMO");
		org[OBJECT_TYPE::RABBIT_TOY] = new iexMesh("DATA/BG/stageobj/usg/usg.IMO");
		org[OBJECT_TYPE::ROBOT_TOY] = new iexMesh("DATA/BG/stageobj/Robot/Robot.IMO");

		org[OBJECT_TYPE::DESK_BASE] = new iexMesh("DATA/BG/stage-desk/stage.IMO");
		org[OBJECT_TYPE::FOREST_BASE] = new iexMesh("DATA/BG/Forest/model/forest_base.IMO");
		org[OBJECT_TYPE::TOY_BASE] = new iexMesh("DATA/BG/stage_toy/stageToy.IMO" );
		forestRiver = new iexMesh("DATA/BG/Forest/model/forest_river.IMO");

		//	�����蔻��p���f���ǂݍ���
		collisionObj[OBJECT_TYPE::BASE] = new iexMesh("DATA/Object/Box/back.imo");
		collisionObj[OBJECT_TYPE::BLUE_BLOCK] = new iexMesh("DATA/Object/Box/blueBox.imo");
		collisionObj[OBJECT_TYPE::GREEN_BLOCK] = new iexMesh("DATA/Object/Box/greenBox.imo");
		collisionObj[OBJECT_TYPE::RED_BLOCK] = new iexMesh("DATA/Object/Box/redBox.imo");
		collisionObj[OBJECT_TYPE::YELLOW_BLOCK] = new iexMesh("DATA/Object/Box/yellowBox.imo");
		collisionObj[OBJECT_TYPE::TREE_TOY] = new iexMesh("DATA/BG/stageobj/tree/treeatr.IMO");
		collisionObj[OBJECT_TYPE::REX_TOY] = new iexMesh("DATA/BG/stageobj/rex/Rexatr.IMO");
		collisionObj[OBJECT_TYPE::RABBIT_TOY] = new iexMesh("DATA/BG/stageobj/usg/usg.IMO");
		collisionObj[OBJECT_TYPE::ROBOT_TOY] = new iexMesh("DATA/BG/stageobj/Robot/Robot.IMO");
		collisionObj[OBJECT_TYPE::DESK_BASE] = new iexMesh("DATA/BG/stage-desk/Collision.IMO");
		collisionObj[OBJECT_TYPE::FOREST_BASE] = new iexMesh("DATA/BG/Forest/Collision/collision_forest.IMO");
		collisionObj[OBJECT_TYPE::TOY_BASE] = new iexMesh( "DATA/BG/stage_toy/stageToy.IMO" );

		//	�ϐ�������
		objectID = 0;
		stageType = gameManager->GetStageType();

		//	�e�X�e�[�W�I�u�W�F�N�g�̐���
		switch ( stageType )
		{
		case STAGE_TYPE::DESK:
			Append(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), MOVE_TYPE::FIX_OBJECT , OBJECT_TYPE::DESK_BASE);
			//Append( Vector3( 10.0f, 20.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::BREAK_OBJECT, OBJECT_TYPE::RED_BLOCK );
			//Append( Vector3( -10.0f, 10.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), MOVE_TYPE::MOVE_BOX_HIEGHT, OBJECT_TYPE::RED_BLOCK );
			break;

		case STAGE_TYPE::TOY:
			Append(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::TOY_BASE);
			//Append(Vector3(-22.0f, 0.0f, 20.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.2f, 0.5f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::YELLOW_BLOCK);
			//Append(Vector3(-14.0f, 3.0f, 20.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.2f, 0.5f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::YELLOW_BLOCK);



			Append(Vector3(-4.0f, 10.0f, 19.5f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.2f, 0.5f), MOVE_TYPE::MOVE_SIDE_OBJECT, OBJECT_TYPE::BLUE_BLOCK);
			//Append(Vector3(-22.0f, 0.0f, 18.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(2.0f, 0.5f, 2.0f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::YELLOW_BLOCK);
			//Append(Vector3(-16.0f, 0.0f, 18.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 1.0f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::GREEN_BLOCK);
			//Append(Vector3(-22.0f, 5.0f, 20.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::BLUE_BLOCK);
			//Append(Vector3(15.0f, 13.0f, 18.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::YELLOW_BLOCK);
			//Append(Vector3(-20.0f, 0.0f, 8.0f), Vector3(0.0f, 1.5f, 0.0f), Vector3(0.09f, 0.09f, 0.09f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::REX_TOY);
			/*Append(Vector3(-10.0f, 10.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.5f, 0.5f), MOVE_TYPE::MOVE_SIDE_OBJECT, OBJECT_TYPE::RED_BLOCK);
			Append(Vector3(-5.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.01f, 0.01f, 0.01f), MOVE_TYPE::BREAK_OBJECT, OBJECT_TYPE::TREE_TOY);
			Append(Vector3(-3.0f, 0.0f, 0.0f), Vector3(0.0f, 3.0f, 0.0f), Vector3(0.01f, 0.01f, 0.01f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::RABBIT_TOY);
			Append(Vector3(5.0f, 0.0f, 5.0f), Vector3(0.0f, 3.0f, 0.0f), Vector3(0.01f, 0.01f, 0.01f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::ROBOT_TOY);
			Append(Vector3(-10.0f, 0.0f, -10.0f), Vector3(0.0f, 1.5f, 0.0f), Vector3(0.09f, 0.09f, 0.09f), MOVE_TYPE::FIX_OBJECT, OBJECT_TYPE::REX_TOY);*/
			break;
		}
	}

	//	�f�X�g���N�^
	Stage::~Stage( void )
	{
		Release();
	}

	//	���s��������
	bool	Stage::LightInitialize( Vector3	dir )
	{
		//	���s���ݒ�
		dirLightVec = dir;

		//	���C�g�F
		Vector3	lightColor;
		
		switch ( stageType )
		{
		case	STAGE_TYPE::DESK:
			lightColor = Vector3( 1.5f, 1.5f, 1.5f );
			break;
			
		case	STAGE_TYPE::TOY:
			lightColor = Vector3(1.5f, 1.5f, 1.5f);
			break;

		case	STAGE_TYPE::BLOCK:
			lightColor = Vector3( 1.5f, 1.5f, 1.5f );
			break;
		}
		//	�V�F�[�_�[�ɃZ�b�g
		iexLight::DirLight( shader3D, 0, &dirLightVec, lightColor.x, lightColor.y, lightColor.z );
		shader3D->SetValue( "DirLightVec", dirLightVec );

		return	true;
	}

	//	���
	void	Stage::Release( void )
	{
		//	�����ډ��
		SafeDelete( forestRiver );

		//	�I���W�i�����
		FOR( 0, OBJECT_TYPE::END )
		{
			SafeDelete( org[value] );
			SafeDelete( collisionObj[value] );
		}
		
		//	�S�I�u�W�F�N�g���
		FOR( 0, OBJ_MAX )
		{
			SafeDelete( object[value] );
		}
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	Stage::Update( void )
	{
		bool	state = true;


		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N( ���݂��Ă�����X�V )
			state = object[value]->GetState();
			if ( state )		object[value]->Update();
		}
	}

	//	���̂ݍX�V
	void	Stage::ObjUpdate(void)
	{
		bool	state = true;

		FOR(0, OBJ_MAX)
		{
			//	�����`�F�b�N( ���݂��Ă�����X�V )
			state = object[value]->GetState();
			if (state)		object[value]->ObjUpdate();
		}
	}
	//	�`��
	void	Stage::Render( iexShader* shader, LPSTR technique )
	{

		FOR( 0, OBJ_MAX )
		{
			if ( !object[value]->GetState() )	continue;
			if ( shader != nullptr && technique != nullptr )		object[value]->Render( shader, technique );
			else																			object[value]->Render();
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	���X�g�ǉ�
	void	Stage::Append( Vector3 pos, Vector3 angle, Vector3 scale, int moveType, int objType )
	{
		FOR( 0, OBJ_MAX )
		{
			//	�󂫃`�F�b�N
			if ( object[value]->GetState() )	continue;

			//	������
			object[value]->Initialize();
			
			//	�p�����[�^�ݒ�
			object[value]->SetState( true );
			object[value]->SetMoveType( moveType );
			object[value]->SetObjectType( objType );
			object[value]->SetMesh( org[objType]->Clone() );
			object[value]->SetCollisionModel( collisionObj[objType]->Clone() );
			object[value]->SetPos( pos );
			object[value]->SetAngle( angle );
			object[value]->SetScale( scale );
			object[value]->SetOriginHeight( pos.y );
			object[value]->SetId( objectID );
			objectID++;
			break;
		}
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;
			
			//	�I�u�W�F�N�g�̃^�C�v�擾
			outType = object[value]->GetObjectType();

			//	�x�[�X�ȊO�Ƃ̓����蔻��
			if (outType != OBJECT_TYPE::BASE 
				&& outType != OBJECT_TYPE::TOY_BASE 
				&& outType != OBJECT_TYPE::FOREST_BASE 
				&& outType != OBJECT_TYPE::DESK_BASE)
			{
				//	�����v�Z
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;

				out = Collision::GetReflect( object[value]->GetMesh(), pos, vec, rate );
			}
			else
			{
				out = Collision::GetRefrectFix( object[value]->GetMesh(), pos, vec, rate );
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�I�u�W�F�N�g�̃^�C�v���擾
			objType= object[value]->GetObjectType();
			
			//	�x�[�X�Ƃ̓����蔻��
			if (objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE 
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::CheckWall( object[value]->GetMesh(), pos, vec );
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�x�[�X�ȊO
			if ( object[value]->GetObjectType() != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	����������������X�L�b�v
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					if ( length <= minLength )
					{
						minLength = length;
						out = Collision::CheckDown( object[value]->GetMesh(), pos, height );
					}
				}
			}
			out = Collision::CheckDown( object[value]->GetMesh(), pos, height );
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;
			
			//	�����Ύ����̓X���[
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	�����v�Z
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetHeight( object[value]->GetMesh(), pos );
						outTempPos = object[value]->GetTempPos();
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�����Ύ����̓X���[
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	�����v�Z
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetFront( object[value]->GetMesh(), pos, outHitPos );
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�����Ύ����̓X���[
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	�����v�Z
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetBack( object[value]->GetMesh(), pos, outHitPos );
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�����Ύ����̓X���[
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	�����v�Z
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetRight( object[value]->GetMesh(), pos, outHitPos );
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�����Ύ����̓X���[
			if ( id != 0 && object[value]->GetID() == id )	continue;
			objType = object[value]->GetObjectType();

			//	�x�[�X�ӊO
			if ( objType != OBJECT_TYPE::BASE 
				&& objType != OBJECT_TYPE::DESK_BASE 
				&& objType != OBJECT_TYPE::FOREST_BASE
				&& objType != OBJECT_TYPE::TOY_BASE)
			{
				//	�����v�Z
				v = object[value]->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetLeft( object[value]->GetMesh(), pos, outHitPos );
						outId = object[value]->GetID();
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�I�u�W�F�N�g�̃^�C�v���擾
			objType = object[value]->GetObjectType();

			//	�x�[�X�̂ݓ����蔻��
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetHeight( object[value]->GetMesh(), pos );
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�I�u�W�F�N�g�̃^�C�v���擾
			objType = object[value]->GetObjectType();

			//	�x�[�X�̂ݓ����蔻��
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetFront( object[value]->GetMesh(), pos );
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�I�u�W�F�N�g�̃^�C�v���擾
			objType = object[value]->GetObjectType();

			//	�x�[�X�̂ݓ����蔻��
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetBack( object[value]->GetMesh(), pos );
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�I�u�W�F�N�g�̃^�C�v���擾
			objType = object[value]->GetObjectType();

			//	�x�[�X�̂ݓ����蔻��
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetRight( object[value]->GetMesh(), pos );
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
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;

			//	�I�u�W�F�N�g�̃^�C�v���擾
			objType = object[value]->GetObjectType();

			//	�x�[�X�̂ݓ����蔻��
			if ( objType == OBJECT_TYPE::BASE 
				|| objType == OBJECT_TYPE::DESK_BASE 
				|| objType == OBJECT_TYPE::FOREST_BASE
				|| objType == OBJECT_TYPE::TOY_BASE)
			{
				out = Collision::GetLeft( object[value]->GetMesh(), pos );
			}
		}

		return	out;
	}

	//	�ϋv�l����
	void	Stage::SubDurableValue( int id )
	{
		FOR( 0, OBJ_MAX )
		{
			//	�����`�F�b�N
			if ( !object[value]->GetState() )	continue;
			
			//	����ID�̃I�u�W�F�N�g�̑ϋv�l����
			if ( object[value]->GetID() == id ) 
			{
				object[value]->SubDurableValue();
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
		FOR( 0, OBJ_MAX )
		{
			if ( !object[value]->GetState() )	continue;
			if ( object[value]->GetID() == id )
			{
				object[value]->SetUnrivaled( state );
				break;
			}
		}
	}

//----------------------------------------------------------------------------
//	���擾
//----------------------------------------------------------------------------

	//	���G��Ԏ擾
	bool	Stage::GetUnrivaled( int id )
	{
		bool	out = false;
		FOR( 0, OBJ_MAX )
		{
			if ( object[value]->GetID() == id )
			{
				out = object[value]->GetUnrivaled();
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
		FOR( 0, OBJ_MAX )
		{
			objId = object[value]->GetID();
			if ( objId == id )
			{
				tempPos = object[value]->GetTempPos();
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
		FOR( 0, OBJ_MAX )
		{
			objId = object[value]->GetID();
			if ( objId == id )
			{
				out = object[value]->GetMove();
				break;
			}
		}

		return	out;
	}

	bool Stage::GetLocal( Vector3 &localPos, Vector3 &localAngle)
	{
		//	�o��
		bool			out =false;
		int			moveType;
		Vector3		outLocalPos = localPos;
		Vector3		outLocalAngle = localAngle;

		//	�S�����蔻��
		FOR(0, OBJ_MAX)
		{
			//	�����`�F�b�N
			if (!object[value]->GetState())	continue;

			//	�I�u�W�F�N�g�̃^�C�v���擾
			moveType = object[value]->GetMoveType();

			//	�x�[�X�̂ݓ����蔻��
			if (moveType == MOVE_TYPE::MOVE_SIDE_OBJECT
				|| moveType == MOVE_TYPE::MOVE_HIEGHT_OBJECT)
			{
				out = object[value]->GetLocal(outLocalPos, outLocalAngle);
				localPos = outLocalPos;
				localAngle = outLocalAngle;
			}
		}

		return	out;
	}

	void Stage::GetWorld( Vector3 &worldPos, Vector3 &worldAngle)
	{
		//	�o��
		int			moveType;
		Vector3		outLocalPos =worldPos;
		Vector3		outLocalAngle = worldAngle;

		//	�S�����蔻��
		FOR(0, OBJ_MAX)
		{
			//	�����`�F�b�N
			if (!object[value]->GetState())	continue;

			//	�I�u�W�F�N�g�̃^�C�v���擾
			moveType = object[value]->GetMoveType();

			//	�x�[�X�̂ݓ����蔻��
			if (moveType == MOVE_TYPE::MOVE_SIDE_OBJECT
				|| moveType == MOVE_TYPE::MOVE_HIEGHT_OBJECT)
			{
				object[value]->GetWorld(outLocalPos, outLocalAngle);
				worldPos = outLocalPos;
				worldAngle = outLocalAngle;
			}
		}
	}