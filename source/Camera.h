
#ifndef		__CAMERA_H__
#define		__CAMERA_H__

//****************************************************************************************
//
//	Cameraクラス
//
//****************************************************************************************
class Camera : public iexView
{
private:

public:
	//	初期化・解放
	Camera( void );
	~Camera( void );

	//	更新
	void	Update( void );

	//	情報取得
	Vector3	GetPos( void ){ return Pos; }
	Vector3	GetTarget( void ){ return Target; }
};

extern	Camera*		m_Camera;

//****************************************************************************************
#endif // !__CAMERA_H__
