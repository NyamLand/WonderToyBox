
#include	"iextreme.h"
#include	"GlobalFunction.h"

//*****************************************************************************
//
//	グローバル関数
//
//*****************************************************************************

//----------------------------------------------------------------------------
//	変数
//----------------------------------------------------------------------------

	//	ストリームサウンド
	LPDSSTREAM		stream;
	bool					debug = true;

//----------------------------------------------------------------------------
//	システム
//----------------------------------------------------------------------------

	//	解放
	void	SafeDelete( void* obj )
	{
		if ( obj != nullptr )
		{
			delete	obj;
			obj = nullptr;
		}
	}

	//	変数変換
	POINT	GetPoint( int x, int y )
	{
		POINT	p;
		p.x = x;
		p.y = y;
		return p;
	}

	DWORD GetColor( float r, float g, float b, float a )
	{
		DWORD	ret;
		ret = ARGB( ( DWORD )( 0xFF * a ), ( DWORD )( 0xFF * r ), ( DWORD )( 0xFF * g ), ( DWORD )( 0xFF * b ) );
		return	ret;
	}

	DWORD GetColor( Vector3 color )
	{
		DWORD	ret;
		ret = ARGB( 0xFF, ( DWORD )( 0xFF * color.x ), ( DWORD )( 0xFF * color.y ), ( DWORD )( 0xFF * color.z ) );
		return ret;
	}

	DWORD GetColor( Vector3 color, float alpha )
	{
		DWORD	ret;
		ret = ARGB( ( DWORD )( 0xFF * alpha ), ( DWORD )( 0xFF * color.x ), ( DWORD )( 0xFF * color.y ), ( DWORD )( 0xFF * color.z ) );
		return ret;
	}

	//	ワールド座標からスクリーン座標に変換
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat )
	{
		//	頂点×行列で射影する
		Vector3	temp;
		temp.x = pos.x *mat._11 + pos.y * mat._21 + pos.z * mat._31 + 1.0f * mat._41;
		temp.y = pos.x *mat._12 + pos.y * mat._22 + pos.z * mat._32 + 1.0f * mat._42;
		temp.z = pos.x *mat._13 + pos.y * mat._23 + pos.z * mat._33 + 1.0f * mat._43;
		float w = pos.x *mat._14 + pos.y * mat._24 + pos.z * mat._34 + 1.0f * mat._44;
		temp.x /= w;
		temp.y /= w;
		temp.z /= w;

		//	画面座標に換算(ビューポート変換)
		long WinWide, WinHeight;
		WinWide = iexSystem::ScreenWidth;
		WinWide /= 2;
		WinHeight = iexSystem::ScreenHeight;
		WinHeight /= 2;

		out.x = ( temp.x *WinWide + WinWide );
		out.y = ( -temp.y*WinHeight + WinHeight );
		out.z = temp.z;

		if ( temp.x >  1.0f ) return false;
		if ( temp.x < -1.0f ) return false;
		if ( temp.y >  1.0f ) return false;
		if ( temp.y < -1.0f ) return false;
		if ( temp.z <  0 )	return false;
		return true;
	}

	//	モーション番号登録
	void	SetMotionNum( int& motionData, int motionNum )
	{
		motionData = motionNum;
	}

	//	デバッグ文字
	void	DrawString( LPSTR string, int x, int y, DWORD color )
	{
		IEX_DrawText( string, x, y, 300, 300, color );
	}
	
	void	DrawString( LPSTR string, int x, int y, float r, float g, float b )
	{
		IEX_DrawText(string, x, y, 300, 300, ARGB(0xFF, ( DWORD )( 0xFF * r ), ( DWORD )( 0xFF * g ), ( DWORD )( 0xFF * b ) ) );
	}
	
	void	DrawString( LPSTR string, int x, int y, Vector3 color )
	{
		IEX_DrawText( string, x, y, 300, 300, ARGB( 0xFF, ( DWORD )( 0xFF * color.x ), ( DWORD )( 0xFF * color.y ), ( DWORD )( 0xFF * color.z ) ) );
	}


//----------------------------------------------------------------------------
//	画像関連
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	３次関数補間( 出力、始点、終点、現在の割合( 0.0f ~ 1.0f ) )
//----------------------------------------------------------------------------
	
	//	効果音鳴らす
	void	SetSound( int type, bool loop )
	{
		if ( !IEX_GetSoundStatus( type ) )
		{
			IEX_PlaySound( type, loop );
		}
	}
	
	//	BGMならす
	void	PlayBGM( int type )
	{
		stream = IEX_PlayStreamSound( SoundInfo::bgm[type] );
	}

	//	BGM止める
	void	StopBGM( void )
	{
		IEX_StopStreamSound( stream );
	}

