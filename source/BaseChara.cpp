
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"CoinManager.h"
#include	"Effect.h"
#include	"Sound.h"

#include	"BaseChara.h"

//*******************************************************************************
//
//	BaseChara�N���X
//
//*******************************************************************************

//----------------------------------------------------------------------------
//	�O���[�o��
//----------------------------------------------------------------------------

#define	MIN_INPUT_STATE 300	//	�X�e�B�b�N����ŏ��l

namespace
{


	namespace AI_MODE_STATE
	{
		enum
		{
			ATTACK,
			MOVE,
			GUARD,
			JUMP,
			WAIT,
		};
	}

	//	��_���[�W�p�F
	namespace
	{
		//	��_���[�W�p�e�F
		const Vector3	DAMAGE_COLOR[] =
		{
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f),
		};
	}
}

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	BaseChara::BaseChara( void ) : obj( nullptr ), input( nullptr ),		//	pointer
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ),	//	Vector3
		angle(0.0f), scale(0.0f), speed(0.0f),	drag(0.0f), force( 0.0f ),	//	float
		unrivaled(false), isGround(false), boosting(false), isPlayer(false),	//	bool
		mode(0), playerNum(0), power(0), leanFrame(0)		//	int
	{
	
	}

	//	�f�X�g���N�^
	BaseChara::~BaseChara( void )
	{
		Release();
	}

	//	������
	bool	BaseChara::Initialize( int playerNum, iex3DObj* org, Vector3 pos, bool isPlayer )
	{
		//	�v���C���[�Ȃ�R���g���[���o�^
		this->isPlayer = isPlayer;
		if ( this->isPlayer )		this->input = ::input[playerNum];

		//	�p�����[�^������
		Initialize( playerNum, org, pos );

		SetMotion( MOTION_NUM::POSTURE );
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Update();

		if ( obj == nullptr )	return false;
		return	true;
	}

	//	������
	bool	BaseChara::Initialize( int playerNum, iex3DObj* org, Vector3 pos )
	{
		//	���f���ݒ�
		obj = nullptr;
		if ( obj == nullptr )	obj = org;
		
		//	�v���C���[�ԍ��o�^
		this->playerNum = playerNum;

		//	�p�����[�^������
		mode = MODE_STATE::WAIT;
		this->pos = pos;
		angle = 0.0f;
		scale = 0.02f;

		//	�\���̏�����
		{
			//	�U����񏉊���
			{
				attackInfo.type = 0;
				attackInfo.bottom = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.top = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.pos = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.r = 0.0f;
				attackInfo.t = 0.0f;
			}

			//	�m�b�N�o�b�N��񏉊���
			{
				knockBackInfo.type = 0;
				knockBackInfo.vec = Vector3( 0.0f, 0.0f, 0.0f );
		}

			//	�_���[�W���F��񏉊���
			{
				damageColor.catchColor = Vector3( 0.0f, 0.0f, 0.0f );
				damageColor.passColor = DAMAGE_COLOR[playerNum];
			}

			//	�p�����[�^��ԏ�����
			{
				ParameterInfoInitialize( slip );
				ParameterInfoInitialize( boost );
				ParameterInfoInitialize( outrage );
				ParameterInfoInitialize( attackUp );
				ParameterInfoInitialize( speedUp );
				ParameterInfoInitialize( bomb );
				ParameterInfoInitialize( jump );
			}

			//	AI��񏉊���
			{
				aiInfo.mode = AI_MODE_STATE::WAIT;
				aiInfo.param = 0;
			}
		}

		if ( obj == nullptr )	return	false;
		return	true;
	}

	//	���
	void	BaseChara::Release( void )
	{
		SafeDelete( obj );
	}

	//	�p�����[�^��ԏ�����
	void	BaseChara::ParameterInfoInitialize( PARAMETER_INFO& ps )
	{
		ps.state = false;
		ps.timer = 0;
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	BaseChara::Update( void )
	{
		//	���[�h�Ǘ�
		ModeManagement();

		//	�d�͉��Z
		move.y += GRAVITY;

		//	�X�e�[�W�����蔻��
		StageCollisionCheck();

		//	�ړ��l���Z
		AddMove();
		
		//	�R�͌v�Z
		CalcDrag();

		//	���X�V
		obj->Animation();
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Update();
	}

	//	�`��
	void	BaseChara::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader == nullptr || technique == nullptr )
		{
			obj->Render();
		}
		else
		{
			CalcColoParameter();
			obj->Render( shader, technique );
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	���[�V�����Ǘ�
	void	BaseChara::MotionManagement( int motion )
	{
		switch ( motion )
		{
		case MOTION_NUM::STAND:
			obj->SetMotion( MOTION_DATA::STAND );
			break;
			
		case MOTION_NUM::POSTURE:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::JUMP:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::GUARD:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::LANDING:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::RUN:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::ATTACK1:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::ATTACK2:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;

		case MOTION_NUM::ATTACK3:
			obj->SetMotion( MOTION_DATA::POSTURE );
			break;


		}
	}

	//	���[�h�Ǘ�
	void	BaseChara::ModeManagement( void )
	{
		switch (mode)
		{
		case MODE_STATE::WAIT:
			Wait();
			break;

		case MODE_STATE::MOVE:
			Move();
			break;

		case MODE_STATE::POWERARTS:
		case MODE_STATE::HYPERARTS:
		case MODE_STATE::QUICKARTS:
			unrivaled = true;
			Attack( mode );
			break;

		case MODE_STATE::JUMP:
			Jump();
			break;

		case MODE_STATE::GUARD:
			Guard();
			break;

		case MODE_STATE::DAMAGE_STRENGTH:
			KnockBackStrength();
			SetDamageColor( damageColor.catchColor );
			break;

		case MODE_STATE::DAMAGE_MIDDLE:
			KnockBackMiddle();
			SetDamageColor( damageColor.catchColor );
			break;

		case MODE_STATE::DAMAGE_WEAK:
			KnockBackWeak();
			SetDamageColor( damageColor.catchColor );
			break;

		case MODE_STATE::DAMAGE_LEANBACKWARD:
			KnockBackLeanBackWard();
			break;

		case MODE_STATE::DAMAGE:
			Damage();
			break;
		}
	}

	//	�R�͉��Z
	void	BaseChara::CalcDrag( void )
	{
		move.x *= drag;
		move.z *= drag;
	}

	//	�ړ��l���Z
	void	BaseChara::AddMove()
	{
		pos += move;
	}

	//	�_���[�W���J���[�v�Z
	void	BaseChara::CalcColoParameter( void )
	{
		damageColor.param -= Vector3( 0.035f, 0.035f, 0.035f );
		if ( damageColor.param.x <= 0.0f )	damageColor.param.x = 0.0f;
		if ( damageColor.param.y <= 0.0f )	damageColor.param.y = 0.0f;
		if ( damageColor.param.z <= 0.0f )	damageColor.param.z = 0.0f;

		shader3D->SetValue( "colorParam", damageColor.param );
	}

	//	�X�e�[�W�����蔻��
	void	BaseChara::StageCollisionCheck( void )
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
	void	BaseChara::AngleAdjust( float speed )
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
	void	BaseChara::AngleAdjust( const Vector3& direction, float speed )
	{
		//	���݂̌����ƖڕW�̌����̍������߂�
		float	targetAngle( atan2f( direction.x, direction.z ) );
		float	dAngle( targetAngle - GetAngle() );

		//	���̐��K��
		if ( dAngle > 1.0f * D3DX_PI )	dAngle -= 2.0f * D3DX_PI;
		if ( dAngle < -1.0f * D3DX_PI ) 	dAngle += 2.0f * D3DX_PI;

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
		if ( GetAngle() >= 1.0f * D3DX_PI )		angle -= 2.0f * D3DX_PI;
		if ( GetAngle() <= -1.0f * D3DX_PI )	angle += 2.0f * D3DX_PI;
	}

	//	�m�b�N�o�b�N
	void	BaseChara::KnockBack( void )
	{
		unrivaled = true;
		SetDrag( 0.9f );

		SetMotion( MOTION_NUM::POSTURE );
		if ( move.Length() <= 0.01f )
		{
			SetMode( MODE_STATE::MOVE );
			unrivaled = false;
		}
	}

	//	�m�b�N�o�b�N	��
	void	BaseChara::KnockBackStrength( void )
	{
		float	force = 1.5f;

		move = knockBackInfo.vec * force;
		SetMode( MODE_STATE::DAMAGE );
	}

	//	�m�b�N�o�b�N�@��
	void	BaseChara::KnockBackMiddle( void )
	{
		float force = 0.7f;

		move = knockBackInfo.vec * force;
		SetMode( MODE_STATE::DAMAGE );
	}

	//	�m�b�N�o�b�N�@��
	void	BaseChara::KnockBackWeak( void )
	{
		float force = 0.5f;

		move = knockBackInfo.vec * force;
		SetMode( MODE_STATE::DAMAGE );
	}

	//	�m�b�N�o�b�N	������̂�
	void	BaseChara::KnockBackLeanBackWard( void )
	{
		static int branktime = 0;	//���̋����莞�ԁ@��Ń��[�V�����t���[������Ƃ�\����

		unrivaled = true;
		if ( branktime == 0 ) ( SetDamageColor( damageColor.catchColor ) );
		branktime++;
		SetMove( Vector3( 0.0f, move.y, 0.0f ) );
		SetMotion( MOTION_NUM::POSTURE );
		if ( branktime >= leanFrame )
		{
			branktime = 0;
			SetMode( MODE_STATE::MOVE );
			unrivaled = false;
		}
	}

	//	�ҋ@
	void	BaseChara::Wait( void )
	{
		SetMotion( MOTION_NUM::POSTURE );
		SetDrag( 0.8f );
	}

	//	����
	void	BaseChara::Move( void )
	{
		//	�v���C���[�������łȂ����ŏ����𕪂���
		if ( isPlayer )	Control();
		else				ControlAI();
	}

	//	�ړ�
	void	BaseChara::Move( float length )
	{

	}

	//	�U��
	void	BaseChara::Attack( int attackKind )
	{
		SetMotion( MOTION_NUM::ATTACK1 );

		bool	isEnd = false;

		switch ( attackKind )
		{
		case	MODE_STATE::QUICKARTS:
			isEnd = QuickArts();
			if ( !isEnd )	SetAttackParam( attackKind );
			break;

		case MODE_STATE::POWERARTS:
			isEnd = PowerArts();
			if ( !isEnd )	SetAttackParam( attackKind );
			break;

		case MODE_STATE::HYPERARTS:
			isEnd = HyperArts();
			canHyper = isEnd;
			if ( !isEnd )	SetAttackParam( attackKind );
			break;
		}

		//	���[�V�����I������
		if ( isEnd )
		{
			mode = MODE_STATE::MOVE;
			attackInfo.t = 0.0f;
			attackInfo.r = 0.0f;
			attackInfo.type = 0;
			attackInfo.pos = GetPos();
			knockBackInfo.type = 0;
			unrivaled = false;
		}
	}

	//	�W�����v
	void	BaseChara::Jump( void )
	{
		SetMode( MODE_STATE::MOVE );
		if ( !isGround )	return;
		static	float toY = pos.y = 10.0f;

		if ( pos.y <= toY )
		{
			move.y += 0.2f;
		}
		
		//	�ړ�
		Run();

		//	�ݒu���Ă���
		if ( isGround )	SetMode( MODE_STATE::MOVE );
	}

	//	�K�[�h
	void	BaseChara::Guard( void )
	{
		move.x = move.z = 0.0f;
		SetMotion( MOTION_NUM::GUARD );
		unrivaled = true;
		SetMotion( MOTION_NUM::GUARD );
		if ( input->Get( KEY_B7 ) == 2 )
		{
			SetMode( MODE_STATE::MOVE );
			unrivaled = false;
		}
	}

	//	�_���[�W
	void	BaseChara::Damage( void )
	{
		KnockBack();
	}

	//	����
	void	BaseChara::Run( void )
	{
		//	���X�e�B�b�N�̓��̓`�F�b�N
		float	axisX = ( float )input->Get( KEY_AXISX );
		float	axisY = ( float )input->Get( KEY_AXISY );
		float	length = sqrtf( axisX * axisX + axisY * axisY );

		if ( length > MIN_INPUT_STATE )
		{
			SetMotion( MOTION_NUM::RUN );
			static	float adjustSpeed = 0.2f;
			AngleAdjust(adjustSpeed);
			move.x = sinf(angle) * speed;
			move.z = cosf(angle) * speed;
		}
		else
		{
			SetMotion(MOTION_NUM::POSTURE);
			SetDrag(0.8f);
		}
	}

//----------------------------------------------------------------------------
//	������@�ʓ���
//----------------------------------------------------------------------------

	//	�R���g���[������
	void	BaseChara::Control( void )
	{
		Run();

		if ( input->Get( KEY_A ) == 3 )		mode = MODE_STATE::QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )		mode = MODE_STATE::POWERARTS;
		if ( canHyper )
		{
			if ( input->Get( KEY_C ) == 3 )	mode = MODE_STATE::HYPERARTS;
		}
	
		if ( input->Get( KEY_D ) == 3 )		mode = MODE_STATE::JUMP;
		if ( input->Get( KEY_B7 ) == 3 )	mode = MODE_STATE::GUARD;
		if ( input->Get( KEY_B10 ) == 3 )	mode = MODE_STATE::DAMAGE_STRENGTH;
	}

	//	AI����
	void	BaseChara::ControlAI( void )
	{
		//	���͉��Ȃ̂œK���ɕς��Ă�������
		switch ( mode )
		{
		case AI_MODE_STATE::WAIT:
			break;

		case AI_MODE_STATE::MOVE:
			break;

		case AI_MODE_STATE::ATTACK:
			break;

		case AI_MODE_STATE::GUARD:
			break;

		case AI_MODE_STATE::JUMP:
			break;

		default:
			break;
		}
	}

//----------------------------------------------------------------------------
//	���擾
//----------------------------------------------------------------------------

	//	�s��擾
	Matrix	BaseChara::GetMatrix( void )const
	{
		return	obj->TransMatrix;
	}

	//	���W�擾
	Vector3	BaseChara::GetPos( void )const
	{
		return	pos;
	}

	//	�ړ��l�擾
	Vector3	BaseChara::GetMove( void )const
	{
		return	move;
	}

	//	�O���擾
	Vector3	BaseChara::GetFront( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._31, mat._32, mat._33 );
		out.Normalize();
		return	out;
	}

	//	�E���擾
	Vector3	BaseChara::GetRight( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._11, mat._12, mat._13 );
		out.Normalize();
		return	out;
	}

	//	����擾
	Vector3	BaseChara::GetUp( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._21, mat._22, mat._23 );
		out.Normalize();
		return	out;
	}

	//	�_���[�W���F�擾
	Vector3	BaseChara::GetDamageColor( void )const
	{
		return	damageColor.passColor;
	}

	//	�U������ʒu�擾
	Vector3	BaseChara::GetAttackPos( void )const
	{
		return	attackInfo.pos;
	}

	//	�U������ʒu���擾
	Vector3	BaseChara::GetAttackPos_Bottom( void )const
	{
		return	attackInfo.bottom;
	}

	//	�U������ʒu��擾
	Vector3	BaseChara::GetAttackPos_Top( void )const
	{
		return	attackInfo.top;
	}

	//	�U�����蔼�a�擾
	float		BaseChara::GetAttack_R( void )const
	{
		return	attackInfo.r;
	}

	//	�U������p�����[�^�擾
	float		BaseChara::GetAttack_T( void )const
	{
		return	attackInfo.t;
	}

	//	�����擾
	float		BaseChara::GetAngle( void )const
	{
		return	angle;
	}

	//	�X�P�[���擾
	float		BaseChara::GetScale( void )const
	{
		return	scale;
	}

	//	�U���͎擾
	int			BaseChara::GetPower( void )const
	{
		return	power;
	}

	//	���G��Ԏ擾
	bool		BaseChara::GetUnrivaled( void )const
	{
		return	unrivaled;
	}

	//	�n�C�p�[�g�p�ێ擾
	bool		BaseChara::GetCanHyper( void )const
	{
		return	canHyper;
	}

	//	���[�h�擾
	int			BaseChara::GetMode( void )const
	{
		return	mode;
	}

	//	�v���C���[�ԍ��擾
	int			BaseChara::GetPlayerNum( void )const
	{
		return	playerNum;
	}

	//	������t���[���擾
	int			BaseChara::GetLeanFrame( void )const
	{
		return	leanFrame;
	}

	//	�U���^�C�v�擾
	int			BaseChara::GetAttackParam( void )const
	{
		return	attackInfo.type;
	}

	//	�m�b�N�o�b�N�^�C�v�擾
	int			BaseChara::GetKnockBackType( void )const
	{
		return	knockBackInfo.type;
	}

