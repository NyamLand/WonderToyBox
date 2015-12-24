
#ifndef __COIN_H__
#define	__COIN_H__

//*****************************************************************************
//
//	Coin�N���X
//
//*****************************************************************************

//	include
#include	<memory>

//	data
namespace
{
	namespace COIN_MODE
	{
		enum
		{
			NORMAL,
			GETAWAY,
			MAGNET,
		};
	}
}

class Coin
{
protected:
	struct SHADOW_INFO
	{
		iex2DObj*	obj;
		Vector3		pos;
		LVERTEX	v[4];
		float			scale;
	};

protected:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	bool			getAwayflag;
	bool			absorbedflg; // �|�����ɋz������Ă�����
	int				judgeTimer;	//	����L������

	//	physics_parameter
	float	mass;
	Vector3	position;
	Vector3	velocity;
	Vector3	acceleration;
	Vector3	resultant;

	//	�e�\����
	SHADOW_INFO	shadow;

public:
	//	�������E���
	Coin( void );
	~Coin( void );
	virtual	bool	Initialize( void );

	//	�X�V�E�`��
	void	ShadowUpdate( void );
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	void	StageCollisionCheck( void );
	void	PlayerCollisionCheck( void );
	virtual	void	Move( void );
	virtual	void	Hitduringtheoperation( const Vector3& pos, const int& Num );	//�q�b�g������
	void	GetAway( void );
	void	Magnet( void );

	//	�����p����֐�
	void	Integrate( float dt );
	void	AddForce( const Vector3& force );
	bool	RayPick( LVERTEX v[3], Vector3& out_hitPos, Vector3& out_normal, Vector3 pos, Vector3 vec, float dist );
	void	Collide( void );

	//	���擾�E�ݒ�
	void	SetPos(const Vector3& pos);
	void	SetMove(const Vector3& move);
	void	SetAngle( const float& angle );
	void	SetScale( const float& angle );
	void	SetGetAwayFlag(bool flag);
	void	SetAbsorbedFlag(bool flag);
	void	SetState(bool state);
	Vector3	GetPos( void );
	float		GetAngle( void );
	bool		GetState( void );

private:
	friend	class CoinManager;
};

//*****************************************************************************
#endif // !__COIN_H__