//----------------------------------------------------------------------
//	線形補間( 出力、開始値、最終値, 割合 )
//----------------------------------------------------------------------

	bool	Lerp( Vector3& out, Vector3 p1, Vector3 p2, float t )
	{
		if ( t >= 1.0f )	return	true;

		out = p1 * ( 1 - t ) + p2 * t ;

		return	false;
	}

	bool	Lerp( float& out, float p1, float p2, float t )
	{
		if ( t >= 1.0f )	return	true;

		out = p1 * ( 1 - t ) + p2 * t;

		return	false;
	}

//----------------------------------------------------------------------------
//	３次関数補間( 出力、始点、終点、現在の割合( 0.0f ~ 1.0f ) )
//----------------------------------------------------------------------------

	//	Vector3
	bool	CubicFunctionInterpolation( Vector3& out, Vector3 p1, Vector3 p2, float t )
	{
		if ( t >= 1.0f )	return	true;
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3次関数補間値に変換

		out = p1 * ( 1.0f - rate ) + p2 * rate;   // いわゆるLerp
		return	false;
	}

	//	float
	bool	CubicFunctionInterpolation( float& out, float p1, float p2, float t )
	{
		if ( t >= 1.0f )	return	true;
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3次関数補間値に変換

		out = p1 * ( 1.0f - rate ) + p2 * rate;   // いわゆるLerp

		return	false;
	}

//----------------------------------------------------------------------------
//	ベジェ曲線( 出力、始点、制御点、終点、現在の割合( 0.0f ~ 1.0f ) )
//----------------------------------------------------------------------------

	//	２次ベジェ曲線
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 end, float t )
	{
		if ( t >= 1.0f )	return true;

		out.x = ( ( 1 - t ) * ( 1 - t ) ) * start.x + ( ( 2 * ( 1 - t ) * t * p1.x ) + ( ( t * t ) * end.x ) );
		out.y = ( ( 1 - t ) * ( 1 - t ) ) * start.y + ( ( 2 * ( 1 - t ) * t * p1.y ) + ( ( t * t ) * end.y ) );
		out.z = ( ( 1 - t ) * ( 1 - t ) ) * start.z + ( ( 2 * ( 1 - t ) * t * p1.z ) + ( ( t * t ) * end.z ) );

		return	false;
	}
	
	//	３次ベジェ曲線
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 p2, Vector3 end, float t )
	{
		if ( t >= 1.0f )	return true;

		out.x = ( ( 1 - t ) * ( 1 - t ) * ( 1 - t ) ) * start.x + ( 3 * ( ( 1 - t ) * ( 1 - t ) ) * t * p1.x ) + ( 3 * ( 1 - t ) * ( t * t ) * p2.x ) + ( t * t * t ) * end.x;
		out.y = ( ( 1 - t ) * ( 1 - t ) * ( 1 - t ) ) * start.y + ( 3 * ( ( 1 - t ) * ( 1 - t ) ) * t * p1.y ) + ( 3 * ( 1 - t ) * ( t * t ) * p2.y ) + ( t * t * t ) * end.y;
		out.z = ( ( 1 - t ) * ( 1 - t ) * ( 1 - t ) ) * start.z + ( 3 * ( ( 1 - t ) * ( 1 - t ) ) * t * p1.z ) + ( 3 * ( 1 - t ) * ( t * t ) * p2.z ) + ( t * t * t ) * end.z;

		return	false;
	}

