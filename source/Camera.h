
#ifndef		__CAMERA_H__
#define		__CAMERA_H__

//****************************************************************************************
//
//	Camera�N���X
//
//****************************************************************************************
class Camera : public iexView
{
private:

public:
	//	�������E���
	Camera( void );
	~Camera( void );

	//	�X�V
	void	Update( void );

	//	���擾
	Vector3	GetPos( void ){ return Pos; }
	Vector3	GetTarget( void ){ return Target; }
};

extern	Camera*		m_Camera;

//****************************************************************************************
#endif // !__CAMERA_H__
