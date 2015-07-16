
#ifndef		__CAMERA_H__
#define		__CAMERA_H__

//****************************************************************************************
//
//	Cameraクラス
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
	
	//	振動用パラメータ
	Vector3 adjust;
	bool		shakeflag;
	int			shakeTimer;
	float		wide;

	D3DXQUATERNION	orientation;

public:
	//	初期化・解放
	Camera( void );
	~Camera( void );

	//	更新
	void	Update( VIEW_MODE mode, Vector3 target );

	//	動作関数
	void	ModeFix( Vector3 target );
	void	ModeSlerp( Vector3 target );
	void	Slerp( Vector3 target, float speed ) ;
	void	Shake( void );
	void	ShakeSet( float wide, int timer );

	//	情報取得
	Vector3	GetPos( void ){ return pos; }
	Vector3	GetTarget( void ){ return Target; }
	Matrix	GetMatrix( void ){ return matView; }
	
	//	情報設定
	void	SetPos( Vector3 pos ){ this->pos = pos; }
	void	SetPos( float posx, float posy, float posz ){ this->pos = Vector3( posx, posy, posz); }
};

extern	Camera*		m_Camera;

//****************************************************************************************
#endif // !__CAMERA_H__
