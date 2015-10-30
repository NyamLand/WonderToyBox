
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
	
	extern	bool					debug;

//----------------------------------------------------------------------
//	�萔
//----------------------------------------------------------------------

	namespace IMAGE_MODE
	{
		enum
		{
			NORMAL,			//	�I���W�i���`��
			ADOPTPARAM,	//	�p�����[�^�̗p
			WAVE,				//	�g��
			FLASH,				//	�_��
			END,					//	�I�[
		};
	}

//----------------------------------------------------------------------
//	�\����
//----------------------------------------------------------------------

	//	�}�`�`��p�\����
	struct _VB
	{
		LPDIRECT3DVERTEXBUFFER9	p;
		_VB(LPDIRECT3DDEVICE9 d3dd, UINT length) : p(0){ d3dd->CreateVertexBuffer(length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL); }
		virtual	~_VB(void) { if (p)	p->Release(); }
		operator LPDIRECT3DVERTEXBUFFER9(void){ return p; }
		LPDIRECT3DVERTEXBUFFER9		operator -> (){ return p; }
	};

	//	�j���[�X�\����
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

	//	�摜�\����
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

		//	wave�p�p�����[�^
		int	 plusScaleX;
		int	 plusScaleY;
		float	wavespeed;
		float	waveAlpha;
		bool	waveState;
		bool	waverenderflag;

		//	flashing�p�p�����[�^
		float	flashingSpeed;
		float	flashingAlpha;
		float	flashingRenderflag;
		float	flashingParam;

		//void	Render(int )
	};

//----------------------------------------------------------------------
//	�V�X�e��
//----------------------------------------------------------------------

	//	���
	template<typename T>
	void	SafeDelete( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete	ptr;
			ptr = nullptr;
		}
	}

	//	�s����
	template<typename T>
	void	SafeDeleteArray( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

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

	//	�摜����
	void	ImageInitialize( ImageObj& image, int x, int y, int w, int h, int sx, int sy, int sw, int sh );
	void	RenderImage( ImageObj image, int sx, int sy, int sw, int sh, int mode );
	void	RenderImage( ImageObj image, int sx, int sy, int sw, int sh, int mode, int x, int y );

	void	SetWave( ImageObj& image, float speed );
	void	WaveUpdate( ImageObj& image, int max_scale = 140, float max_alpha = 1.0f );
	void	FlashingUpdate( ImageObj& image, float speed = -1.0f );

//----------------------------------------------------------------------
//	�}�`�`��
//----------------------------------------------------------------------
	void	DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawSphere( const Vector3& pos, float r, DWORD color = 0xFFFFFFFF );
	void	DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p1, CONST D3DXVECTOR3 &p2, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawCapsule( const Vector3& p1, const Vector3& p2, float r, DWORD color = 0xFFFFFFFF );

//----------------------------------------------------------------------
//	�}�`�ݒ�
//----------------------------------------------------------------------

	//	���_�ݒ�
	void	SetVertex( LVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color );

	//	���_�ݒ�
	void	SetVertex( TLVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color );

//----------------------------------------------------------------------
//	���`���( �o�́A�J�n�l�A�ŏI�l, ���� )
//----------------------------------------------------------------------

	template<typename T, typename T2>
	bool	Lerp( T& out, T2 p1, T2 p2, float t )
	{
		if ( t >= 1.0f )	return	true;

		out = ( T )( p1 * ( 1 - t ) + p2 * t );

		return	false;
	}

//----------------------------------------------------------------------
//	�R���֐����( �o�́A�J�n�l�A�ŏI�l, ���� )
//----------------------------------------------------------------------

	template<typename T, typename T2>
	bool	CubicFunctionInterpolation( T& out, T2 p1, T2 p2, float t )
	{
		if ( t >= 1.0f )	return	true;
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3���֐���Ԓl�ɕϊ�

		out = ( T )( p1 * ( 1.0f - rate ) + p2 * rate );
		return	false;
	}

//----------------------------------------------------------------------
//	�x�W�F�Ȑ�
//----------------------------------------------------------------------

	//	�Q���x�W�F�Ȑ�
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 end, float t );

	//	�R���x�W�F�Ȑ�
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 p2, Vector3 end, float t );

	//	�n�_�I�_�̓�����
	namespace ePrm_t
	{
		enum {           // Prm1                   / Prm2
			eSlow_Lv5,      // ���@������蓮���n�߂� / ������蓮���I����
			eSlow_Lv4,      // ���@������蓮���n�߂� / ������蓮���I����
			eSlow_Lv3,      // �@�@������蓮���n�߂� / ������蓮���I����
			eSlow_Lv2,      // ���@������蓮���n�߂� / ������蓮���I����
			eSlow_Lv1,      // ��@������蓮���n�߂� / ������蓮���I����
			eNoAccel,       // �@�@�����I�ȓ���������
			eRapid_Lv1,     // ��@�}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv2,     // ���@�}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv3,     // �@�@�}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv4,     // ���@�}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv5,     // ���@�}�ɓ����n�߂�      / �}�ɓ����I����
		};
	}

	//	�������x�W�F�Ȑ��p�����[�^�擾( 0.0f ~ 1.0f )
	float GetBezier( int ePrm1, int ePrm2, float fRate );

//----------------------------------------------------------------------
//	D3DX��IEX�̓���ւ�
//----------------------------------------------------------------------
	void QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ );
	void QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ );
	void Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV );
	void Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV );

//******************************************************************************
#endif // !__GROBALFUNCTION_H__
