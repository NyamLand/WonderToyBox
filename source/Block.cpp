
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Camera.h"
#include	"Block.h"

//*********************************************************************************
//
//	Block�N���X
//
//*********************************************************************************

Block*	m_Block;

//-------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------

	//	�R���X�g���N�^
	Block::Block( void )
	{
	
	}
	
	//	�f�X�g���N�^
	Block::~Block( void )
	{
		SafeDelete( box );
	}

	//	������
	bool	Block::Initialize( int input, int type, Vector3 pos, float scale )
	{
		this->input = ::input[input];

		switch ( type )
		{
		case 0:
			box = new iexMesh( "DATA/Box/redBox.IMO" );
			break;

		case 1:
			box = new iexMesh( "DATA/Box/blueBox.IMO" );
			break;

		case 2:
			box = new iexMesh( "DATA/Box/greenBox.IMO" );
			break;

		case	3:
			break;
		}

		this->pos = pos;
		this->scale = scale;
		this->angle = 0.0f;
		this->speed = 0.5f;
		this->move = Vector3( 0.0f, 0.0f, 0.0f );
		return	true;
	}

//-------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------

	//	�X�V
	void	Block::Update( void )
	{
		Move();
		pos += move;
		box->SetPos( pos );
		box->SetAngle( angle );
		box->SetScale( scale );
		box->Update();
	}

	//	�`��
	void	Block::Render( void )
	{
		box->Render();
	}


	//-----------------------------------------------------------------------------------------
	//	����֐�
	//-----------------------------------------------------------------------------------------

	//	�ړ�
	void	Block::Move( void )
	{
		//	���X�e�B�b�N�̓��̓`�F�b�N
		float	axisX = ( float )input->Get( KEY_AXISX );
		float	axisY = ( float )input->Get( KEY_AXISY );
		float	length = sqrtf( axisX * axisX + axisY * axisY );
		if ( length > 300 )
		{
			static float adjustSpeed = 0.2f;
			AngleAdjust( adjustSpeed );
			Move( speed );
		}
		else
		{
			move = Vector3( 0.0f, move.y, 0.0f );
		}
	}

	//	�ړ�
	void	Block::Move( float speed )
	{
		move.x = sinf( angle ) * speed;
		move.z = cosf( angle ) * speed;
	}

	//-----------------------------------------------------------------------------------------
	//	�p�x�␳�֐�
	//-----------------------------------------------------------------------------------------

	//	�p�x����
	void	Block::AngleAdjust( float speed )
	{
		if (!(input->Get( KEY_AXISX ) || input->Get( KEY_AXISY ) ) )		return;

		//	���X�e�B�b�N�̓���
		float axisX = input->Get( KEY_AXISX ) * 0.001f;
		float	axisY = -input->Get(KEY_AXISY) * 0.001f;

		//	�J�����̑O�����������߂�
		Vector3	vEye( m_Camera->GetTarget() - m_Camera->GetPos() );
		float	cameraAngle = atan2f( vEye.x, vEye.z );

		//	���͕��������߂�
		float inputAngle = atan2f( axisX, axisY );

		//	�ڕW�̊p�x�����߂�
		float	targetAngle = cameraAngle + inputAngle;

		//	�e�ɓ�����
		AngleAdjust( Vector3( sinf( targetAngle ), 0.0f, cosf( targetAngle ) ), speed );
	}

	//	�p�x����
	void	Block::AngleAdjust( const Vector3& direction, float speed )
	{
		//	���݂̌����ƖڕW�̌����̍������߂�
		float	targetAngle( atan2f( direction.x, direction.z ) );
		float	dAngle( targetAngle - GetAngle() );

		//	���̐��K��
		if ( dAngle > 1.0f * PI )	dAngle -= 2.0f * PI;
		if ( dAngle < -1.0f * PI )	dAngle += 2.0f * PI;

		//	����speed���k�߂�
		if ( dAngle > 0.0f ){
			dAngle -= speed;
			if ( dAngle < 0.0f )  dAngle = 0.0f;
		} 
		else{
			dAngle += speed;
			if ( dAngle > 0.0f )	dAngle = 0.0f;
		}

		//	�����ɔ��f
		SetAngle( targetAngle - dAngle );

		//	�v���C���[�̌������Έȏ�ɂȂ�Ȃ��悤�ɒ�������
		if ( GetAngle() >= 1.0f * PI )	angle -= 2.0f * PI;
		if ( GetAngle() <= -1.0f * PI )	angle += 2.0f * PI;
	}

	//	�ݒ�
	void	Block::SetPos( Vector3 pos ){ this->pos = pos; }
	void	Block::SetPos( float x, float y, float z ){ this->pos = Vector3( x, y, z ); }
	void	Block::SetAngle( float angle ){ this->angle = angle; }
	void	Block::SetScale( float scale ){ this->scale = scale; }

	//	�擾
	Vector3		Block::GetPos( void ){ return	pos; }
	Vector3		Block::GetAttackPos( void ){ return attackPos; }
	float			Block::GetAngle( void ){ return angle; }
	int				Block::GetAttackParam( void ){ return attackParam; }






