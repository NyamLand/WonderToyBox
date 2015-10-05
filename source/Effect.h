
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

private:
	iexMesh*	aura;
	bool		isAura;
	float		_adjustV;

	iex2DObj*	circle_pic[4];
	Circle		circle[4];
	Circle		circle_out[4];
	float		c_angle;


public:
	//	初期化・解放
	Effect( void );
	~Effect( void );

	void	Initialize();

	//	更新・描画
	void	Spin( void );
	void	Update( void );
	void	Render( void );

	//	情報更新
	void	PoligonSet( Circle* );
	void	CirclePosSet( Circle*, int );

	//	情報変換
	void	LVChange( LVERTEX& l,Vector3& p );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
