
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
	//	サークル
	typedef struct
	{
		LVERTEX		poligon[4];
		Vector3		c_pos;
	}Circle;

	struct EFF_VERTEX
	{
		LVERTEX	v[4];		//	ポリゴン
		float			angle;	//	向き
	};

	//	シールド本体
	struct SHIELD
	{
		EFF_VERTEX		v[4];			//	ポリゴン情報
		Vector3				pos[4];		//	ポリゴン座標
	};

	//	シールド用情報
	struct SHIELD_INFO
	{
		iex2DObj*	obj;
		SHIELD	shield[4];
		Vector3	pos[4];
		float	scale;
		float	r;
		bool	state[4];
	};
private:
	//	オーラ
	iexMesh*	aura;
	bool		isAura;
	//	竜巻
	iexMesh*	storm;
	bool		isStorm;
	Vector3		storm_pos;
	int			storm_time;
	float		_adjustV;

	iex2DObj*	circle_pic[4];
	Circle		circle[4];
	Circle		circle_out[4];
	float		c_angle;
	//	混乱
	Circle		confusion[4];
	Circle		confusion_out[4];
	bool		isConfusion[4];

	ImageObj	pow_up;
	Vector3		pow_pos;
	int			pow_time;

	ImageObj	itemEffect;
	Vector3		item_pos;
	Vector3		item_start;
	Vector3		item_finish;

	SHIELD_INFO	shieldInfo;
	
public:
	//	初期化・解放
	Effect( void );
	~Effect( void );
	void	Initialize();
	void	ShieldInitialize( void );

	//	更新・描画
	void	Spin( void );
	void	Update( void );
	void	AuraUpdate( void );
	void	StormSet( Vector3 pos, int time );
	void	StormTimer( void );
	void	ItemEffectUpdate( void );
	void	ItemEffectSet( int num, int state );
	void	Render( void );
	void	RenderShield( void );

	//	情報設定
	void	PoligonSet( Circle*, float, float, float );
	void	CirclePosSet( Circle*, int , Vector3 add = Vector3(0.0f, 0.0f, 0.0f) );
	void	SetShield( int player, bool state );

	//	動作関数
	void	Shield( void );

	//	情報変換
	void	LVChange( LVERTEX& l,Vector3& p );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
