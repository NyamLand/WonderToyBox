
#ifndef __COIN_H__
#define	__COIN_H__

//*****************************************************************************
//
//	Coinクラス
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
	bool			absorbedflg; // 掃除屋に吸収されている状態
	int				judgeTimer;	//	判定有効時間

	//	physics_parameter
	float	mass;
	Vector3	position;
	Vector3	velocity;
	Vector3	acceleration;
	Vector3	resultant;

	//	影構造体
	SHADOW_INFO	shadow;

public:
	//	初期化・解放
	Coin( void );
	~Coin( void );
	virtual	bool	Initialize( void );

	//	更新・描画
	void	ShadowUpdate( void );
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	動作関数
	void	StageCollisionCheck( void );
	void	PlayerCollisionCheck( void );
	virtual	void	Move( void );
	virtual	void	Hitduringtheoperation( const Vector3& pos, const int& Num );	//ヒット時動作
	void	GetAway( void );
	void	Magnet( void );

	//	物理用動作関数
	void	Integrate( float dt );
	void	AddForce( const Vector3& force );
	bool	RayPick( LVERTEX v[3], Vector3& out_hitPos, Vector3& out_normal, Vector3 pos, Vector3 vec, float dist );
	void	Collide( void );

	//	情報取得・設定
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
