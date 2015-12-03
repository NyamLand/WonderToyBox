
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
	typedef struct
	{
		LVERTEX		poligon[4];
		Vector3		c_pos;
	}Circle;

	struct EFF_VERTEX
	{
		LVERTEX	v[4];		//	�|���S��
		float			angle;	//	����
	};

	//	�V�[���h�{��
	struct SHIELD
	{
		EFF_VERTEX		v[4];			//	�|���S�����
		Vector3				pos[4];		//	�|���S�����W
	};

	//	�V�[���h�p���
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
	//	�I�[��
	iexMesh*	aura;
	bool		isAura;
	//	����
	iexMesh*	storm;
	bool		isStorm;
	Vector3		storm_pos;
	int			storm_time;
	float		_adjustV;

	iex2DObj*	circle_pic[4];
	Circle		circle[4];
	Circle		circle_out[4];
	float		c_angle;

	ImageObj	pow_up;
	Vector3		pow_pos;
	int			pow_time;

	SHIELD_INFO	shieldInfo;
	
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
	void	Render( void );
	void	RenderShield( void );

	//	���ݒ�
	void	PoligonSet( Circle* );
	void	CirclePosSet( Circle*, int );
	void	SetShield( int player, bool state );

	//	����֐�
	void	Shield( void );

	//	���ϊ�
	void	LVChange( LVERTEX& l,Vector3& p );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