//----------------------------------------------------------------------------
//	高次元ベジェ曲線パラメータ取得( 0.0f ~ 1.0f )
//----------------------------------------------------------------------------

	const static int nPasTgl[12][12] = {
		{ 1 },
		{ 1, 1 },
		{ 1, 2, 1 },
		{ 1, 3, 3, 1 },
		{ 1, 4, 6, 4, 1 },
		{ 1, 5, 10, 10, 5, 1 },
		{ 1, 6, 15, 20, 15, 6, 1 },
		{ 1, 7, 21, 35, 35, 21, 7, 1 },
		{ 1, 8, 28, 56, 70, 56, 28, 8, 1 },
		{ 1, 9, 36, 84, 126, 126, 84, 36, 9, 1 },
		{ 1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1 },
		{ 1, 11, 55, 165, 330, 464, 464, 330, 165, 55, 11, 1 }
	};

	inline float Get( const float &y1, const float &y2, const float &t, const int &n ){
		float b = t > 1 ? 1 : (t < 0 ? 0 : t);
		float a = 1 - b;
		float ay = 0;
		float y[4] = { 0, y1, y2, 1 };
		int m;
		for (int i = 0; i <= n; i++){
			m = i == 0 ? 0 : (i == n ? 3 : (i <= n / 2 ? 1 : 2));//yの添え字決定
			ay += nPasTgl[n][i] * pow(a, n - i)*pow(b, i)*y[m];//※1
		}
		return ay;
	}

	//	ベジェ曲線パラメータ取得（ 0.0f ~ 1.0f で返す ）　
	float GetBezier( ePrm_t ePrm1, ePrm_t ePrm2, float fRate ){
		int n = 3;                //n次元指定
		float y1, y2;
		switch (ePrm1){
		case eSlow_Lv5: y1 = 0;						n = 11; break;//11次元
		case eSlow_Lv4: y1 = 0;                        n = 9; break;//9次元
		case eSlow_Lv3: y1 = 0;                        n = 7; break;//7次元
		case eSlow_Lv2: y1 = 0;                        n = 5; break;//5次元
		case eSlow_Lv1: y1 = 0;                        n = 3; break;//3次元
		case eNoAccel: y1 = 0.333333f;				n = 3; break;//直線の場合は3次元中1/3の点
		case eRapid_Lv1: y1 = 1;						n = 3; break;//3次元
		case eRapid_Lv2: y1 = 1;						n = 5; break;//5次元
		case eRapid_Lv3: y1 = 1;						n = 7; break;//7次元
		case eRapid_Lv4: y1 = 1;						n = 9; break;//9次元
		case eRapid_Lv5: y1 = 1;						n = 11; break;//11次元
		}
		switch (ePrm2){
		case eSlow_Lv5: y2 = 1;                       n = 11; break;//11次元
		case eSlow_Lv4: y2 = 1;                       n = 9; break;//9次元
		case eSlow_Lv3: y2 = 1;                       n = 7; break;//7次元
		case eSlow_Lv2: y2 = 1;                       n = 5; break;//5次元
		case eSlow_Lv1: y2 = 1;                       n = 3; break;//3次元
		case eNoAccel: y2 = 0.6666667f;              n = 3; break;//直線の場合は3次元中2/3の点
		case eRapid_Lv1: y2 = 0;                       n = 3; break;//3次元
		case eRapid_Lv2: y2 = 0;                       n = 5; break;//5次元
		case eRapid_Lv3: y2 = 0;                       n = 7; break;//7次元
		case eRapid_Lv4: y2 = 0;                       n = 9; break;//9次元
		case eRapid_Lv5: y2 = 0;                       n = 11; break;//11次元
		}

		float	out = Get( y1, y2, fRate, n );
		return out;
	}

