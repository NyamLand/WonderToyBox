
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	<assert.h>
#include	"Camera.h"

//****************************************************************************************
//
//	Camera�N���X
//
//****************************************************************************************

//------------------------------------------------------------------------------------------
//	�O���[�o���ϐ�
//------------------------------------------------------------------------------------------

	//	���̂̐錾
	Camera*	mainView;

//------------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	Camera::Camera( void )
	{
		q = new Rubber();

		pos = Vector3( 0.0f, 20.0f, -80.0f );
		q->position = pos;
		q->mass = 1.0f;
		target = Vector3( 0.0f, 0.6f, 0.8f ); 
		orientation = D3DXQUATERNION( 0, 0, 0, 1 );
		nextPoint = TITLE_MOVE_INFO::pos[TITLE_TARGET::PLAYERNUMBER];
		startPos = pos;
		t = 0.0f;
		moveState = false;
		target = nextTarget = TITLE_MOVE_INFO::target[TITLE_TARGET::PLAYERNUMBER];
		speed = 0.005f;
		Set( pos, target );
	}

	//	�f�X�g���N�^
	Camera::~Camera( void )
	{

	}

//------------------------------------------------------------------------------------------
//	�X�V
//------------------------------------------------------------------------------------------

	//	�X�V
	void	Camera::Update( int viewmode, Vector3 target )
	{
		switch ( viewmode )
		{
		case VIEW_MODE::SETUP:
			ModeSlerp( nextTarget );
			break;

		case  VIEW_MODE::FIX:
			ModeFix( target );
			break;

		case	VIEW_MODE::SLERP:
			ModeSlerp( target );
			break;

		case	VIEW_MODE::CHASE:
			ModeChase();
			break;

		case	VIEW_MODE::RESULT:
			ModeResult();
			break;

		case VIEW_MODE::TITLE:
			ModeTitle();
			break;

		case VIEW_MODE::INDIVIDUAL:
			ModeIndividualSurveillance( target );
			break;
		}

		shader3D->SetValue("ViewPos", mainView->GetPos());
		shader3D->SetValue("matView", mainView->GetMatrix());
		q->Update();
	}

	//	�`��
	void	Camera::Render( void )
	{
		char	str[256];
		sprintf_s( str, "t.x = %f\nt.y = %f\nt.z = %f\n", target.x, target.y, target.z);
		DrawString(str, 50, 500);
	}

//------------------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------------------

	//	���ʐ��`���
	void	Camera::Slerp( Vector3 target, float speed )
	{
		D3DXMATRIX	mat;
		D3DXMatrixRotationQuaternion( &mat, &orientation );
		D3DXVECTOR3	right( mat._11, mat._12, mat._13 );
		D3DXVECTOR3	up( mat._21, mat._22, mat._23 );
		D3DXVECTOR3	forward( mat._31, mat._32, mat._33 );

		D3DXVECTOR3	d;
		D3DXVECTOR3	c_target, c_pos;
		c_target = D3DXVECTOR3( target.x, target.y, target.z );
		c_pos = D3DXVECTOR3( pos.x, pos.y, pos.z );
		d = c_target - c_pos;
		D3DXVec3Normalize( &d, &d );

		D3DXVECTOR3		axis;
		FLOAT	angle;
		angle = acosf( D3DXVec3Dot( &forward, &d ) );
		D3DXVec3Cross( &axis, &forward, &d );
		D3DXVec3Normalize( &axis, &axis );

		if ( fabs( angle ) > 1e-8f )
		{
			D3DXQUATERNION q;
			D3DXQuaternionRotationAxis( &q, &axis, angle );
			D3DXQuaternionSlerp( &orientation, &orientation, &( orientation * q ), speed );
		}

		D3DXMATRIX		R, T;
		D3DXMatrixRotationQuaternion( &R, &orientation );
		D3DXMatrixTranslation( &T, pos.x, pos.y, pos.z );
		matView = R * T;
		Vector3 front( matView._31, matView._32, matView._33 );
		Vector3	position( matView._41, matView._42, matView._43 );

		pos = position;
		this->target = position + front;
	}

	//	�Œ�J����
	void	Camera::ModeFix( Vector3 target )
	{
		Set( pos, target );
	}

	//	�ǂ������J����
	void	Camera::ModeChase( void )
	{
		//	���S���W�E���S����̍ő勗���E�J�����ʒu�v�Z
		this->target = CalcCenterPos();
		this->length = CalcMaxDist();
		CalcCameraPos();

	
		//	�e�͐�
		SpringMove(pos);

		////	��]���
		//Slerp( this->target, 0.1f );

		//	���ݒ�
		Set( q->position, this->target );

	}

	//	���ʐ��`��Ԏg�p
	void	Camera::ModeSlerp( Vector3 target )
	{
		Slerp( target, 0.1f );

		Set( pos, this->target );
	}

	//	���U���g�p�J����
	void	Camera::ModeResult( void )
	{
		Set(Vector3(0.0f, 5.0f, 13.0f), Vector3(0.0f, 5.0f, -30.0f));
	}

	//	�^�C�g���p�J����
	void	Camera::ModeTitle( void )
	{

		//	�ړ��ۊ�
		CubicFunctionInterpolation( pos, startPos, nextPoint, t );

		//	�p�����[�^���Z
		t += speed;
		if ( t >= 1.0f )
		{
			t = 1.0f;
			moveState = true;
		}
		else
		{
			moveState = false;
		}
		
		//	��]
		Slerp( nextTarget, 0.1f );

		//	���X�V
		Set( pos, this->target );
	}

	//	�X�Ď��J����
	void	Camera::ModeIndividualSurveillance( Vector3 target )
	{
		Slerp( target, 0.1f );

		pos = target - Vector3( 0.0f, 10.0f, -10.0f );

		Set( pos, this->target );
	}
	
	//	�U��
	void	Camera::Shake( void )
	{
		shakeTimer--;
		if ( shakeTimer > 0 )
		{
			adjust.x = wide * shakeTimer * ( rand() % 3 - 1 );
			adjust.y = wide * shakeTimer * ( rand() % 3 - 1 );
			adjust.z = wide * shakeTimer * ( rand() % 3 - 1 );
		}
		else
		{
			adjust = Vector3( 0.0f, 0.0f, 0.0f );
			shakeTimer = 0;
			shakeflag = false;
		}
	}

	//	�U���ݒ�
	void	Camera::ShakeSet( float wide, int timer )
	{
		if ( shakeflag ) 	return;
		srand( 0 );
		shakeflag = true;
		this->wide = wide / ( float )shakeTimer;
		this->shakeTimer = timer;
	}

	//	�e���͂��g�����J�����ړ�
	void	Camera::SpringMove( Vector3 position )
	{
		//	����̂�q->position�Ɍ��݂�pos��^����
		if ( q->init_flag )
		{
			q->position = position;
			q->init_flag = false;
		}

		Vector3 n = position - q->position;
		float len = n.Length();
		float F = -2 * (5 - len);
		n.Normalize();
		Vector3 drag = -q->velocity * 1.0f;

		q->AddForce((n*F) + drag);

		//	���X�V
		q->Update();

	}

