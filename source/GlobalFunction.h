
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
	extern	bool					debug;

//----------------------------------------------------------------------
//	�萔
//----------------------------------------------------------------------

	//	�T�E���h�֘A
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

		//	�a�f�l�̃t�@�C�����͂�����
		//	��̒萔�ƕ��я��𑵂��邱��
		const		LPSTR	bgm[]	=
		{
			"",
			"DATA/SOUND/FightBGM.ogg",
			"",
		};
	}

	//	���ʃ��[�V�����ԍ�
	namespace MotionType
	{
		enum Motion
		{
			STAND,					//	����
			POSTURE,				//	�\��
			RUN,						//	����
			JUMP,					//	�W�����v
			LANDING,				//	���n
			ATTACK1,				//	�U���P�i�K��
			ATTACK2,				//	�U���Q�i�K��
			ATTACK3,				//	�U���R�i�K��
			GUARD,					//	�K�[�h
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

	//	���[�V�����ԍ��ۑ��p�\����
	struct MotionData
	{
		int		STAND;					//	����
		int		POSTURE;				//	�\��
		int		RUN;						//	����
		int		ATTACK1;				//	�U���P�i�K��
		int		JUMP;					//	�W�����v
		int		LANDING;				//	���n
		int		ATTACK2;				//	�U���Q�i�K��
		int		ATTACK3;				//	�U���R�i�K��
		int		GUARD;					//	�K�[�h
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

	//	�T�E���h�֘A
	void	SetSound( int type, bool loop = false );
	void	PlayBGM( int type );
	void	StopBGM( void );

	//	���[�V�����ԍ��o�^
	void	SetMotionNum( int& motionData, int motionNum );

//----------------------------------------------------------------------
//	�摜�֘A
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//	�}�`�`��
//----------------------------------------------------------------------
	void	DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawSphere( const Vector3& pos, float r, DWORD color = 0xFFFFFFFF );
	void	DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p1, CONST D3DXVECTOR3 &p2, FLOAT r, D3DCOLOR c = 0xFFFFFFFF );
	void	DrawCapsule( const Vector3& p1, const Vector3& p2, float r, DWORD color = 0xFFFFFFFF );

//----------------------------------------------------------------------
//	���`���( �o�́A�J�n�l�A�ŏI�l, ���� )
//----------------------------------------------------------------------

	//	Vector3
	bool	Lerp( Vector3& out, Vector3 p1, Vector3 p2, float t );

	//	float
	bool	Lerp( float& out, float p1, float p2, float t );

//----------------------------------------------------------------------
//	�R���֐����( �o�́A�J�n�l�A�ŏI�l, ���� )
//----------------------------------------------------------------------

	//	Vector3	
	bool	CubicFunctionInterpolation( Vector3& out, Vector3 p1, Vector3 p2, float t );

	//	float
	bool	CubicFunctionInterpolation( float& out, float p1, float p2, float t );

//----------------------------------------------------------------------
//	�x�W�F�Ȑ�
//----------------------------------------------------------------------

	//	�Q���x�W�F�Ȑ�
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 end, float t );

	//	�R���x�W�F�Ȑ�
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 p2, Vector3 end, float t );

	//	�n�_�I�_�̓�����
	enum ePrm_t {           // Prm1                   / Prm2
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

	//	�������x�W�F�Ȑ��p�����[�^�擾( 0.0f ~ 1.0f )
	float GetBezier( ePrm_t ePrm1, ePrm_t ePrm2, float fRate );

//----------------------------------------------------------------------
//	D3DX��IEX�̓���ւ�
//----------------------------------------------------------------------
	void QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ );
	void QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ );
	void Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV );
	void Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV );

//******************************************************************************
#endif // !__GROBALFUNCTION_H__
