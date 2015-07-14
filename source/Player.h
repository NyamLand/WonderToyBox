
#ifndef		__PLAYER_H__
#define		__PLAYER_H__

//****************************************************************************************
//
//	Player関連クラス
//
//****************************************************************************************
class Player : public	BaseObj
{
private:
	const float GRAVITY = -0.01f;
	
	//	定数
	static const int MIN_INPUT_STATE = 300;	//	スティック判定最小値

	enum Y2009Motion
	{
		STAND = 1,		//	立ち
		POSTURE,				//	構え
		RUN = 4,		//	走り
		ATTACK1,				//	攻撃１段階目
		ATTACK2,				//	攻撃２段階目
		ATTACK3,				//	攻撃３段階目
		JUMP
	};

	//	変数

public:
	//	初期化・解放
	Player( void );
	~Player( void );

	//	更新・描画
	void	Update( void );

	//	動作関数
	void	Move( void );
	void	Move( float speed );
	void	Attack( void );
	void	Damage( void );
	void	Jump( void );

	//	角度補正関数
	void	AngleAdjust( float speed );
	void	AngleAdjust( const Vector3& direction, float speed );
};

//****************************************************************************************
#endif // !__PLAYER_H__