//-------------------------------------------------------------------------
//	図形描画
//-------------------------------------------------------------------------

	//	球描画
	void	DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c )
	{
		CONST INT N = 36;

		static _VB vb( d3dd, sizeof( D3DXVECTOR3 ) * ( N + 1 ) );
		for ( int n = 0; n < 3; n++ )
		{
			D3DXVECTOR3 *p;
			vb->Lock( 0, 0, ( void ** )&p, 0 );
			for ( int i = 0; i < N; i++ )
			{
				switch (  n )
				{
				case 0: p[i] = r * D3DXVECTOR3( sinf( 360.0f / N * 0.01745f * i ), cosf( 360.0f / N * 0.01745f * i ), 0 ) + p0; break;
				case 1: p[i] = r * D3DXVECTOR3( sinf( 360.0f / N * 0.01745f * i ), 0, cosf( 360.0f / N * 0.01745f * i ) ) + p0; break;
				case 2: p[i] = r * D3DXVECTOR3( 0, sinf( 360.0f / N * 0.01745f * i ), cosf( 360.0f / N * 0.01745f * i ) ) + p0; break;
				}
			}
			p[N] = p[0];
			vb->Unlock();

			D3DMATERIAL9 m = { 0 };
			m.Diffuse = m.Ambient = D3DXCOLOR( c );
			d3dd->SetMaterial( &m );

			d3dd->SetFVF( D3DFVF_XYZ );
			d3dd->SetStreamSource( 0, vb, 0, sizeof( D3DXVECTOR3 ) );
			d3dd->SetTransform( D3DTS_WORLD, &D3DXMATRIX( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ) );
			d3dd->DrawPrimitive( D3DPT_LINESTRIP, 0, N );
		}
	}

	void	DrawSphere( const Vector3& pos, float r, DWORD color )
	{
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		DrawSphere( iexSystem::GetDevice(), D3DXVECTOR3( pos.x, pos.y, pos.z ), r, color );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	}

	//	カプセル描画
	void	DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, CONST D3DXVECTOR3 &p1, FLOAT r, D3DCOLOR c )
	{
		CONST INT N = 36;
		CONST INT L0 = 0;
		CONST INT L1 = N + 1;
		CONST INT L2 = L1 + N + 1;

		CONST INT L3 = L2 + N * 2;
		CONST INT L4 = L3 + N + 1;
		CONST INT L5 = L4 + N + 1;
		CONST INT L6 = L5 + N + 1;

		static _VB vb(d3dd, sizeof(D3DXVECTOR3)* ((N + 1) * 2 + N * 2 + (N + 1) * 4));
		D3DXVECTOR3 *p;
		vb->Lock(0, 0, (void **)&p, 0);
		D3DXVECTOR3 up = p0 - p1;
		CONST FLOAT L = D3DXVec3Length(&up);
		for (int i = 0; i < N; i++)
		{
			p[L0 + i] = D3DXVECTOR3(r * sinf(360.0f / N * 0.01745f * i), L / 2.0f, r * cosf(360.0f / N * 0.01745f * i));
			p[L1 + i] = D3DXVECTOR3(r * sinf(360.0f / N * 0.01745f * i), -L / 2.0f, r * cosf(360.0f / N * 0.01745f * i));
			if (i % (N / 4) == 0)
			{
				p[L2 + 2 * i] = p[L0 + i];
				p[L2 + 2 * i + 1] = p[L1 + i];
			}
			p[L3 + i] = D3DXVECTOR3(r * cosf(180.0f / N * 0.01745f * i), r * sinf(180.0f / N * 0.01745f * i) + L / 2.0f, 0);
			p[L4 + i] = D3DXVECTOR3(0, r * sinf(180.0f / N * 0.01745f * i) + L / 2.0f, r * cosf(180.0f / N * 0.01745f * i));
			p[L5 + i] = D3DXVECTOR3(r * cosf(180.0f / N * 0.01745f * i), -r * sinf(180.0f / N * 0.01745f * i) - L / 2.0f, 0);
			p[L6 + i] = D3DXVECTOR3(0, -r * sinf(180.0f / N * 0.01745f * i) - L / 2.0f, r * cosf(180.0f / N * 0.01745f * i));
		}
		p[L0 + N] = p[L0];
		p[L1 + N] = p[L1];
		p[L3 + N] = D3DXVECTOR3(-r, L / 2.0f, 0);
		p[L4 + N] = D3DXVECTOR3(0, L / 2.0f, -r);
		p[L5 + N] = D3DXVECTOR3(-r, L / 2.0f, 0);
		p[L6 + N] = D3DXVECTOR3(0, L / 2.0f, -r);

		vb->Unlock();

		D3DXVECTOR3 axis;
		D3DXVec3Cross(&axis, &D3DXVECTOR3(0, 1, 0), &up);
		D3DXVec3Normalize(&axis, &axis);
		FLOAT angle = acosf(D3DXVec3Dot(&up, &D3DXVECTOR3(0, 1, 0)) / D3DXVec3Length(&up));
		D3DXMATRIX M;
		D3DXMatrixRotationAxis(&M, &axis, angle);
		D3DXVECTOR3 g = (p0 + p1) / 2;
		M._41 = g.x;
		M._42 = g.y;
		M._43 = g.z;

		D3DMATERIAL9 m = { 0 };
		m.Diffuse = m.Ambient = D3DXCOLOR(c);
		d3dd->SetMaterial(&m);

		d3dd->SetFVF(D3DFVF_XYZ);
		d3dd->SetStreamSource(0, vb, 0, sizeof(D3DXVECTOR3));
		d3dd->SetTransform(D3DTS_WORLD, &M);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L0, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L1, N);
		d3dd->DrawPrimitive(D3DPT_LINELIST, L2, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L3, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L4, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L5, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L6, N);
	}

	void	DrawCapsule( const Vector3& p1, const Vector3& p2, float r, DWORD color )
	{
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		DrawCapsule( iexSystem::GetDevice(), D3DXVECTOR3( p1.x, p1.y, p1.z ), D3DXVECTOR3( p2.x, p2.y, p2.z ), r, color );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	}

//-------------------------------------------------------------------------
//	相互変換DirectX<->IEX
//-------------------------------------------------------------------------

	void	QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ )
	{
		pOut->x = pQ->x;
		pOut->y = pQ->y;
		pOut->z = pQ->z;
		pOut->w = pQ->w;
	}

	void	QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ )
	{
		pOut->x = pQ->x;
		pOut->y = pQ->y;
		pOut->z = pQ->z;
		pOut->w = pQ->w;
	}

	void	Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV )
	{
		pOut->x = pV->x;
		pOut->y = pV->y;
		pOut->z = pV->z;
	}

	void	Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV )
	{
		pOut->x = pV->x;
		pOut->y = pV->y;
		pOut->z = pV->z;
	}