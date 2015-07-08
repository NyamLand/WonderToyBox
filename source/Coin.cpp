
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"

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
		obj = new iexMesh( "DATA/coin.imo" );
		angle = 0.0f;
		pos = Vector3( 0.0f, 0.0f, 0.0f );
		move = Vector3( 0.0f, 0.0f, 0.0f );
		scale = 0.5f;
		activate = true;

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

		pos += move;
		StageCollisionCheck();

		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}

	//	�`��
	void	Coin::Render( void )
	{
		obj->Render();
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
	