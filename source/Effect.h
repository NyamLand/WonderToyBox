
#ifndef __EFFECT_H__
#define	__EFFECT_H__

//*******************************************************************************
//	
//	Effect�N���X
//
//*******************************************************************************

class Effect
{
private:
	//	�T�[�N��
	typedef struct
	{
		LVERTEX		poligon[PLAYER_MAX];
		Vector3		c_pos;
	}Circle;

	struct Eff_Vertex
	{
		LVERTEX	v[PLAYER_MAX];		//	�|���S��
		float			angle;		//	����
	};

	//	�V�[���h�{��
	struct Shield
	{
		Eff_Vertex			v[PLAYER_MAX];		//	�|���S�����
		Vector3				pos[PLAYER_MAX];		//	�|���S�����W
	};

	//	��Ԉُ�
	struct StateEffect
	{
		ImageObj	image;
		Vector3		pos;
		Vector3		start;
		Vector3		finish;
	};

	//	�V�[���h�p���
	struct ShieldInfo
	{
		iex2DObj*	obj;
		Shield		shield[PLAYER_MAX];
		Vector3		pos[PLAYER_MAX];
		float		scale;
		float		r;
		bool		state[PLAYER_MAX];
	};

	//	�T�[�N�����
	struct CircleInfo
	{
		Circle		circle[PLAYER_MAX];
		Circle		circle_out[PLAYER_MAX];
		iex2DObj*	pic[PLAYER_MAX];
		float		speed;
	};

	//	�����p���
	struct ConfusionInfo
	{
		Circle		confusion[PLAYER_MAX];
		Circle		confusion_out[PLAYER_MAX];
		iex2DObj*	pic;
		float		speed;
		bool		state[PLAYER_MAX];
	};

	//	��Ԉُ�p���
	struct StateInfo
	{
		StateEffect		stateEffect[PLAYER_MAX];
		iex2DObj*		pic;
	};

	//	�I�[��
	struct Aura
	{
		iexMesh*	mesh;
		bool		state;
	};

	//	����
	struct Storm
	{
		iexMesh*	mesh;
		bool		state;
		Vector3		pos;
		int			time;
		float		_adjustV;
	};

	//	�A�C�e���摜�؂���ꏊ���
	struct StateImageInfo
	{
		int		sx;
		int		sy;
	};

private:
	//	�I�[��
	Aura		aura;

	//	����
	Storm		storm;

	//	�T�[�N��
	CircleInfo	circleInfo;

	//	����
	ConfusionInfo	confusionInfo;

	//	��Ԉُ�p
	StateInfo		stateInfo;

	//	��
	ShieldInfo		shieldInfo;

	//	�萔
	StateImageInfo	STATE_INFO[10];
	
public:
	//	�������E���
	Effect( void );
	~Effect( void );
	void	Initialize();
	void	ShieldInitialize( void );

	//	�X�V�E�`��
	void	Spin( void );
	void	Update( void );
	void	AuraUpdate( void );
	void	StormSet( Vector3 pos, int time );
	void	StormTimer( void );
	void	StateEffectUpdate( int num );
	void	StateEffectSet( int num, int state );
	void	Render( void );
	void	RenderShield( void );

	//	���ݒ�
	void	PoligonSet( Circle*, float, float, float );
	void	CirclePosSet( Circle*, int , Vector3 add = Vector3(0.0f, 0.0f, 0.0f) );
	void	SetShield( int player, bool state );
	void	SetConfusion( bool state, int num ){ confusionInfo.state[num] = state; }

	//	����֐�
	void	Shield( void );

	//	���ϊ�
	void	LVChange( LVERTEX& l,Vector3& p );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
