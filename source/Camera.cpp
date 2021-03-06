
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	<assert.h>
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
	Camera*	mainView;

//------------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------------
	
	//	コンストラクタ
	Camera::Camera( void )
	{
		q = new Rubber();

		pos = Vector3( 0.0f, 200.0f, -1000.0f );
		q->position = pos;
		q->mass = 1.0f;
		target = Vector3( 0.0f, 0.6f, 0.8f ); 
		orientation = D3DXQUATERNION( 0, 0, 0, 1 );
		nextPoint = TITLE_MOVE_INFO::pos[TITLE_TARGET::PLAY];
		startPos = pos;
		t = 0.0f;
		moveState = false;
		target = nextTarget = TITLE_MOVE_INFO::target[TITLE_TARGET::PLAY];
		speed = 0.005f;

		FOR(0, PLAYER_NUM)
		{
			p[value] = new Rubber();
			p[value]->position = pos;
			p[value]->mass = 1.0f;
		}

		//振動関連
		adjust = Vector3(0.0f, 0.0f, 0.0f);
		shakeflag = false;
		shakeTimer = 0;
		wide = 0;

		Set(pos + adjust, target + adjust);
	}

	//	デストラクタ
	Camera::~Camera( void )
	{

	}

//------------------------------------------------------------------------------------------
//	更新
//------------------------------------------------------------------------------------------

	//	更新
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
	}

	//	描画
	void	Camera::Render( void )
	{

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
		Set(pos + adjust, target + adjust);
	}

	//	追いかけカメラ
	void	Camera::ModeChase( void )
	{
		//-----------------------------------
		//	ターゲットプレイヤー
		//-----------------------------------
		FOR(0, PLAYER_NUM){
			//	初回のみq->positionに現在のposを与える
			p[value]->FastInitialize(playerPos[value]);

			//	理想地点への移動
			p[value]->Move(playerPos[value]);

			//	情報更新
			p[value]->Update();
		}

		//	中心座標・中心からの最大距離・カメラ位置計算
		this->target = CalcCenterPos();
		this->length = CalcMaxDist();
		CalcCameraPos();

	
		//	弾力性
		SpringMove(pos);

		////	回転補間
		//Slerp( this->target, 0.1f );

		//振動情報設定
		Shake();
		//	情報設定
		Set(q->position + adjust, this->target + adjust);

	}

	//	球面線形補間使用
	void	Camera::ModeSlerp( Vector3 target )
	{
		Slerp( target, 0.1f );

		Set(pos + adjust, this->target + adjust);
	}

	//	リザルト用カメラ
	void	Camera::ModeResult( void )
	{
		Set(Vector3(0.0f, 5.0f, 13.0f) + adjust, Vector3(0.0f, 5.0f, -30.0f) + adjust);
	}

	//	タイトル用カメラ
	void	Camera::ModeTitle( void )
	{

		//	移動保管
		CubicFunctionInterpolation( pos, startPos, nextPoint, t );

		//	パラメータ加算
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
		
		//	回転
		Slerp( nextTarget, 0.1f );

		//	情報更新
		Set(pos + adjust, this->target + adjust);
	}

	//	個々監視カメラ
	void	Camera::ModeIndividualSurveillance( Vector3 target )
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
		this->wide = wide / ( float )timer;
		this->shakeTimer = timer;
	}

	//	弾性力を使ったカメラ移動
	void	Camera::SpringMove( Vector3 position )
	{
		//-----------------------------------
		//	カメラ
		//-----------------------------------

		//	初回のみq->positionに現在のposを与える
		q->FastInitialize(position);	

		//	理想地点への移動
		q->Move(position);

		//	情報更新
		q->Update();

	}

