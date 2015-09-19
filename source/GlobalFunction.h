
#ifndef	 __GLOBALFUNCTION_H__
#define	 __GLOBALFUNCTION_H__

//******************************************************************************
//
//	グローバル関数
//
//******************************************************************************

//----------------------------------------------------------------------
//	変数
//----------------------------------------------------------------------
	
	//	extern宣言
	extern	LPDSSTREAM		stream;
	extern	bool					debug;

//----------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------

	//	サウンド関連
	namespace SoundInfo
	{
		enum SOUNDEFFECT
		{
			NEWS_SE,
			HYPER_SE,
			POWER_SE,
			QUICK_SE,
			COIN_SE,
			DECIDE_SE,
			SELECT_SE,
			CANCEL_SE,
		};

		enum BGM
		{
			TITLE_BGM,
			MAIN_BGM,
			RESULT_BGM,
		};

		//	ＢＧＭのファイル名はここに
		//	上の定数と並び順を揃えること
		const		LPSTR	bgm[]	=
		{
			"",
			"DATA/SOUND/FightBGM.ogg",
			"",
		};
	}

	//	共通モーション番号
	namespace MotionType
	{
		enum Motion
		{
			STAND,					//	立ち
			POSTURE,				//	構え
			RUN,						//	走り
			JUMP,					//	ジャンプ
			LANDING,				//	着地
			ATTACK1,				//	攻撃１段階目
			ATTACK2,				//	攻撃２段階目
			ATTACK3,				//	攻撃３段階目
			GUARD,					//	ガード
		};
	}

//----------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------

	//	図形描画用構造体
	struct _VB
	{
		LPDIRECT3DVERTEXBUFFER9	p;
		_VB(LPDIRECT3DDEVICE9 d3dd, UINT length) : p(0){ d3dd->CreateVertexBuffer(length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL); }
		virtual	~_VB(void) { if (p)	p->Release(); }
		operator LPDIRECT3DVERTEXBUFFER9(void){ return p; }
		LPDIRECT3DVERTEXBUFFER9		operator -> (){ return p; }
	};

	//	モーション番号保存用構造体
	struct MotionData
	{
		int		STAND;					//	立ち
		int		POSTURE;				//	構え
		int		RUN;						//	走り
		int		ATTACK1;				//	攻撃１段階目
		int		JUMP;					//	ジャンプ
		int		LANDING;				//	着地
		int		ATTACK2;				//	攻撃２段階目
		int		ATTACK3;				//	攻撃３段階目
		int		GUARD;					//	ガード
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

//----------------------------------------------------------------------
//	システム
//----------------------------------------------------------------------

	//	解放
	void	SafeDelete( void* obj );

	//	変換
	POINT	GetPoint( int x, int y );
	DWORD	GetColor( float r, float g, float b, float a );
	DWORD	GetColor( Vector3 color );
	DWORD	GetColor( Vector3 color, float alpha );

	//	ワールド座標からクライエント座標への変換
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat );

	//	デバッグ文字描画
	void	DrawString( LPSTR string, int x, int y, DWORD color = 0xFFFFFFFF );
	void	DrawString( LPSTR string, int x, int y, float r, float g, float b );
	void	DrawString( LPSTR string, int x, int y, Vector3 color );

	//	サウンド関連
	void	SetSound( int type, bool loop = false );
	void	PlayBGM( int type );
	void	StopBGM( void );

	//	モーション番号登録
	void	SetMotionNum( int& motionData, int motionNum );

//----------------------------------------------------------------------
//	画像関連
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//	図形描画
//----------------------------------------------------------------------
	void	DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawSphere( const Vector3& pos, float r, DWORD color = 0xFFFFFFFF );
	void	DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p1, CONST D3DXVECTOR3 &p2, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawCapsule( const Vector3& p1, const Vector3& p2, float r, DWORD color = 0xFFFFFFFF );

//----------------------------------------------------------------------
//	線形補間( 出力、開始値、最終値, 割合 )
//----------------------------------------------------------------------

	//	Vector3
	bool	Lerp( Vector3& out, Vector3 p1, Vector3 p2, float t );

	//	float
	bool	Lerp( float& out, float p1, float p2, float t );

//----------------------------------------------------------------------
//	３次関数補間( 出力、開始値、最終値, 割合 )
//----------------------------------------------------------------------

	//	Vector3	
	bool	CubicFunctionInterpolation( Vector3& out, Vector3 p1, Vector3 p2, float t );

	//	float
	bool	CubicFunctionInterpolation( float& out, float p1, float p2, float t );

//----------------------------------------------------------------------
//	ベジェ曲線
//----------------------------------------------------------------------

	//	２次ベジェ曲線
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 end, float t );

	//	３次ベジェ曲線
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 p2, Vector3 end, float t );

	//	始点終点の動作種類
	enum ePrm_t {           // Prm1                   / Prm2
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

	//	高次元ベジェ曲線パラメータ取得( 0.0f ~ 1.0f )
	float GetBezier( ePrm_t ePrm1, ePrm_t ePrm2, float fRate );

//----------------------------------------------------------------------
//	D3DXとIEXの入れ替え
//----------------------------------------------------------------------
	void QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ );
	void QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ );
	void Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV );
	void Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV );

//******************************************************************************
#endif // !__GROBALFUNCTION_H__
