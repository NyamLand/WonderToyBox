
#ifndef	 __GLOBALFUNCTION_H__
#define	 __GLOBALFUNCTION_H__

//******************************************************************************
//
//	�O���[�o���֐�
//
//******************************************************************************

//----------------------------------------------------------------------
//	�萔
//----------------------------------------------------------------------

	#define	ZERO_VECTOR3		Vector3( 0.0f, 0.0f, 0.0f )
	#define	FOR( minValue, maxValue )	for( int value = minValue; value < maxValue; value++ )
	#define	FOR_LIST( minValue, maxValue )	for( auto it = minValue; it != maxValue; it++ )

	namespace IMAGE_MODE
	{
		enum
		{
			NORMAL,			//	�I���W�i���`��
			ADOPTPARAM,	//	�p�����[�^�̗p
			ADD,			//	���Z����
			WAVE,				//	�g��
			FLASH,				//	�_��
			SCALING,			//	�g��k��
			END,					//	�I�[
		};
	}

//----------------------------------------------------------------------
//	�ϐ�
//----------------------------------------------------------------------
	
	extern	bool					debug;

//----------------------------------------------------------------------
//	�\����
//----------------------------------------------------------------------

	//	�}�`�`��p�\����
	struct _VB
	{
		LPDIRECT3DVERTEXBUFFER9	p;
		_VB( LPDIRECT3DDEVICE9 d3dd, UINT length ) : p( 0 ){ d3dd->CreateVertexBuffer( length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL ); }
		virtual	~_VB(void) { if ( p )	p->Release(); }
		operator LPDIRECT3DVERTEXBUFFER9( void ){ return p; }
		LPDIRECT3DVERTEXBUFFER9		operator -> (){ return p; }
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
		Vector3	color;

		//	wave�Escaling���ʗp�p�����[�^
		int	 plusScaleX;
		int	 plusScaleY;

		//	wave�p�p�����[�^
		float	wavespeed;
		float	waveAlpha;
		bool	waveState;
		bool	waverenderflag;

		//	flashing�p�p�����[�^
		float	flashingSpeed;
		float	flashingAlpha;
		float	flashingRenderflag;
		float	flashingParam;

		//	scaling�p�p�����[�^
		float	scalingspeed;
		float	scalingAlpha;
		bool	scalingAlphaFlag;
		bool	scalingState;
		bool	scalingFlag;
		bool	scalingrenderflag;
	};

//----------------------------------------------------------------------
//	�V�X�e��
//----------------------------------------------------------------------

	//	�ǂݍ��� �|�C���^�[��null����������new����
	//	�������Ă�����true��������
	template<typename T>
	bool	PointerNew( T*& ptr, LPSTR filename = nullptr )
	{
		ptr = nullptr;
		if ( filename != nullptr )	ptr = new T( filename );
		else ptr = new T();

		if ( ptr != nullptr )	return	true;
		return	false;
	}

	//	��� �|�C���^�[��delete����nullptr��������
	template<typename T>
	void	SafeDelete( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete	ptr;
			ptr = nullptr;
		}
	}

	//	�s���� �s��̃|�C���^�[��delete[]����nullptr��������
	template<typename T>
	void	SafeDeleteArray( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	//	�f�o�b�O�����`��
	void	DrawString( LPSTR string, int x, int y, DWORD color = 0xFFFFFFFF );
	void	DrawString( LPSTR string, int x, int y, float r, float g, float b );
	void	DrawString( LPSTR string, int x, int y, Vector3 color );

//----------------------------------------------------------------------
//	�ϊ�
//----------------------------------------------------------------------

	//	int�^���l��POINT�ɕϊ����ϊ����POINT��������
	POINT	GetPoint( int x, int y );

	//	�e�F,�����x��ݒ肵�ϊ����DWORD��������
	DWORD	GetColor( float r, float g, float b, float a );
	DWORD	GetColor( Vector3 color );
	DWORD	GetColor( Vector3 color, float alpha );

	//	���[���h���W����N���C�G���g���W�ւ̕ϊ�
	//	out��pos�̃X�N���[�����W��������
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat );

	//	�N���C�A���g���W���烏�[���h���W�ւ̕ϊ�
	//	out�ɃX�N���[����̃��[���h���W��������
	void	ClientToWorld( const Vector3& screenPos, Vector3& out );

