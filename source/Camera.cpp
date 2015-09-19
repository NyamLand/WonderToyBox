
#include	"iextreme.h"
#include	"system/System.h"

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
	Camera*	m_Camera;

//------------------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	Camera::Camera( void )
	{
		pos = Vector3( 0.0f, 40.0f, 50.0f );
		target = Vector3( 0.0f, 2.0f, 0.0f );
		orientation = D3DXQUATERNION( 0, 0, 0, 1 );
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
	void	Camera::Update( VIEW_MODE mode, Vector3 target )
	{
		switch ( mode )
		{
		case  VIEW_MODE::FIX:
			ModeFix( target );
			break;

		case	VIEW_MODE::SLERP:
			ModeSlerp( target );
			break;

		case	VIEW_MODE::CHASE:
			ModeChase();
			break;
		}

		shader3D->SetValue("ViewPos", m_Camera->GetPos());
		shader3D->SetValue("matView", m_Camera->GetMatrix());
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
		Slerp(this->target, 0.1f);

		Set( pos, this->target );
	}

	//	���ʐ��`��Ԏg�p
	void	Camera::ModeSlerp( Vector3 target )
	{
		Slerp( target, 0.1f );

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
//------------------------------------------------------------------------------------------
//	���ݒ�
//------------------------------------------------------------------------------------------
	void	Camera::SetPos( const Vector3 p_1, const Vector3 p_2, const Vector3 p_3, const Vector3 p_4 )
	{
		float	len[4];
		Vector3 pos[4];
		float	temp;
		pos[0] = p_1;
		pos[1] = p_2;
		pos[2] = p_3;
		pos[3] = p_4;

		//	���ꂼ���pos����^�[�Q�b�g�܂ł̋������Ƃ�
		for ( int i = 0; i < 4; i++ )
		{
			pos[i] = this->target - pos[i];
			len[i] = pos[i].Length();
		}

		//	�\�[�g
		for ( int i = 0; i < 4; ++i )
		{
			//	��납�珇�ԂɃ`�F�b�N���Ă���
			for ( int s = 3; s > i; --s )
			{
				//	����̗v�f�Ɣ�r
				if ( len[s] >	len[s - 1] )
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
		//	�J��������^�[�Q�b�g�܂ł̒�������
		length = len[0];
		if ( length < (float)MIN ) length = (float)MIN;
		if ( length > (float)MAX ) length = (float)MAX;
		

		Vector3 target;

		target = Vector3( 0.0f, 40.0f, 50.0f ) - Vector3( 0.0f, 2.0f, 0.0f );
		target.Normalize();
		this->pos = this->target +target * length * 3;

	}


