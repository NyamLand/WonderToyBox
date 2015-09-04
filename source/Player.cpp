
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"PlayerManager.h"
#include	"Player.h"

//****************************************************************************************
//
//	Playerクラス
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------------

	//	コンストラクタ
	Player::Player( void ) : obj( NULL ),
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ), power( 0 ), diffence( 0 ), knockBackVec( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f ), speed( 0.0f ), mode( 0 ), unrivaled( false ),
		attackParam( 0 ), attackPos( 0.0f, 0.0f, 0.0f ), attackPos_top( 0.0f, 0.0f, 0.0f ), attackPos_bottom( 0.0f, 0.0f, 0.0f ), attack_r( 0.0f ), attack_t( 0.0f ), knockBackType( 0 ),
		isGround(true), force(0.0f), type(0), p_num(0), CanHyper( true )
	{

	}

	//	デストラクタ
	Player::~Player( void )
	{
		SafeDelete( obj );
	}

	//	初期化
	bool	Player::Initialize( int input, iex3DObj* org, Vector3 pos )
	{
		this->obj = org;
		this->input = ::input[input];
		this->p_num = input;
		this->pos = pos;
		this->passDamageColor = PlayerData::DAMAGE_COLOR[input];

		if ( obj == NULL )	return	false;
		return	true;
	}

//-------------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------------

	//	更新
	void	Player::Update( void )
	{
		ModeManagement();
		CommonUpdate();
	}
	
	//	描画
	void	Player::Render( void )
	{
		CommonRender();
	}

	//	共通更新
	void	Player::CommonUpdate( void )
	{
		//	重力加算
		move.y += GRAVITY;

		//	ステージ当たり判定
		StageCollisionCheck();

		//	移動値加算
		pos += move;

		//	情報更新
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Animation();
		obj->Update();
	}

	//	共通描画
	void	Player::CommonRender( void )
	{
		obj->Render();
	}

	//	共通シェーダー付き描画
	void	Player::CommonRender( iexShader* shader, LPSTR technique )
	{

		//	ダメージ時白化計算
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
//	動作関数
//-------------------------------------------------------------------------------------

	//	モード管理
	void	Player::ModeManagement( void )
	{
		switch ( mode )
		{
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

	//	ステージ当たり判定チェック
	void	Player::StageCollisionCheck( void )
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
		if ( GetAngle() >= 1.0f * PI )		angle -= 2.0f * PI;
		if ( GetAngle() <= -1.0f * PI )	angle += 2.0f * PI;
	}

	//	共通動作
	void	Player::CommonMove( void )
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

	void	Player::CommonMove( float speed )
	{
		move.x = sinf( angle ) * speed;
		move.z = cosf( angle ) * speed;
	}

	//	ジャンプ
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

		//	移動
		CommonMove();

		//	接地してたら
		if ( isGround )	mode = PlayerData::MOVE;
	}

	//	ガード
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

	//	ノックバック　強
	void	Player::CommonKnockBackStrength( void )
	{
		AddForce( 0.3f );

		move = knockBackVec * force;
		mode = PlayerData::DAMAGE;
	}

	//	ノックバック	共通
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

	//	力加算
	void	Player::AddForce( float force )
	{
		this->force = force;
	}

	//	モードMove
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

	//	モードAttack
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

		//	モーション終了時に
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

	//	モードJump
	void	Player::Jump( void )
	{
		CommonJump();
	}

	//	モードGuard
	void	Player::Guard( void )
	{
		move.x = move.z = 0.0f;
		SetMotion( motionData.GUARD );
		CommonGuard();
	}

	//	モードDamage
	void	Player::Damage( void )
	{
		CommonKnockBack();
	}

//-------------------------------------------------------------------------------------
//	情報設定・取得
//-------------------------------------------------------------------------------------

	//	設定
	void	Player::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion( motion );
		}
	}
	void	Player::SetMode( int mode )
	{
		if ( this->mode != mode )		this->mode = mode;
	}
	void	Player::SetPos( Vector3 pos ){ this->pos = pos; }
	void	Player::SetPos( float x, float y, float z ){ this->pos = Vector3( x, y, z ); }
	void	Player::SetAngle( float angle ){ this->angle = angle; }
	void	Player::SetScale( float scale ){ this->scale = scale; }
	void	Player::SetKnockBackVec( Vector3 knockBackVec ){ this->knockBackVec = knockBackVec; }
	void	Player::SetMode( PlayerData::STATE state )
	{
		if (GetMode() != state)
		{
			mode = state;
		}
	}
	void	Player::SetType( int type ){ this->type = type; }
	void	Player::SetDamageColor( Vector3 color ){ this->colorParam = color; }
	void	Player::SetReceiveColor( Vector3 color ){ this->receiveDamageColor = color; }

	//	取得
	Vector3		Player::GetPos( void ){ return	pos; }
	Matrix		Player::GetMatrix( void ){ return obj->TransMatrix; }
	float		Player::GetAngle( void ){ return angle; }
	bool		Player::GetUnrivaled( void ){ return unrivaled; }
	int			Player::GetMode( void ){ return mode; }
	int			Player::GetType( void ){ return type; }
	int			Player::GetP_Num( void ){ return p_num; }
	Vector3		Player::GetDamageColor( void ){ return	passDamageColor; }
	bool		Player::GetCanHyper( void ){ return CanHyper; }

	//	当たり判定用パラメータ取得
	int			Player::GetAttackParam( void ){ return attackParam; }
	int			Player::GetKnockBackType( void ){ return knockBackType; }
	Vector3		Player::GetAttackPos( void ){ return attackPos; }
	Vector3		Player::GetAttackPos_Top( void ){ return attackPos_top; }
	Vector3		Player::GetAttackPos_Bottom( void ){ return attackPos_bottom; }
	float		Player::GetAttack_T( void ){ return attack_t; }
	float		Player::GetAttack_R( void ){ return attack_r; }

