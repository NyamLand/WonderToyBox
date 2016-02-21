
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
		LVERTEX		poligon[PLAYER_MAX];
		Vector3		c_pos;
	}Circle;

	struct Eff_Vertex
	{
		LVERTEX	v[PLAYER_MAX];		//	ポリゴン
		float			angle;		//	向き
	};

	//	シールド本体
	struct Shield
	{
		Eff_Vertex			v[PLAYER_MAX];		//	ポリゴン情報
		Vector3				pos[PLAYER_MAX];		//	ポリゴン座標
	};

	//	状態異常
	struct StateEffect
	{
		ImageObj	image;
		Vector3		pos;
		Vector3		start;
		Vector3		finish;
	};

	//	シールド用情報
	struct ShieldInfo
	{
		iex2DObj*	obj;
		Shield		shield[PLAYER_MAX];
		Vector3		pos[PLAYER_MAX];
		float		scale;
		float		r;
		bool		state[PLAYER_MAX];
	};

	//	サークル情報
	struct CircleInfo
	{
		Circle		circle[PLAYER_MAX];
		Circle		circle_out[PLAYER_MAX];
		iex2DObj*	pic[PLAYER_MAX];
		float		speed;
	};

	//	混乱用情報
	struct ConfusionInfo
	{
		Circle		confusion[PLAYER_MAX];
		Circle		confusion_out[PLAYER_MAX];
		iex2DObj*	pic;
		float		speed;
		bool		state[PLAYER_MAX];
	};

	//	状態異常用情報
	struct StateInfo
	{
		StateEffect		stateEffect[PLAYER_MAX];
		iex2DObj*		pic;
	};

	//	オーラ
	struct Aura
	{
		iexMesh*	mesh;
		bool		state;
	};

	//	竜巻
	struct Storm
	{
		iexMesh*	mesh;
		bool		state;
		Vector3		pos;
		int			time;
		float		_adjustV;
	};

	//	アイテム画像切り取り場所情報
	struct StateImageInfo
	{
		int		sx;
		int		sy;
	};

private:
	//	オーラ
	Aura		aura;

	//	竜巻
	Storm		storm;

	//	サークル
	CircleInfo	circleInfo;

	//	混乱
	ConfusionInfo	confusionInfo;

	//	状態異常用
	StateInfo		stateInfo;

	//	盾
	ShieldInfo		shieldInfo;

	//	定数
	StateImageInfo	STATE_INFO[10];
	
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
	void	StateEffectUpdate( int num );
	void	StateEffectSet( int num, int state );
	void	Render( void );
	void	RenderShield( void );

	//	情報設定
	void	PoligonSet( Circle*, float, float, float );
	void	CirclePosSet( Circle*, int , Vector3 add = Vector3(0.0f, 0.0f, 0.0f) );
	void	SetShield( int player, bool state );
	void	SetConfusion( bool state, int num ){ confusionInfo.state[num] = state; }

	//	動作関数
	void	Shield( void );

	//	情報変換
	void	LVChange( LVERTEX& l,Vector3& p );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