//------------------------------------------------------------------------------------------
//	���l�v�Z
//------------------------------------------------------------------------------------------

	//	�^�[�Q�b�g���S���W�v�Z
	Vector3	Camera::CalcCenterPos( void )
	{
		Vector3	out = Vector3( 0.0f, 0.0f, 0.0f );

		for ( int i = 0; i < 4; i++ )	out += playerPos[i];

		out /= 4;

		return	out;
	}

	//	�ő勗���Z�o
	float	Camera::CalcMaxDist( void )
	{
		float	len[4];
		Vector3 pos[4];
		float	temp;
		for ( int i = 0; i < 4; i++ )
		{
			pos[i] = playerPos[i];
		}

		//	���ꂼ���pos����^�[�Q�b�g�܂ł̋������Ƃ�
		for ( int i = 0; i < 4; i++ )
		{
			pos[i] = this->target - pos[i];
			len[i] = pos[i].Length();
		}

		//	�\�[�g
		for (int i = 0; i < 4; ++i)
		{
			//	��납�珇�ԂɃ`�F�b�N���Ă���
			for (int s = 3; s > i; --s)
			{
				//	����̗v�f�Ɣ�r
				if (len[s] >	len[s - 1])
				{
					//	�ꎞ�I�ɑޔ�
					temp = len[s - 1];

					//	����
					len[s - 1] = len[s];

					//	�ޔ����Ă����߂�
					len[s] = temp;
				}
			}
		}

		float	out = 0.0f;
		//	�J��������^�[�Q�b�g�܂ł̒�������
		out = len[0];
		if ( out < ( float )MIN ) out = ( float )MIN;
		if ( out >( float )MAX ) out = ( float )MAX;

		return	out;
	}

	//	�J�����ʒu�v�Z
	void	Camera::CalcCameraPos( void ) 
	{
		Vector3 vec;
		vec = Vector3( 0.0f, 40.0f, -50.0f ) - Vector3( 0.0f, 2.0f, 0.0f );
		vec.Normalize();
		this->pos = this->target + vec *length *3;
	}

//------------------------------------------------------------------------------------------
//	���ݒ�
//------------------------------------------------------------------------------------------

	//	�v���C���[���n��
	void	Camera::SetPlayerInfo( const Vector3& p_1, const Vector3& p_2, const Vector3& p_3, const Vector3& p_4 )
	{
		playerPos[0] = p_1;
		playerPos[1] = p_2;
		playerPos[2] = p_3;
		playerPos[3] = p_4;
		if (p_1.y < -20.0f) playerPos[0].y = 0;
		if (p_2.y < -20.0f) playerPos[1].y = 0;
		if (p_3.y < -20.0f) playerPos[2].y = 0;
		if (p_4.y < -20.0f) playerPos[3].y = 0;
	}
	
	//	���̈ړ��ꏊ�ݒ�
	void	Camera::SetNextPoint( int num, float speed )
	{
		if ( !moveState )	return;
		this->speed = speed;
		t = 0.0f;
		startPos = nextPoint;
		nextPoint = TITLE_MOVE_INFO::pos[num];
		nextTarget = TITLE_MOVE_INFO::target[num];
	}

//****************************************************************************************
//
//	Spring�N���X
//
//****************************************************************************************

//------------------------------------------------------------------------------------------
//	�O���[�o���ϐ�
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	Rubber::Rubber( void ) : mass( FLT_MAX ), position( 0, 0, 0 ), velocity( 0, 0, 0 ),
						acceleration( 0, 0, 0 ), resultant( 0, 0, 0 ), init_flag( true )
	{

	}

	//	�f�X�g���N�^
	Rubber::~Rubber( void )
	{

	}

//------------------------------------------------------------------------------------------
//	�X�V
//------------------------------------------------------------------------------------------

	//	�X�V
	void	Rubber::Update( void )
	{
		static DWORD last = timeGetTime();
		DWORD elapse = timeGetTime() - last;
		if (elapse > 0) Integrate((FLOAT)elapse / 1000.0f);
		last += elapse;

	}
	
	//	���̂��ׂĂ��X�V
	void	Rubber::Integrate( float dt )
	{
		assert(mass > 0);

		acceleration = (resultant / mass);
		velocity += acceleration * dt;
		position += velocity * dt;

		resultant = Vector3(0, 0, 0);
	}
