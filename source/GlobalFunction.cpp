
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

//----------------------------------------------------------------------------
//	システム
//----------------------------------------------------------------------------

	//	解放
	void	SafeDelete( void* obj )
	{
		if ( obj != NULL )
		{
			delete	obj;
			obj = NULL;
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

	//	画像構造体初期化
	void	InitImage( Image& img, iex2DObj* obj, int x, int y, int w, int h, int sx, int sy, int sw, int sh, float angle, float alpha, Vector3 color )
	{
		img.obj = obj;
		img.x = x;		img.y = y;
		img.w = w;	img.h = h;
		img.sx = sx;	img.sy = sy;
		img.sw = sw; img.sh = sh;
		img.angle = angle;
		img.alpha = alpha;
		img.color = color;
	}
							 
	void	InitImage( Image& img, iex2DObj* obj, int x, int y, int w, int h, int srcScale )
	{
		img.obj = obj;
		img.x = x;		img.y = y;
		img.w = w;	img.h = h;
		img.sx = img.sy = 0;
		img.sw = img.sh = srcScale;
		img.angle = 0.0f;
		img.alpha = 1.0f;
		img.color = Vector3( 1.0f, 1.0f, 1.0f );
	}

	//	画像描画
	void	RenderImage( Image img )
	{
		int x, y, w, h, sx, sy, sw, sh;
		float angle, alpha;
		Vector3 color;

		w = img.w;	h = img.h;
		x = img.x - w / 2;
		y = img.y - h / 2;
		angle = img.angle;
		alpha = img.alpha;
		color= img.color;
		sx = img.sx; sy = img.sy;
		sw = img.sw; sh = img.sh;

		img.obj->Render( x, y, w, h, sx, sy, sw, sh, GetPoint( x, y ), angle, RS_COPY, GetColor( color, alpha ) );
	}

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

//----------------------------------------------------------------------------
//	３次関数補間( 出力、始点、終点、現在の割合( 0.0f ~ 1.0f ) )
//----------------------------------------------------------------------------

	//	Vector3
	bool	Lerp( Vector3& out, Vector3 p1, Vector3 p2, float t )
	{
		if ( t >= 1.0f )	return	true;
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3次関数補間値に変換

		out = p1 * ( 1.0f - rate ) + p2 * rate;   // いわゆるLerp
		return	false;
	}

	//	float
	bool	Lerp( float& out, float p1, float p2, float t )
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

