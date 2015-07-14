
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"BaseObj.h"

#include	"Player.h"

//****************************************************************************************
//
//	Playerクラス
//
//****************************************************************************************

//-----------------------------------------------------------------------------------------
//	グローバル
//-----------------------------------------------------------------------------------------

	//	プレイヤーのステータス
	namespace PlayerStatus
	{

	}

//-----------------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------------

	//	コンストラクタ
	Player::Player( void )
	{
		//	パラメータ初期化
		speed = 0.2f;
		scale = 0.02f;
	}

	//	デストラクタ
	Player::~Player( void )
	{

	}

//-----------------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------------

	//	更新
	void	Player::Update( void )
	{
		//	動作分け
		switch ( mode )
		{
		case MOVE:
			Move();
			break;
			
		case ATTACK:
			Attack();
			break;
		}

		//	更新
		BaseObj::Update();
	}

//-----------------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------------

	//	移動
	void	Player::Move( void )
	{
		//	左スティックの入力チェック
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

		//	攻撃
		if ( input->Get( KEY_A ) == 3 )		mode = ATTACK;
	}

	//	移動
	void	Player::Move( float speed )
	{
		move.x	 = sinf( angle ) * speed;
		move.z	 = cosf( angle ) * speed;
	}

	//	攻撃
	void	Player::Attack( void )
	{
		SetMotion( Y2009Motion::ATTACK1 );
		int		frame = obj->GetFrame();

		//	少し前進
		float	moveSpeed = 0.05f;
		move.x = moveSpeed * sinf(angle);
		move.z = moveSpeed * cosf(angle);

		//	攻撃判定
		if ( frame >= 377 && frame <= 385 )		attackParam = 1;
		else attackParam = 0;

		//	モーション終了時に
		if ( frame >= 410 )		mode = MOVE;
	}

	//	ダメージ
	void	Player::Damage( void )
	{
		
	}

//-----------------------------------------------------------------------------------------
//	角度補正関数
//-----------------------------------------------------------------------------------------

	//	角度調整
	void	Player::AngleAdjust( float speed )
	{
		if ( !( input->Get( KEY_AXISX ) || input->Get( KEY_AXISY ) ) )	return;

		//	左スティックの入力
		float axisX = input->Get( KEY_AXISX ) * 0.001f;
		float	axisY = -input->Get( KEY_AXISY ) * 0.001f;

		//	カメラの前方方向を求める
		Vector3	vEye( m_Camera->GetTarget() - m_Camera->GetPos() );
		float	cameraAngle = atan2f( vEye.x, vEye.z );

		//	入力方向を求める
		float inputAngle = atan2f( axisX, axisY );

		//	目標の角度を求める
		float	targetAngle = cameraAngle + inputAngle;

		//	親に投げる
		AngleAdjust( Vector3( sinf( targetAngle ), 0.0f, cosf( targetAngle ) ), speed );
	}

	//	角度調整
	void	Player::AngleAdjust( const Vector3& direction, float speed )
	{
		//	現在の向きと目標の向きの差を求める
		float	targetAngle( atan2f( direction.x, direction.z ) );
		float	dAngle( targetAngle - GetAngle() );

		//	差の正規化
		if ( dAngle > 1.0f * PI )	dAngle -= 2.0f * PI;
		if ( dAngle < -1.0f * PI )	dAngle += 2.0f * PI;

		//	差をspeed分縮める
		if ( dAngle > 0.0f ){
			dAngle -= speed;
			if ( dAngle < 0.0f )  dAngle = 0.0f;
		}
		else{
			dAngle += speed;
			if ( dAngle > 0.0f )	dAngle = 0.0f;
		}

		//	向きに反映
		SetAngle( targetAngle - dAngle );

		//	プレイヤーの向きがπ以上にならないように調整する
		if ( GetAngle() >= 1.0f * PI )	angle -= 2.0f * PI;
		if ( GetAngle() <= -1.0f * PI )	angle += 2.0f * PI;
	}
