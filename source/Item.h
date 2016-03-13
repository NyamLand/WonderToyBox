
#ifndef __ITEM_H__
#define	__ITEM_H__

//*****************************************************************************
//
//	Itemクラス
//
//*****************************************************************************

//インクルード
#include	"Coin.h"

//	情報
namespace
{
	namespace ITEM_TYPE
	{
		enum
		{
			ATTACK_UP,
			SPEED_UP,
			MAGNET,
			UNRIVALED,
			LIFE,
			MAX,
			BOMB,
			SEALED_ATTACK,
			SELFISHNESS_JUMP,
			OTHERS_SPEED_DOWN,
		};
	}
}

class Item // :public Coin
{
private:
	struct SHADOW_INFO
	{
		iex2DObj*	obj;
		Vector3		pos;
		LVERTEX		v[4];
		float				scale;
	}; 

private:
	iexMesh*	obj;
	Vector3	pos;
	Vector3	move;
	float			scale;
	float			angle;
	float			stageHeight;
	bool			state;
	int		type;
	int		time;

	//	影構造体
	SHADOW_INFO	shadow;

public:
	//	初期化・解放
	Item( void );
	~Item( void );
	bool	Initialize( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );
	void	ObjectUpdate( void );
	void	ShadowUpdate( void );

	//	動作関数
	void	Move( void );
	virtual	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum );	//ヒット時動作
	void	StageCollisionCheck( void );
	void	PlayerCollisionCheck( void );
	
	//	情報設定
	void	SetPos( const Vector3& pos );
	void	SetMove( const Vector3& move );
	void	SetAngle( const float& angle );
	void	SetScale( const float& scale );
	void	SetMesh( iexMesh* obj );
	void	SetState( bool state );
	void	SetType( const int& type );

	//	情報取得
	int	GetType( void )const;
	bool	GetState( void )const;
	Vector3	GetPos( void )const;
	float	GetAngle( void )const;
};

//*****************************************************************************
#endif // !__ITEM_H__
