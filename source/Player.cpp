
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"PlayerManager.h"
#include	"Player.h"

//****************************************************************************************
//
//	Player�N���X
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Player::Player( void ) : obj( NULL ),
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ), power( 0 ), bPower(power), diffence( 0 ), knockBackVec( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f ), speed( 0.0f ),bSpeed(speed), mode( 0 ), unrivaled( false ),
		attackParam( 0 ), attackPos( 0.0f, 0.0f, 0.0f ), attackPos_top( 0.0f, 0.0f, 0.0f ), attackPos_bottom( 0.0f, 0.0f, 0.0f ), attack_r( 0.0f ), attack_t( 0.0f ), knockBackType( 0 ),
		isGround(true), force(0.0f), type(0), p_num(0), CanHyper(true), boosting(false)
	{

	}

	//	�f�X�g���N�^
	Player::~Player( void )
	{
		SafeDelete( obj );
	}

	//	������
	bool	Player::Initialize( int input, iex3DObj* org, Vector3 pos )
	{
		this->obj = org;
		this->input = ::input[input];
		this->p_num = input;
		this->pos = pos;
		this->mode = PlayerData::WAIT;
		this->passDamageColor = PlayerData::DAMAGE_COLOR[input];
		
		bPower = power * 2;
		bSpeed = speed * 2;

		if ( obj == NULL )	return	false;
		return	true;
	}

//-------------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------------

	//	�X�V
	void	Player::Update( void )
	{
		ModeManagement();
		CommonUpdate();
	}
	
	//	�`��
	void	Player::Render( void )
	{
		CommonRender();
	}

	//	���ʍX�V
	void	Player::CommonUpdate( void )
	{
		//	�d�͉��Z
		move.y += GRAVITY;

		//	�X�e�[�W�����蔻��
		StageCollisionCheck();

		//	�ړ��l���Z
		pos += move;

		//�@�ǂ񂯂��ǂ����Ńp�����[�^�X�V
		power = (boosting) ? bPower : power;
		speed = (boosting) ? bSpeed : speed;

		//	���X�V
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Animation();
		obj->Update();
	}

	//	���ʕ`��
	void	Player::CommonRender( void )
	{
		obj->Render();
	}

	//	���ʃV�F�[�_�[�t���`��
	void	Player::CommonRender( iexShader* shader, LPSTR technique )
	{

		//	�_���[�W�������v�Z
		colorParam -= Vector3( 0.035f, 0.035f, 0.035f );
		if ( colorParam.x <= 0.0f ){
			colorParam.x = 0.0f;
		}
		if ( colorParam.y <= 0.0f ){
			colorParam.y = 0.0f;
		}
		if ( colorParam.z <= 0.0f ){
			colorParam.z = 0.0f;
		}

		shader3D->SetValue( "colorParam", colorParam );
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------------

	//	���[�h�Ǘ�
	void	Player::ModeManagement( void )
	{
		switch ( mode )
		{
		case PlayerData::WAIT:
			Wait();
			break;

		case PlayerData::MOVE:
			Move();
			break;

		case PlayerData::ATTACK:
		case PlayerData::POWERARTS:
		case PlayerData::HYPERARTS:
		case PlayerData::QUICKARTS:
			unrivaled = true;
			Attack( mode );
			break;

		case PlayerData::JUMP:
			Jump();
			break;

		case PlayerData::GUARD:
			Guard();
			break;

		case PlayerData::DAMAGE_STRENGTH:
			CommonKnockBackStrength();
			SetDamageColor( receiveDamageColor );
			break;

		case PlayerData::DAMAGE:
			Damage();
			break;
		}
	}

	//	�X�e�[�W�����蔻��`�F�b�N
	void	Player::StageCollisionCheck( void )
	{
		//�@������
		float work = Collision::GetHeight( pos );
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
		if ( GetAngle() >= 1.0f * PI )		angle -= 2.0f * PI;
		if ( GetAngle() <= -1.0f * PI )	angle += 2.0f * PI;
	}

	//	���ʓ���
	void	Player::CommonMove( void )
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

	void	Player::CommonMove( float speed )
	{
		move.x = sinf( angle ) * speed;
		move.z = cosf( angle ) * speed;
	}

	//	�W�����v
	void	Player::CommonJump( void )
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
	void	Player::CommonGuard( void )
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
	void	Player::CommonKnockBackStrength( void )
	{
		AddForce( 0.3f );

		move = knockBackVec * force;
		mode = PlayerData::DAMAGE;
	}

	//	�m�b�N�o�b�N	����
	void	Player::CommonKnockBack( void )
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
	void	Player::AddForce( float force )
	{
		this->force = force;
	}

	//	���[�hWait
	void	Player::Wait( void )
	{
		SetMotion( motionData.POSTURE );
	}

	//	���[�hMove
	void	Player::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )		mode = PlayerData::QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )		mode = PlayerData::POWERARTS;
		CanHyper = m_Player->CanHyper;
		if (CanHyper)
		{
			if (input->Get(KEY_C) == 3)		mode = PlayerData::HYPERARTS;
		}
		if ( input->Get( KEY_D ) == 3 )		mode = PlayerData::JUMP;
		if ( input->Get( KEY_B7 ) == 3 )	mode = PlayerData::GUARD;
		if ( input->Get( KEY_B10 ) == 3 )	mode = PlayerData::DAMAGE_STRENGTH;
	}

	//	���[�hAttack
	void	Player::Attack( int attackKind )
	{
		SetMotion( motionData.ATTACK1 );
		int		frame = obj->GetFrame();

		bool	isEnd = false;

		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			isEnd = QuickArts();
			if ( !isEnd )	SetAttackParam( attackKind );
			break;

		case PlayerData::POWERARTS:
			isEnd = PowerArts();
			if ( !isEnd )	SetAttackParam( attackKind );
			break;

		case PlayerData::HYPERARTS:
			isEnd = HyperArts();
			CanHyper = HyperArts();
			if ( !isEnd )	SetAttackParam( attackKind );
			break;
		}

		//	���[�V�����I������
		if ( isEnd )
		{
			mode = PlayerData::MOVE;
			attack_t = 0.0f;
			attack_r = 0.0f;
			attackParam = 0;
			attackPos = GetPos();
			knockBackType = 0;
			unrivaled = false;
		}
	}

	//	���[�hJump
	void	Player::Jump( void )
	{
		CommonJump();
	}

	//	���[�hGuard
	void	Player::Guard( void )
	{
		move.x = move.z = 0.0f;
		SetMotion( motionData.GUARD );
		CommonGuard();
	}

	//	���[�hDamage
	void	Player::Damage( void )
	{
		CommonKnockBack();
	}

