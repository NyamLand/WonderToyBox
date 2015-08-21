
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"

#include	"BaseObj.h"

//****************************************************************************************
//
//	BaseObj�N���X
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	BaseObj::BaseObj( void ) : obj( NULL ),
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ), power( 0 ), diffence( 0 ), knockBackVec( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f ), speed( 0.0f ), mode( 0 ), unrivaled( false ),
		attackParam( 0 ), attackPos( 0.0f, 0.0f, 0.0f ), attackPos_top( 0.0f, 0.0f, 0.0f ), attackPos_bottom( 0.0f, 0.0f, 0.0f ), attack_r( 0.0f ), attack_t( 0.0f ), knockBackType( 0 ),
		isGround(true), coinNum(0), force(0.0f), type(0), p_num( 0 )
	{
		
	}

	//	�f�X�g���N�^
	BaseObj::~BaseObj( void )
	{
		SafeDelete( obj );
	}

	//	������
	bool	BaseObj::Initialize( int input, iex3DObj* org, Vector3 pos )
	{
		this->obj = org;
		this->input = ::input[input];
		this->p_num = input;
		this->pos = pos;

		if ( obj == NULL )	return	false;
		return	true;
	}

//-------------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------------

	//	�X�V
	void	BaseObj::Update( void )
	{
		//	�d�͉��Z
		move.y += GRAVITY;

		//	�X�e�[�W�����蔻��
		StageCollisionCheck();

		//	�ړ��l���Z
		pos += move;

		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Animation();
		obj->Update();
	}

	//	�`��
	void	BaseObj::Render( void )
	{
		obj->Render();
	}

	//	�V�F�[�_�[�t���`��
	void	BaseObj::Render( iexShader* shader, LPSTR technique )
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------------

	//	�X�e�[�W�����蔻��`�F�b�N
	void	BaseObj::StageCollisionCheck( void )
	{
		//�@������
		float work = Collision::GetHeight( pos, 50.0f );
		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
			isGround = true;
		}
		else
		{
			isGround = false;
		}

		//	�ǔ���
		Collision::CheckWall( pos, move );
	}
	
	//	�R�C���������Z
	void	BaseObj::AddCoin( void )
	{
		coinNum++;
	}

	//	�R�C���������Z
	void	BaseObj::SubCoin( void )
	{
		if ( coinNum > 0 )	coinNum--;
	}

	//	�p�x����
	void	BaseObj::AngleAdjust( float speed )
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
	void	BaseObj::AngleAdjust( const Vector3& direction, float speed )
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
		if ( GetAngle() >= 1.0f * PI )		angle -= 2.0f * PI;
		if ( GetAngle() <= -1.0f * PI )	angle += 2.0f * PI;
	}

	//	���ʓ���
	void	BaseObj::CommonMove( void )
	{
		//	���X�e�B�b�N�̓��̓`�F�b�N
		float	axisX = ( float )input->Get( KEY_AXISX );
		float	axisY = ( float )input->Get( KEY_AXISY );
		float	length = sqrtf( axisX * axisX + axisY * axisY );
		if ( length > MIN_INPUT_STATE )
		{
			SetMotion( motionData.RUN );
			static float adjustSpeed = 0.2f;
			AngleAdjust( adjustSpeed );
			CommonMove( speed );
		}
		else
		{
			SetMotion( motionData.POSTURE );
			
			//	���X�Ɉړ��ʂƗ͌��炷
			move.x *= 0.8f;
			move.z *= 0.8f;
		}
	}

	void	BaseObj::CommonMove( float speed )
	{
		move.x = sinf( angle ) * speed;
		move.z = cosf( angle ) * speed;
	}

	//	�W�����v
	void	BaseObj::CommonJump( void )
	{
		mode = PlayerData::MOVE;
		if ( !isGround )	return;
		static	float	toY = pos.y + 20.0f;

		if ( pos.y <= toY )
		{
			move.y += 0.2f;
			pos += move;
		}

		//	�ړ�
		CommonMove();

		//	�ڒn���Ă���
		if ( isGround )	mode = PlayerData::MOVE;
	}

	//	�K�[�h
	void	BaseObj::CommonGuard( void )
	{
		unrivaled = true;
		SetMotion( motionData.GUARD );
		if ( input->Get( KEY_B7 ) == 2 )
		{
			mode = PlayerData::MOVE;
			unrivaled = false;
		}
	}

	//	�m�b�N�o�b�N�@��
	void	BaseObj::CommonKnockBackStrength( void )
	{
		AddForce( 0.3f );

		move = knockBackVec * force;
		
		//static	float	toY = pos.y + 5.0f;

		//if ( pos.y <= toY )
		//{
		//	move.y += 0.3f;
		//	pos += move;
		//}

		mode = PlayerData::DAMAGE;
	}

	//	�m�b�N�o�b�N	����
	void	BaseObj::CommonKnockBack( void )
	{
		unrivaled = true;
		move.x *= 0.9f;
		move.z *= 0.9f;

		SetMotion( motionData.POSTURE );
		if ( move.Length() <= 0.01f )
		{
			mode = PlayerData::MOVE;
			unrivaled = false;
		}
	}

	//	�͉��Z
	void	BaseObj::AddForce( float force )
	{
		this->force = force;
	}

//-------------------------------------------------------------------------------------
//	���ݒ�E�擾
//-------------------------------------------------------------------------------------

	//	�ݒ�
	void	BaseObj::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion( motion );
		}
	}
	void	BaseObj::SetMode( int mode )
	{
		if ( this->mode != mode )		this->mode = mode;
	}

	void	BaseObj::SetPos( Vector3 pos ){ this->pos = pos; }
	void	BaseObj::SetPos( float x, float y, float z ){ this->pos = Vector3( x, y, z ); }
	void	BaseObj::SetAngle( float angle ){ this->angle = angle; }
	void	BaseObj::SetScale( float scale ){ this->scale = scale; }
	void	BaseObj::SetKnockBackVec( Vector3 knockBackVec ){ this->knockBackVec = knockBackVec; }
	void	BaseObj::SetMode( PlayerData::STATE state )
	{
		if ( GetMode() != state )
		{
			mode = state;
		}
	}
	void	BaseObj::SetType( int type ){ this->type = type; }

	//	�擾
	Vector3		BaseObj::GetPos( void ){ return	pos; }
	Matrix		BaseObj::GetMatrix( void ){ return obj->TransMatrix; }
	float			BaseObj::GetAngle( void ){ return angle; }
	bool			BaseObj::GetUnrivaled( void ){ return unrivaled; }
	int				BaseObj::GetCoinNum( void ){ return	coinNum; }
	int				BaseObj::GetMode( void ){ return mode; }
	int				BaseObj::GetType( void ){ return type; }
	int				BaseObj::GetP_Num( void ){ return p_num; }

	//	�����蔻��p�p�����[�^�擾
	int				BaseObj::GetAttackParam( void ){ return attackParam; }
	int				BaseObj::GetKnockBackType( void ){ return knockBackType; }
	Vector3		BaseObj::GetAttackPos( void ){ return attackPos; }
	Vector3		BaseObj::GetAttackPos_Top( void ){ return attackPos_top; }
	Vector3		BaseObj::GetAttackPos_Bottom( void ){ return attackPos_bottom; }
	float			BaseObj::GetAttack_T( void ){ return attack_t; }
	float			BaseObj::GetAttack_R( void ){ return attack_r; }
