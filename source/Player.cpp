
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"BaseObj.h"

#include	"Player.h"

//****************************************************************************************
//
//	Player�N���X
//
//****************************************************************************************

//-----------------------------------------------------------------------------------------
//	�O���[�o��
//-----------------------------------------------------------------------------------------

	//	�v���C���[�̃X�e�[�^�X
	namespace PlayerStatus
	{

	}

//-----------------------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Player::Player( void )
	{
		//	�p�����[�^������
		speed = 0.2f;
		scale = 0.02f;
	}

	//	�f�X�g���N�^
	Player::~Player( void )
	{

	}

//-----------------------------------------------------------------------------------------
//	�X�V�E�`��
//-----------------------------------------------------------------------------------------

	//	�X�V
	void	Player::Update( void )
	{
		//	���앪��
		switch ( mode )
		{
		case MOVE:
			Move();
			break;
			
		case ATTACK:
			Attack();
			break;
		}

		//	�X�V
		BaseObj::Update();
	}

//-----------------------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------------------

	//	�ړ�
	void	Player::Move( void )
	{
		//	���X�e�B�b�N�̓��̓`�F�b�N
		float	axisX = ( float )input->Get( KEY_AXISX );
		float	axisY = ( float )input->Get( KEY_AXISY );
		float	length = sqrtf( axisX * axisX + axisY * axisY );
		if ( length > MIN_INPUT_STATE )
		{
			SetMotion( Y2009Motion::RUN );
			static float adjustSpeed = 0.2f;
			AngleAdjust( adjustSpeed );
			Move( speed );
		}
		else
		{
			SetMotion( Y2009Motion::STAND );
			move = Vector3( 0.0f, move.y, 0.0f );
		}

		//	�U��
		if ( input->Get( KEY_A ) == 3 )		mode = ATTACK;
	}

	//	�ړ�
	void	Player::Move( float speed )
	{
		move.x	 = sinf( angle ) * speed;
		move.z	 = cosf( angle ) * speed;
	}

	//	�U��
	void	Player::Attack( void )
	{
		SetMotion( Y2009Motion::ATTACK1 );
		int		frame = obj->GetFrame();

		//	�����O�i
		float	moveSpeed = 0.05f;
		move.x = moveSpeed * sinf(angle);
		move.z = moveSpeed * cosf(angle);

		//	�U������
		if ( frame >= 377 && frame <= 385 )		attackParam = 1;
		else attackParam = 0;

		//	���[�V�����I������
		if ( frame >= 410 )		mode = MOVE;
	}

	//	�_���[�W
	void	Player::Damage( void )
	{
		
	}

//-----------------------------------------------------------------------------------------
//	�p�x�␳�֐�
//-----------------------------------------------------------------------------------------

	//	�p�x����
	void	Player::AngleAdjust( float speed )
	{
		if ( !( input->Get( KEY_AXISX ) || input->Get( KEY_AXISY ) ) )	return;

		//	���X�e�B�b�N�̓���
		float axisX = input->Get( KEY_AXISX ) * 0.001f;
		float	axisY = -input->Get( KEY_AXISY ) * 0.001f;

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
	void	Player::AngleAdjust( const Vector3& direction, float speed )
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
