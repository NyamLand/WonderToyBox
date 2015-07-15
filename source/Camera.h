
#ifndef		__CAMERA_H__
#define		__CAMERA_H__

//****************************************************************************************
//
//	Camera�N���X
//
//****************************************************************************************
enum VIEW_MODE
{
	FIX,
	SLERP,
};

class Camera : public iexView
{
public:

private:
	Vector3 target;
	Vector3 pos;
	D3DXQUATERNION	orientation;

public:
	//	�������E���
	Camera( void );
	~Camera( void );

	//	�X�V
	void	Update( VIEW_MODE mode, Vector3 target );

	//	����֐�
	void	ModeFix( Vector3 target );
	void	ModeSlerp( Vector3 target );
	void	Slerp( Vector3 target, float speed ) ;

	//	���擾
	Vector3	GetPos( void ){ return pos; }
	Vector3	GetTarget( void ){ return Target; }
	Matrix	GetMatrix( void ){ return matView; }
	
	//	���ݒ�
	void	SetPos( Vector3 pos ){ this->pos = pos; }
	void	SetPos( float posx, float posy, float posz ){ this->pos = Vector3( posx, posy, posz); }
};

extern	Camera*		m_Camera;

//****************************************************************************************
#endif // !__CAMERA_H__
