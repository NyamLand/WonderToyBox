
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
	CHASE,		//	※このモードの場合はSetTargetでtargetに値を与える必要があります。
};

class Camera : public iexView
{
private:
	//	定数
	static	const		int		MAX = 20;		//	カメラからターゲットの距離の最大
	static	const		int		MIN = 8;		//								最少

private:
	Vector3	playerPos[4];
	Vector3 target;
	Vector3 pos;
	float length;
	
	//	振動用パラメータ
	Vector3 adjust;
	bool		shakeflag;
	int			shakeTimer;
	float		wide;

	//	球面線形補間用
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
	void	ModeChase( void );
	void	Slerp( Vector3 target, float speed ) ;
	void	Shake( void );
	void	ShakeSet( float wide, int timer );

	//	情報取得
	Vector3	GetPos( void ){ return pos; }
	Vector3	GetTarget( void ){ return Target; }
	Matrix	GetMatrix( void ){ return matView; }
	
	//	数値計算
	void	CalcCameraPos( void );
	Vector3	CalcCenterPos( void );
	float	CalcMaxDist( void );
	
	//	情報設定
	void	SetPos( Vector3 pos ){ this->pos = pos; }
	void	SetPos( float posx, float posy, float posz ){ this->pos = Vector3( posx, posy, posz); }
	void	SetPlayerInfo( const Vector3& p_1, const Vector3& p_2, const Vector3& p_3, const Vector3& p_4 );
};

extern	Camera*		m_Camera;

//****************************************************************************************
#endif // !__CAMERA_H__
