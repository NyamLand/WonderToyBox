
#ifndef	 __GLOBALFUNCTION_H__
#define	 __GLOBALFUNCTION_H__

//******************************************************************************
//
//	グローバル関数
//
//******************************************************************************

//----------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------

	#define	ZERO_VECTOR3		Vector3( 0.0f, 0.0f, 0.0f )
	#define	FOR( minValue, maxValue )	for( int value = minValue; value < maxValue; value++ )
	#define	FOR_LIST( minValue, maxValue )	for( auto it = minValue; it != maxValue; it++ )

	namespace IMAGE_MODE
	{
		enum
		{
			NORMAL,			//	オリジナル描画
			ADOPTPARAM,	//	パラメータ採用
			ADD,			//	加算合成
			WAVE,				//	波紋
			FLASH,				//	点滅
			SCALING,			//	拡大縮小
			END,					//	終端
		};
	}

//----------------------------------------------------------------------
//	変数
//----------------------------------------------------------------------
	
	extern	bool					debug;

//----------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------

	//	図形描画用構造体
	struct _VB
	{
		LPDIRECT3DVERTEXBUFFER9	p;
		_VB( LPDIRECT3DDEVICE9 d3dd, UINT length ) : p( 0 ){ d3dd->CreateVertexBuffer( length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL ); }
		virtual	~_VB(void) { if ( p )	p->Release(); }
		operator LPDIRECT3DVERTEXBUFFER9( void ){ return p; }
		LPDIRECT3DVERTEXBUFFER9		operator -> (){ return p; }
	};

	//	ニュース構造体
	struct NewsBar
	{
		int		left;
		int		top;
		int		right;
		int		bottom;
		float	alpha;
		Vector3	color;
		LPSTR	text;
		int		textleft;
		int		step;
		bool	renderflag;
	};

	//	画像構造体
	struct ImageObj
	{
		iex2DObj*	obj;
		int	 x, y, w, h;
		int	 sx, sy, sw, sh;
		float	t;
		float	alpha;
		float	angle;
		POINT	p;
		bool	renderflag;
		Vector3	color;

		//	wave・scaling共通用パラメータ
		int	 plusScaleX;
		int	 plusScaleY;

		//	wave用パラメータ
		float	wavespeed;
		float	waveAlpha;
		bool	waveState;
		bool	waverenderflag;

		//	flashing用パラメータ
		float	flashingSpeed;
		float	flashingAlpha;
		float	flashingRenderflag;
		float	flashingParam;

		//	scaling用パラメータ
		float	scalingspeed;
		float	scalingAlpha;
		bool	scalingAlphaFlag;
		bool	scalingState;
		bool	scalingFlag;
		bool	scalingrenderflag;
	};

//----------------------------------------------------------------------
//	システム
//----------------------------------------------------------------------

	//	解放
	template<typename T>
	void	SafeDelete( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete	ptr;
			ptr = nullptr;
		}
	}

	//	行列解放
	template<typename T>
	void	SafeDeleteArray( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	//	変換
	POINT	GetPoint( int x, int y );
	DWORD	GetColor( float r, float g, float b, float a );
	DWORD	GetColor( Vector3 color );
	DWORD	GetColor( Vector3 color, float alpha );

	//	ワールド座標からクライエント座標への変換
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat );

	//	クライアント座標からワールド座標への変換
	void	ClientToWorld( const Vector3& screenPos, Vector3& out );

	//	デバッグ文字描画
	void	DrawString( LPSTR string, int x, int y, DWORD color = 0xFFFFFFFF );
	void	DrawString( LPSTR string, int x, int y, float r, float g, float b );
	void	DrawString( LPSTR string, int x, int y, Vector3 color );

	//	画像操作
	void	ImageInitialize( ImageObj& image, int x, int y, int w, int h, int sx, int sy, int sw, int sh );
	void	RenderImage( ImageObj image, int sx, int sy, int sw, int sh, int mode );
	void	RenderImage( ImageObj image, int sx, int sy, int sw, int sh, int mode, int x, int y );

	void	SetWave( ImageObj& image, float speed );
	bool	WaveUpdate( ImageObj& image, int max_scale = 140, float max_alpha = 1.0f );
	void	FlashingUpdate( ImageObj& image, float speed = -1.0f );
	void	SetScaling( ImageObj& image, float speed, bool state = true );					//	state:	true = 拡大　false = 縮小
	void	ScalingUpdate(ImageObj& image, int max_scale = 140);
	bool	ScalingAlphaUpdate(ImageObj& image, int max_scale = 140);
	void	ScalingLandingUpdate(ImageObj& image, int max_scale = 140);
	void	StopScaling( ImageObj& image );
	
