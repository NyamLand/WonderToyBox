
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Camera.h"
#include	"Block.h"

//*********************************************************************************
//
//	Blockクラス
//
//*********************************************************************************

Block*	m_Block;

//-------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------

	//	コンストラクタ
	Block::Block( void )
	{
	
	}
	
	//	デストラクタ
	Block::~Block( void )
	{
		SafeDelete( box );
	}

	//	初期化
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
//	更新・描画
//-------------------------------------------------------------------------

	//	更新
	void	Block::Update( void )
	{
		Move();
		pos += move;
		box->SetPos( pos );
		box->SetAngle( angle );
		box->SetScale( scale );
		box->Update();
	}

	//	描画
	void	Block::Render( void )
	{
		box->Render();
	}


	//-----------------------------------------------------------------------------------------
	//	動作関数
	//-----------------------------------------------------------------------------------------

	//	移動
	void	Block::Move( void )
	{
		//	左スティックの入力チェック
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

	//	移動
	void	Block::Move( float speed )
	{
		move.x = sinf( angle ) * speed;
		move.z = cosf( angle ) * speed;
	}

	//-----------------------------------------------------------------------------------------
	//	角度補正関数
	//-----------------------------------------------------------------------------------------

	//	角度調整
	void	Block::AngleAdjust( float speed )
	{
		if (!(input->Get( KEY_AXISX ) || input->Get( KEY_AXISY ) ) )		return;

		//	左スティックの入力
		float axisX = input->Get( KEY_AXISX ) * 0.001f;
		float	axisY = -input->Get(KEY_AXISY) * 0.001f;

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
	void	Block::AngleAdjust( const Vector3& direction, float speed )
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

	//	設定
	void	Block::SetPos( Vector3 pos ){ this->pos = pos; }
	void	Block::SetPos( float x, float y, float z ){ this->pos = Vector3( x, y, z ); }
	void	Block::SetAngle( float angle ){ this->angle = angle; }
	void	Block::SetScale( float scale ){ this->scale = scale; }

	//	取得
	Vector3		Block::GetPos( void ){ return	pos; }
	Vector3		Block::GetAttackPos( void ){ return attackPos; }
	float			Block::GetAngle( void ){ return angle; }
	int				Block::GetAttackParam( void ){ return attackParam; }