//-------------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------------

	//	���[�V�����ݒ�
	void	Player::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion( motion );
		}
	}

	//	���[�h�ݒ�
	void	Player::SetMode( int mode )
	{
		if ( this->mode != mode )		this->mode = mode;
	}

	//	���W�ݒ�
	void	Player::SetPos( const Vector3& pos )
	{ 
		this->pos = pos;
	}

	//	���W�ݒ�
	void	Player::SetPos( const float& x, const float& y, const float& z )
	{
		this->pos = Vector3( x, y, z );
	}

	//	�����ݒ�
	void	Player::SetAngle( const float& angle )
	{ 
		this->angle = angle;
	}

	//	�X�P�[���ݒ�
	void	Player::SetScale( const float& scale )
	{ 
		this->scale = scale; 
	}

	//	�m�b�N�o�b�N�����ݒ�
	void	Player::SetKnockBackVec( const Vector3& knockBackVec )
	{ 
		this->knockBackVec = knockBackVec;
	}

	//	���[�h�ݒ�
	void	Player::SetMode( const PlayerData::STATE& state )
	{
		if ( GetMode() != state )		mode = state;
	}

	//	�^�C�v�ݒ�
	void	Player::SetType( const int& type )
	{
		this->type = type;
	}

	//	�_���[�W���F�ݒ�
	void	Player::SetDamageColor( const Vector3& color )
	{ 
		this->colorParam = color;
	}

	//	�n���F�ݒ�
	void	Player::SetReceiveColor( const Vector3& color )
	{
		this->receiveDamageColor = color;
	}

	//	�p���[�ݒ�
	void	Player::SetPower( const int& power )
	{
		this->power = power;
	}

	//	�X�s�[�h�ݒ�
	void	Player::SetSpeed( const float& speed )
	{
		this->speed = speed;
	}

	//	�u�[�X�g�ݒ�
	void	Player::SetBoosting( const bool& boosting )
	{ 
		this->boosting = boosting; 
	}

//-------------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------------
	
	//	���W�擾
	Vector3		Player::GetPos( void )
	{
		Vector3	out = pos;
		return	out;
	}

	//	�s��擾
	Matrix		Player::GetMatrix( void )
	{ 
		Matrix	out = obj->TransMatrix;
		return	out;
	}

	//	�����擾
	float		Player::GetAngle( void )
	{ 
		float	out = angle;
		return out;
	}

	//	���G��Ԏ擾
	bool		Player::GetUnrivaled( void )
	{ 
		bool	out = unrivaled;
		return out;
	}

	//	���[�h�擾
	int			Player::GetMode( void )
	{
		int		out = this->mode;
		return out;
	}

	//	�^�C�v�擾
	int			Player::GetType( void )
	{
		int		out = this->type;
		return out; 
	}

	//	�v���C���[�ԍ��擾
	int			Player::GetP_Num( void )
	{
		int		out = this->p_num;
		return out; 
	}

	//	�_���[�W���F�擾
	Vector3		Player::GetDamageColor( void )
	{
		Vector3	out = this->passDamageColor;
		return	out; 
	}

	//	�n�C�p�[�g�p��Ԏ擾
	bool		Player::GetCanHyper( void )
	{
		bool	out = CanHyper;
		return out; 
	}

	//	�p���[�擾
	int			Player::GetPower( void )
	{
		int		out = power;
		return out; 
	}

	//	�X�s�[�h�擾
	float		Player::GetSpeed( void )
	{
		float	out = speed;
		return out; 
	}

	//	�U���p�����[�^�擾
	int			Player::GetAttackParam( void )
	{
		int		out = attackParam;
		return out; 
	}

	//	�m�b�N�o�b�N�^�C�v�擾
	int			Player::GetKnockBackType( void )
	{
		int		out = knockBackType;
		return out; 
	}

	//	�����蔻��ʒu�擾
	Vector3		Player::GetAttackPos( void )
	{
		Vector3	out = attackPos;
		return out;
	}

	//	�����蔻��ʒu��[�擾
	Vector3		Player::GetAttackPos_Top( void )
	{
		Vector3	out = attackPos_top;
		return out; 
	}

	//	�����蔻��ʒu���[�擾
	Vector3		Player::GetAttackPos_Bottom( void )
	{
		Vector3	out = attackPos_bottom;
		return out; 
	}

	//	�U���������擾
	float		Player::GetAttack_T( void )
	{
		float	out = attack_t;
		return out; 
	}

	//	�U�������蔻�蔼�a�擾
	float		Player::GetAttack_R( void )
	{
		float	out = attack_r;
		return out; 
	}