//----------------------------------------------------------------------
//	�摜����
//----------------------------------------------------------------------

	//	�摜�������E�`��
	void	ImageInitialize( ImageObj& image, int x, int y, int w, int h, int sx, int sy, int sw, int sh );
	void	RenderImage( ImageObj image, int sx, int sy, int sw, int sh, int mode );
	void	RenderImage( ImageObj image, int sx, int sy, int sw, int sh, int mode, int x, int y );

	//	�g��̐ݒ�(�g��o�������I�u�W�F�N�g�A�g��X�s�[�h)
	void	SetWave( ImageObj& image, float speed );

	//	�g��X�V�i�g��o�������I�u�W�F�N�g�A�g��傫���A�X�^�[�g�������x�j�g��I����true��������
	bool	WaveUpdate( ImageObj& image, int max_scale = 140, float max_alpha = 1.0f );

	//	�_�ōX�V�i�_�ł��������I�u�W�F�N�g�A�_�ŃX�s�[�h�j
	void	FlashingUpdate( ImageObj& image, float speed = -1.0f );

	//	�g��
	void	SetScaling( ImageObj& image, float speed, bool state = true );					//	state:	true = �g��@false = �k��
	void	ScalingUpdate(ImageObj& image, int max_scale = 140);
	void	ScalingUpdate2(ImageObj& image, int max_scale = 140);
	bool	ScalingAlphaUpdate(ImageObj& image, int max_scale = 140);
	void	ScalingLandingUpdate(ImageObj& image, int max_scale = 140);
	void	StopScaling( ImageObj& image );
	
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
//	���`���( �o�́A�J�n�l�A�ŏI�l, ���� )�@�I����true��������
//----------------------------------------------------------------------

	template<typename T, typename T2>
	bool	Lerp( T& out, T2 p1, T2 p2, float t )
	{
		out = ( T )( p1 * ( 1 - t ) + p2 * t );

		if ( t >= 1.0f )	return	true;
		return	false;
	}

//----------------------------------------------------------------------
//	�R���֐����( �o�́A�J�n�l�A�ŏI�l, ���� )�@�I����true��������
//----------------------------------------------------------------------

	template<typename T, typename T2>
	bool	CubicFunctionInterpolation( T& out, T2 p1, T2 p2, float t )
	{
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3���֐���Ԓl�ɕϊ�
		out = ( T )( p1 * ( 1.0f - rate ) + p2 * rate );

		if ( t >= 1.0f )	return	true;
		return	false;
	}

//----------------------------------------------------------------------
//	�x�W�F�Ȑ��i�o�́A�X�^�[�g�ʒu�A����_�A�I���ʒu�A���߂��������j �I����true��������
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
//	��Ԋ֐��p�p�����[�^�X�V
//----------------------------------------------------------------------

	//	�����X�V( �X�V�������p�����[�^, ���Z�l ) : �X�V�������p�����[�^��1.0f�ȏ�̏ꍇtrue��������
	bool	PercentageUpdate( float& param, float addParam );

//----------------------------------------------------------------------
//	������(�o�́A�J�n�_�A�ڕW�n�_�A�����A�d��)
//----------------------------------------------------------------------
	bool Parabola(Vector3& out, Vector3 start, Vector3 end, float speed, float mass);

//----------------------------------------------------------------------
//	D3DX��IEX�̓���ւ�
//----------------------------------------------------------------------
	void QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ );
	void QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ );
	void Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV );
	void Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV );

//----------------------------------------------------------------------
//	���Z
//----------------------------------------------------------------------
	inline void Increment(int& out){ out++; }
	bool isMiddle(const int& value, const int& min, const int& max);
	bool isMiddle(const float& value, const float& min, const float& max);

//******************************************************************************
#endif // !__GROBALFUNCTION_H__
