
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

private:
	iexMesh*	aura;
	bool		isAura;
	float		_adjustV;

	iex2DObj*	circle_pic[4];
	Circle		circle[4];
	Circle		circle_out[4];
	float		c_angle;


public:
	//	�������E���
	Effect( void );
	~Effect( void );

	void	Initialize();

	//	�X�V�E�`��
	void	Spin( void );
	void	Update( void );
	void	Render( void );

	//	���X�V
	void	PoligonSet( Circle* );
	void	CirclePosSet( Circle*, int );

	//	���ϊ�
	void	LVChange( LVERTEX& l,Vector3& p );
};

extern	Effect*		m_Effect;

//*******************************************************************************
#endif // !__EFFECT_H__
