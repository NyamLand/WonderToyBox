
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"

#include	"BaseObj.h"

//****************************************************************************************
//
//	BaseObjクラス
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------------
	
	//	コンストラクタ
	BaseObj::BaseObj( void ) : obj( NULL ),
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ), power( 0 ), diffence( 0 ), knockBackVec( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f ), speed( 0.0f ), mode( 0 ), unrivaled( false ),
		attackParam( 0 ), attackPos( 0.0f, 0.0f, 0.0f ), attackPos_top( 0.0f, 0.0f, 0.0f ), attackPos_bottom( 0.0f, 0.0f, 0.0f ), attack_r( 0.0f ), attack_t( 0.0f ), knockBackType( 0 ),
		isGround(true), coinNum(0), force(0.0f), type(0), p_num( 0 )
	{
		
	}

	//	デストラクタ
	BaseObj::~BaseObj( void )
	{
		SafeDelete( obj );
	}

	//	初期化
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
//	更新・描画
//-------------------------------------------------------------------------------------

	//	更新
	void	BaseObj::Update( void )
	{
		//	重力加算
		move.y += GRAVITY;

		//	ステージ当たり判定
		StageCollisionCheck();

		//	移動値加算
		pos += move;

		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Animation();
		obj->Update();
	}

	//	描画
	void	BaseObj::Render( void )
	{
		obj->Render();
	}

	//	シェーダー付き描画
	void	BaseObj::Render( iexShader* shader, LPSTR technique )
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------------

	//	ステージ当たり判定チェック
	void	BaseObj::StageCollisionCheck( void )
	{
		//　床判定
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

		//	壁判定
		Collision::CheckWall( pos, move );
	}
	
	//	コイン枚数加算
	void	BaseObj::AddCoin( void )
	{
		coinNum++;
	}

	//	コイン枚数減算
	void	BaseObj::SubCoin( void )
	{
		if ( coinNum > 0 )	coinNum--;
	}

	//	角度調整
	void	BaseObj::AngleAdjust( float speed )
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
	void	BaseObj::AngleAdjust( const Vector3& direction, float speed )
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
		if ( GetAngle() >= 1.0f * PI )		angle -= 2.0f * PI;
		if ( GetAngle() <= -1.0f * PI )	angle += 2.0f * PI;
	}

	//	共通動作
	void	BaseObj::CommonMove( void )
	{
		//	左スティックの入力チェック
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
			
			//	徐々に移動量と力減らす
			move.x *= 0.8f;
			move.z *= 0.8f;
		}
	}

	void	BaseObj::CommonMove( float speed )
	{
		move.x = sinf( angle ) * speed;
		move.z = cosf( angle ) * speed;
	}

	//	ジャンプ
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

		//	移動
		CommonMove();

		//	接地してたら
		if ( isGround )	mode = PlayerData::MOVE;
	}

	//	ガード
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

	//	ノックバック　強
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

	//	ノックバック	共通
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

	//	力加算
	void	BaseObj::AddForce( float force )
	{
		this->force = force;
	}

//-------------------------------------------------------------------------------------
//	情報設定・取得
//-------------------------------------------------------------------------------------

	//	設定
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

	//	取得
	Vector3		BaseObj::GetPos( void ){ return	pos; }
	Matrix		BaseObj::GetMatrix( void ){ return obj->TransMatrix; }
	float			BaseObj::GetAngle( void ){ return angle; }
	bool			BaseObj::GetUnrivaled( void ){ return unrivaled; }
	int				BaseObj::GetCoinNum( void ){ return	coinNum; }
	int				BaseObj::GetMode( void ){ return mode; }
	int				BaseObj::GetType( void ){ return type; }
	int				BaseObj::GetP_Num( void ){ return p_num; }

	//	当たり判定用パラメータ取得
	int				BaseObj::GetAttackParam( void ){ return attackParam; }
	int				BaseObj::GetKnockBackType( void ){ return knockBackType; }
	Vector3		BaseObj::GetAttackPos( void ){ return attackPos; }
	Vector3		BaseObj::GetAttackPos_Top( void ){ return attackPos_top; }
	Vector3		BaseObj::GetAttackPos_Bottom( void ){ return attackPos_bottom; }
	float			BaseObj::GetAttack_T( void ){ return attack_t; }
	float			BaseObj::GetAttack_R( void ){ return attack_r; }
