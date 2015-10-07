
#ifndef		__CAMERA_H__
#define		__CAMERA_H__

//****************************************************************************************
//
//	Camera�N���X
//
//****************************************************************************************

namespace
{
	namespace VIEW_MODE
	{
		enum
		{
			FIX,
			SLERP,
			CHASE,		//	�����̃��[�h�̏ꍇ��SetTarget��target�ɒl��^����K�v������܂��B
			TITLE,
		};
	}
}

class Camera : public iexView
{
private:
	//	�萔
	static	const		int		MAX = 20;		//	�J��������^�[�Q�b�g�̋����̍ő�
	static	const		int		MIN = 8;		//								�ŏ�

private:
	Vector3	playerPos[4];
	Vector3 target;
	Vector3 pos;
	float length;
	
	//	�U���p�p�����[�^
	Vector3 adjust;
	bool		shakeflag;
	int			shakeTimer;
	float		wide;

	//	���ʐ��`��ԗp
	D3DXQUATERNION	orientation;

public:
	//	�������E���
	Camera( void );
	~Camera( void );

	//	�X�V�E�`��
	void	Update( int viewmode, Vector3 target );
	void	Render( void );

	//	����֐�
	void	ModeFix( Vector3 target );
	void	ModeSlerp( Vector3 target );
	void	ModeChase( void );
	void	ModeTitle( Vector3	 target );
	void	Slerp( Vector3 target, float speed ) ;
	void	Shake( void );
	void	ShakeSet( float wide, int timer );

	//	���擾
	Vector3	GetPos( void ){ return pos; }
	Vector3	GetTarget( void ){ return target; }
	Matrix	GetMatrix( void ){ return matView; }
	
	//	���l�v�Z
	void	CalcCameraPos( void );
	Vector3	CalcCenterPos( void );
	float	CalcMaxDist( void );
	
	//	���ݒ�
	void	SetPos( Vector3 pos ){ this->pos = pos; }
	void	SetPos( float posx, float posy, float posz ){ this->pos = Vector3( posx, posy, posz); }
	void	SetPlayerInfo( const Vector3& p_1, const Vector3& p_2, const Vector3& p_3, const Vector3& p_4 );
};

extern	Camera*		m_Camera;

//****************************************************************************************
#endif // !__CAMERA_H__