//------------------------------------------------------------------------------------------
//	数値計算
//------------------------------------------------------------------------------------------

	//	ターゲット中心座標計算
	Vector3	Camera::CalcCenterPos( void )
	{
		Vector3	out = Vector3( 0.0f, 0.0f, 0.0f );

		for ( int i = 0; i < PLAYER_NUM; i++ )	out += p[i]->position;

		out /= PLAYER_NUM;

		return	out;
	}

	//	最大距離算出
	float	Camera::CalcMaxDist( void )
	{
		float	len[PLAYER_NUM];
		Vector3 pos[PLAYER_NUM];
		float	temp;
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			pos[i] = CheckPosLength(playerPos[i]);

		}

		//	それぞれのposからターゲットまでの距離をとる
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			pos[i] = this->target - pos[i];
			len[i] = pos[i].Length();
		}

		//	ソート
		for (int i = 0; i < PLAYER_NUM; ++i)
		{
			//	後ろから順番にチェックしていく
			for (int s = 3; s > i; --s)
			{
				//	一つ下の要素と比較
				if (len[s] >	len[s - 1])
				{
					//	一時的に退避
					temp = len[s - 1];

					//	交換
					len[s - 1] = len[s];

					//	退避してたやつを戻す
					len[s] = temp;
				}
			}
		}

		float	out = 0.0f;
		//	カメラからターゲットまでの長さ調整
		out = len[0];
		//if ( out < ( float )MIN ) out = ( float )MIN;
		//if ( out >( float )MAX ) out = ( float )MAX;

		return	out;
	}

	//	カメラ位置計算
	void	Camera::CalcCameraPos( void ) 
	{
		Vector3 vec;
		vec = Vector3( 0.0f, 35.0f, -50.0f ) - Vector3( 0.0f, 2.0f, 0.0f );
		vec.Normalize();
		this->pos = this->target + vec *length *3;
	}

	//	Lengthの上限下限管理関数
	Vector3	Camera::CheckPosLength( Vector3 position )
	{
		//	positionからターゲットへのベクトル
		Vector3		work = target - position;
		float		len = work.Length();

		//		最少
		if ( len < ( float )MIN )	len = ( float )MIN;
		//		最大
		if ( len >( float )MAX )	len = ( float )MAX;
		//	単位ベクトル化
		work.Normalize();

		//	ターゲットから単位ベクトルの逆方向にlen分のposを返す
		return  target  + ( -work * len);
	}

	//	視野角の計算
	void	Camera::CheckViewAngle( void )
	{
		//	カメラからターゲットへのベクトル
		Vector3 c_t = target - ( q->position + adjust );
		Vector3 c_p[PLAYER_NUM];


		float ct_len = c_t.Length();
		float cp_len[PLAYER_NUM];
		float work; 


		for (int i = 0; i < PLAYER_NUM; i++)
		{
			//	カメラから各プレイヤーへのベクトル
			c_p[i] = playerPos[i] - ( q->position + adjust );
			cp_len[i] = c_p[i].Length();

			//	内積とベクトル長さを使ってcosθを求める
			work = Vector3Dot(c_t, c_p[i]) / (ct_len * cp_len[i]);

			//	角度を求める
			playerAngle[i] = acosf( work );

			//playerAngle[i] = playerAngle[i] * 180.0f / PI;

		}
	}

//------------------------------------------------------------------------------------------
//	情報設定
//------------------------------------------------------------------------------------------

	//	プレイヤー情報渡し
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
	
	//	次の移動場所設定
	void	Camera::SetNextPoint( int num, float speed )
	{
		if ( !moveState )	return;
		this->speed = speed;
		t = 0.0f;
		startPos = nextPoint;
		nextPoint = TITLE_MOVE_INFO::pos[num];
		nextTarget = TITLE_MOVE_INFO::target[num];
	}

//------------------------------------------------------------------------------------------
//	情報取得
//------------------------------------------------------------------------------------------

	//	バネ移動したカメラポジションを取得
	Vector3 Camera::GetSpringPos( void )
	{
		return q->position;
	}



//****************************************************************************************
//
//	Springクラス
//
//****************************************************************************************

//------------------------------------------------------------------------------------------
//	グローバル変数
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------------
	
	//	コンストラクタ
	Rubber::Rubber( void ) : mass( FLT_MAX ), position( 0, 0, 0 ), velocity( 0, 0, 0 ),
						acceleration( 0, 0, 0 ), resultant( 0, 0, 0 ), init_flag( true )
	{

	}

	//	デストラクタ
	Rubber::~Rubber( void )
	{

	}

//------------------------------------------------------------------------------------------
//	更新
//------------------------------------------------------------------------------------------

	//	初回のみ初期化
	void	Rubber::FastInitialize( Vector3 pos )
	{
		if (!init_flag)	return;

		position = pos;
		init_flag = false;
	}

	//	理想地点への移動
	void	Rubber::Move( Vector3 pos )
	{
		Vector3 n = pos - position;
		float len = n.Length();
		float F = -2 * (5 - len);
		n.Normalize();
		Vector3 drag = -velocity * 1.0f;

		AddForce((n*F) + drag);
	}

	//	更新
	void	Rubber::Update( void )
	{
		static DWORD last = timeGetTime();
		DWORD elapse = timeGetTime() - last;
		if (elapse > 0) Integrate((FLOAT)elapse / 1000.0f);
		last += elapse;
	}
	
	//	情報のすべてを更新
	void	Rubber::Integrate( float dt )
	{
		assert(mass > 0);

		acceleration = (resultant / mass);
		velocity += acceleration * dt;
		position += velocity * dt;

		resultant = Vector3(0, 0, 0);
	}
