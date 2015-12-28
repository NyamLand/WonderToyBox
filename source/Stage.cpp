
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
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
	namespace OBJECT_TYPE
	{
		enum
		{
			BASE,
			BLUE_BLOCK,
			GREEN_BLOCK,
			RED_BLOCK,
			YELLOW_BLOCK,
		};
	}
}

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Stage::Stage( void )
	{
		//	���f���ǂݍ���
		org[0] = new iexMesh( "DATA/Object/Box/back.imo" );
		org[1] = new iexMesh( "DATA/Object/Box/blueBox.imo" );
		org[2] = new iexMesh( "DATA/Object/Box/greenBox.imo" );
		org[3] = new iexMesh( "DATA/Object/Box/redBox.imo" );

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
		Append( Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 1.0f, 1.0f, 1.0f ), 0 );

		Append( Vector3( 20.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 1 );
		Append( Vector3( 0.0f, 20.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 20.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		Append( Vector3( 10.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 1 );
		Append( Vector3( 0.0f, 10.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 10.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		Append( Vector3( 5.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 1 );
		Append( Vector3( 0.0f, 5.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 5.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		Append( Vector3( 0.0f, 10.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 10.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		Append( Vector3( 5.0f, 0.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 1 );
		Append( Vector3( 0.0f, 5.0f, 0.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 2 );
		Append( Vector3( 0.0f, 0.0f, 5.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.5f, 0.5f, 0.5f ), 3 );
		return	true;
	}

	//	���
	void	Stage::Release( void )
	{
		//	�I���W�i�����
		for ( int i = 0; i < 4; i++ )
		{
			SafeDelete( org[i] );
		}

		//	���X�g���
		for ( auto it = objList.begin(); it != objList.end(); )
		{
			it = objList.erase(it);
		}
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	Stage::Update( void )
	{
		FOR_LIST( objList.begin(), objList.end() )
		{
			( *it )->obj->Update();
		}
	}

	//	�`��
	void	Stage::Render( void )
	{
		FOR_LIST( objList.begin(), objList.end() )
		{
			( *it )->obj->Render( shader3D, "full" );
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	���X�g�ǉ�
	void	Stage::Append( Vector3 pos, Vector3 angle, Vector3 scale, int type )
	{
		meshInfo*	mesh;
		mesh = new meshInfo();
		mesh->obj = nullptr;

		//	�p�����[�^�ݒ�
		mesh->type = type;
		mesh->obj = org[type]->Clone();
		mesh->obj->SetPos( pos );
		mesh->obj->SetAngle( angle );
		mesh->obj->SetScale( scale );
		mesh->obj->Update();

		//	���X�g�ɒǉ�
		objList.push_back( mesh );
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

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
			}
			
			out = Collision::GetReflect( ( *it )->obj, pos, vec, rate );
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

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			//	�x�[�X�ȊO��������
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	����������������X�L�b�v
				if ( length >= COLLISION_LENGTH )	continue;
			}
			out = Collision::CheckWall( ( *it )->obj, pos, vec );
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
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	����������������X�L�b�v
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					if ( length <= minLength )
					{
						minLength = length;
						out = Collision::CheckDown( ( *it )->obj, pos, height );
					}
				}
			}
			out = Collision::CheckDown( ( *it )->obj, pos, height );
		}
		outHeight = height;
		return	out;
	}

	//	�X�e�[�W�����擾(�I�u�W�F�N�g)
	float	Stage::GetHeightToObject( const Vector3& pos )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = -1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetHeight( ( *it )->obj, pos );
					}
				}
			}
		}

		return	out;
	}

	//	�X�e�[�W�O���擾(�I�u�W�F�N�g)
	float	Stage::GetFrontToObject( const Vector3& pos )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = 1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = (*it)->obj->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetFront( ( *it )->obj, pos );
					}
				}
			}
		}

		return	out;
	}

	//	�X�e�[�W����擾(�I�u�W�F�N�g)
	float	Stage::GetBackToObject( const Vector3& pos )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = -1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = (*it)->obj->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetBack( ( *it )->obj, pos );
					}
				}
			}
		}

		return	out;
	}

	//	�X�e�[�W�E���擾(�I�u�W�F�N�g)
	float	Stage::GetRightToObject( const Vector3& pos )
	{
		//	�����p
		Vector3	v = Vector3( 0.0f, 0.0f, 0.0f );
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = 1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetRight( ( *it )->obj, pos );
					}
				}
			}
		}

		return	out;
	}
	
	//	�X�e�[�W�����擾(�I�u�W�F�N�g)
	float	Stage::GetLeftToObject( const Vector3& pos )
	{
		//	�����p
		Vector3	v = Vector3(0.0f, 0.0f, 0.0f);
		float			length = 0.0f;
		float			minLength = 1000.0f;

		//	�o��
		float			out = -1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type != OBJECT_TYPE::BASE )
			{
				//	�����v�Z
				v = ( *it )->obj->GetPos() - pos;
				length = v.Length();

				//	�������߂������瓖���蔻��
				if ( length >= COLLISION_LENGTH )	continue;
				else
				{
					//	�P�ԋ߂�������ۑ�
					if ( minLength >= length )
					{
						minLength = length;
						out = Collision::GetLeft( ( *it )->obj, pos );
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

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetHeight( ( *it )->obj, pos );
			}
		}

		return	out;
	}

	//	�X�e�[�W�O���擾
	float	Stage::GetFront( const Vector3& pos )
	{
		//	�o��
		float			out = 1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetFront( ( *it )->obj, pos );
			}
		}

		return	out;
	}
	
	//	�X�e�[�W����擾
	float	Stage::GetBack( const Vector3& pos )
	{
		//	�o��
		float			out = -1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetBack( ( *it )->obj, pos );
			}
		}
		return	out;
	}

	//	�X�e�[�W�E���擾
	float	Stage::GetRight( const Vector3& pos )
	{
		//	�o��
		float			out = 1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetRight( ( *it )->obj, pos );
			}
		}

		return	out;
	}

	//	�X�e�[�W�����擾
	float	Stage::GetLeft( const Vector3& pos )
	{
		//	�o��
		float			out = -1000.0f;

		//	�S�����蔻��
		FOR_LIST( objList.begin(), objList.end() )
		{
			if ( ( *it )->type == OBJECT_TYPE::BASE )
			{
				out = Collision::GetLeft( ( *it )->obj, pos );
			}
		}

		return	out;
	}

//----------------------------------------------------------------------------
//	���ݒ�
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	���擾
//----------------------------------------------------------------------------

	//	���̎擾
	Stage*	Stage::GetInstance( void )
	{
		static	Stage	out;
		return	&out;
	}