//----------------------------------------------------------------------------
//	���ݒ�
//----------------------------------------------------------------------------

	//	���[�V�����ݒ�
	void	BaseChara::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			MotionManagement( motion );
		}
	}

	//	���W�ݒ�
	void	BaseChara::SetPos( Vector3 pos )
	{
		this->pos = pos;
	}

	//	�ړ��l�ݒ�
	void	BaseChara::SetMove( Vector3 move )
	{
		this->move = move;
	}

	//	�����ݒ�
	void	BaseChara::SetAngle( float angle )
	{
		this->angle = angle;
	}

	//	�X�P�[���ݒ�
	void	BaseChara::SetScale( float scale )
	{
		this->scale = scale;
	}

	//	���[�h�ݒ�
	void	BaseChara::SetMode( int mode )
	{
		if ( GetMode() != mode )
		{
			this->mode = mode;
		}
	}

	//	�R�͐ݒ�
	void	BaseChara::SetDrag( float param )
	{
		drag = param;
	}

	//	�_���[�W���F�ݒ�
	void	BaseChara::SetDamageColor( Vector3 color )
	{
		damageColor.param = color;
	}

	//	�n���F�ݒ�
	void	BaseChara::SetPassColor( Vector3 color )
	{
		damageColor.catchColor = color;
	}

	//	�����莞�Ԑݒ�
	void	BaseChara::SetLeanFrame( int frame )
	{
		leanFrame = frame;
	}

	//	�u�[�X�g��Ԑݒ�
	void	BaseChara::SetBoosting( bool boosting )
	{
		this->boosting = boosting;
	}

	//	�m�b�N�o�b�N�����ݒ�
	void	BaseChara::SetKnockBackVec( Vector3 vec )
	{
		knockBackInfo.vec = vec;
	}
