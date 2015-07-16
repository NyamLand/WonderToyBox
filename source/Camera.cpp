
#include	"iextreme.h"

#include	"Camera.h"

//****************************************************************************************
//
//	Cameraクラス
//
//****************************************************************************************

//------------------------------------------------------------------------------------------
//	グローバル変数
//------------------------------------------------------------------------------------------

	//	実体の宣言
	Camera*	m_Camera;

//------------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------------
	
	//	コンストラクタ
	Camera::Camera( void )
	{
		pos = Vector3( 0.0f, 20.0f, -25.0f );
		target = Vector3( 0.0f, 2.0f, 0.0f );
		orientation = D3DXQUATERNION( 0, 0, 0, 1 );
		Set( pos, target );
	}

	//	デストラクタ
	Camera::~Camera( void )
	{

	}

//------------------------------------------------------------------------------------------
//	更新
//------------------------------------------------------------------------------------------

	//	更新
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
		}
	}

//------------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------------

	//	球面線形補間
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

	//	固定カメラ
	void	Camera::ModeFix( Vector3 target )
	{
		Set( pos, target );
	}

	//	球面線形補間使用
	void	Camera::ModeSlerp( Vector3 target )
	{
		Slerp( target, 0.1f );

		Set( pos, this->target );
	}

	//	振動
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

	//	振動設定
	void	Camera::ShakeSet( float wide, int timer )
	{
		if ( shakeflag ) 	return;
		srand( 0 );
		shakeflag = true;
		this->wide = wide / ( float )shakeTimer;
		this->shakeTimer = timer;
	}
