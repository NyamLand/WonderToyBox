
#ifndef __EFFECT_H__
#define	__EFFECT_H__

//*******************************************************************************
//	
//	Effectクラス
//
//*******************************************************************************

class Effect
{
private:
	typedef struct
	{
		LVERTEX		poligon[4];
		Vector3		c_pos;
	}Circle;

	struct SHIELD
	{
		LVERTEX	v[4];			//	ポリゴン情報
		Vector3		pos[4];		//	ポリゴン座標
		float			r;				//	半径
	};

private:
	iexMesh*	aura;
	bool		isAura;
	float		_adjustV;

	iex2DObj*	circle_pic[4];
	Circle		circle[4];
	Circle		circle_out[4];
	float		c_angle;

	ImageObj	pow_up;
	Vector3		pow_pos;
	int			pow_time;

	SHIELD			shield;

public:
	//	初期化・解放
	Effect( void );
	~Effect( void );

	void	Initialize();
	void	ShieldInitialize( void );

	//	更新・描画
	void	Spin( void );
	void	Update( void );
	void	Render( void );

	//	情報更新
	void	PoligonSet( Circle* );
	void	CirclePosSet( Circle*, int );

	//	動作関数
	void	Shield( const Vector3& pos );

	//	情報変換
	void	LVChange( LVERTEX& l,Vector3& p );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
