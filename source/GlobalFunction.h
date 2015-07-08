
#ifndef	 __GLOBALFUNCTION_H__
#define	 __GLOBALFUNCTION_H__

//******************************************************************************
//
//	�O���[�o���֐�
//
//******************************************************************************

//----------------------------------------------------------------------
//	�ϐ�
//----------------------------------------------------------------------
	
	//	extern�錾
	extern	LPDSSTREAM		stream;

//----------------------------------------------------------------------
//	�萔
//----------------------------------------------------------------------

	//	�T�E���h�֘A
	namespace SoundInfo
	{
		enum SOUNDEFFECT
		{
			SELECT_SE,
			CANCEL_SE,
		};

		enum BGM
		{
			TITLE_BGM,
			MAIN_BGM,
			RESULT_BGM,
		};

		//	�a�f�l�̃t�@�C�����͂�����
		//	��̒萔�ƕ��я��𑵂��邱��
		const		LPSTR	bgm[]	=
		{
			"",
			"",
			"",
		};
	}

//----------------------------------------------------------------------
//	�\����
//----------------------------------------------------------------------

	//	�摜�\����
	struct Image
	{
		iex2DObj*	obj;
		int		x, y, w, h, sx, sy, sw, sh;
		float	alpha;
		float	angle;
		Vector3	color;
	};

	//	�}�`�`��p�\����
	struct _VB
	{
		LPDIRECT3DVERTEXBUFFER9	p;
		_VB(LPDIRECT3DDEVICE9 d3dd, UINT length) : p(0){ d3dd->CreateVertexBuffer(length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL); }
		virtual	~_VB(void) { if (p)	p->Release(); }
		operator LPDIRECT3DVERTEXBUFFER9(void){ return p; }
		LPDIRECT3DVERTEXBUFFER9		operator -> (){ return p; }
	};

//----------------------------------------------------------------------
//	�V�X�e��
//----------------------------------------------------------------------

	//	���
	void	SafeDelete( void* obj );

	//	�ϊ�
	POINT	GetPoint( int x, int y );
	DWORD	GetColor( float r, float g, float b, float a );
	DWORD	GetColor( Vector3 color );
	DWORD	GetColor( Vector3 color, float alpha );

	//	���[���h���W����N���C�G���g���W�ւ̕ϊ�
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat );

	//	�f�o�b�O�����`��
	void	DrawString( LPSTR string, int x, int y, DWORD color = 0xFFFFFFFF );
	void	DrawString( LPSTR string, int x, int y, float r, float g, float b );
	void	DrawString( LPSTR string, int x, int y, Vector3 color );

	//	�摜�\���̏�����
	void	InitImage( Image& img, iex2DObj* obj, int x, int y, int w, int h, int sx = 0, int sy = 0, int sw = 512, int sh = 512, float angle = 0.0f, float alpha = 1.0f, Vector3 color = Vector3( 1.0f, 1.0f, 1.0f ) );
	void	InitImage( Image& img, iex2DObj* obj, int x, int y, int w, int h, int srcScale );

	//	�摜�`��
	void	RenderImage( Image img );

	//	�T�E���h�֘A
	void	SetSound( int type, bool loop = false );
	void	PlayBGM( int type );
	void	StopBGM( void );
	
//----------------------------------------------------------------------
//	�}�`�`��
//----------------------------------------------------------------------
	void	DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawSphere( const Vector3& pos, float r, DWORD color = 0xFFFFFFFF );
	void	DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p1, CONST D3DXVECTOR3 &p2, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawCapsule( const Vector3& p1, const Vector3& p2, float r, DWORD color = 0xFFFFFFFF );

//----------------------------------------------------------------------
//	�R���֐����( �o�́A�J�n�l�A�ŏI�l, ���� )
//----------------------------------------------------------------------

	//	Vector3	
	bool	Lerp( Vector3& out, Vector3 p1, Vector3 p2, float t );

	//	float
	bool	Lerp( float& out, float p1, float p2, float t );

//----------------------------------------------------------------------
//	�x�W�F�Ȑ�
//----------------------------------------------------------------------

	//	�Q���x�W�F�Ȑ�
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 end, float t );

	//	�R���x�W�F�Ȑ�
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 p2, Vector3 end, float t );

//******************************************************************************
#endif // !__GROBALFUNCTION_H__