//----------------------------------------------------------------------
//	図形描画
//----------------------------------------------------------------------
	void	DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawSphere( const Vector3& pos, float r, DWORD color = 0xFFFFFFFF );
	void	DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p1, CONST D3DXVECTOR3 &p2, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawCapsule( const Vector3& p1, const Vector3& p2, float r, DWORD color = 0xFFFFFFFF );

//----------------------------------------------------------------------
//	図形設定
//----------------------------------------------------------------------

	//	頂点設定
	void	SetVertex( LVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color );

	//	頂点設定
	void	SetVertex( TLVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color );

//----------------------------------------------------------------------
//	線形補間( 出力、開始値、最終値, 割合 )
//----------------------------------------------------------------------

	template<typename T, typename T2>
	bool	Lerp( T& out, T2 p1, T2 p2, float t )
	{
		out = ( T )( p1 * ( 1 - t ) + p2 * t );

		if ( t >= 1.0f )	return	true;
		return	false;
	}

//----------------------------------------------------------------------
//	３次関数補間( 出力、開始値、最終値, 割合 )
//----------------------------------------------------------------------

	template<typename T, typename T2>
	bool	CubicFunctionInterpolation( T& out, T2 p1, T2 p2, float t )
	{
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3次関数補間値に変換
		out = ( T )( p1 * ( 1.0f - rate ) + p2 * rate );

		if ( t >= 1.0f )	return	true;
		return	false;
	}

//----------------------------------------------------------------------
//	ベジェ曲線
//----------------------------------------------------------------------

	//	２次ベジェ曲線
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 end, float t );

	//	３次ベジェ曲線
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 p2, Vector3 end, float t );

	//	始点終点の動作種類
	namespace ePrm_t
	{
		enum {           // Prm1                   / Prm2
			eSlow_Lv5,      // 強　ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv4,      // ↑　ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv3,      // 　　ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv2,      // ↓　ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv1,      // 弱　ゆっくり動き始める / ゆっくり動き終える
			eNoAccel,       // 　　直線的な動きをする
			eRapid_Lv1,     // 弱　急に動き始める      / 急に動き終える
			eRapid_Lv2,     // ↑　急に動き始める      / 急に動き終える
			eRapid_Lv3,     // 　　急に動き始める      / 急に動き終える
			eRapid_Lv4,     // ↓　急に動き始める      / 急に動き終える
			eRapid_Lv5,     // 強　急に動き始める      / 急に動き終える
		};
	}

	//	高次元ベジェ曲線パラメータ取得( 0.0f ~ 1.0f )
	float GetBezier( int ePrm1, int ePrm2, float fRate );


//----------------------------------------------------------------------
//	放物線(出力、開始点、目標地点、初速、重力)
//----------------------------------------------------------------------
	bool Parabola(Vector3& out, Vector3 start, Vector3 end, float speed, float mass);
//----------------------------------------------------------------------
//	D3DXとIEXの入れ替え
//----------------------------------------------------------------------
	void QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ );
	void QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ );
	void Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV );
	void Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV );

//----------------------------------------------------------------------
//	演算
//----------------------------------------------------------------------
	inline void Increment(int& out){ out++; }
	bool isMiddle(const int& value, const int& min, const int& max);
	bool isMiddle(const float& value, const float& min, const float& max);

//******************************************************************************
#endif // !__GROBALFUNCTION_H__
