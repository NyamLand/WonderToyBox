
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

//------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Object::Object( void ) : obj( nullptr ),
		pos(0.0f, 0.0f, 0.0f), angle(0.0f, 0.0f, 0.0f), scale(0.0f,0.0f,0.0f),
		state(true), 
		type(0), durableValue(0)
	{
		
	}

	//	�f�X�g���N�^
	Object::~Object( void )
	{
		SafeDelete( obj );
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void	Object::Update( void )
	{
		obj->SetScale( scale );
		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->Update();
	}

	//	�`��
	void	Object::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader != nullptr && technique != nullptr )	
			obj->Render( shader, technique );
		else
			obj->Render();
	}

//------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------

	//	����
	void	Object::Move( void )
	{

	}

	//	�����蔻��
	void	Object::HitCheck( void )
	{

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
	
	//	�^�C�v�ݒ�
	void	Object::SetType( int objType )
	{
		type = objType;
	}

//------------------------------------------------------------------------------
//	���擾
//------------------------------------------------------------------------------

	//	���W�擾
	Vector3	Object::GetPos( void )const
	{
		return	pos;
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

	//	�I�u�W�F�N�g�^�C�v�擾
	int			Object::GetType( void )const
	{
		return	type;
	}

	//	�ϋv�l�擾
	int			Object::GetDurable( void )const
	{
		return	durableValue;
	}
	