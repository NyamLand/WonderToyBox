
#ifndef		__COLLISION_H__
#define		__COLLISION_H__

//****************************************************************************************
//
//	Collisionクラス
//
//****************************************************************************************
class Collision
{
private:
	static	iexMesh*	obj;

public:
	enum
	{
		NONE = -1,				//　判定なし
		SPHEREVSCAPSULE = 1,	//	球VSカプセル
		SPHEREVSSPHERE,			//	球VS球
		CAPSULEVSCAPSULE,			//	カプセルVSカプセル
		CAPSULEVSCYRINDER,		//	球VS円柱（円柱の回転なし）
		CYRINDERVSCYRINDER,		//	球VS円柱（円柱の回転なし）
		SPHEREVSCYRINDER,			//	球VS円柱（円柱の回転なし）
		SPHEREVSPOINT,				//	球VS点
	};
	
public:
	//	初期化・解放
	Collision( void );
	~Collision( void );
	static	bool	Initiallize( iexMesh* org );
	static	void	Release( void );

	//	ステージ判定
	static	float	GetHeight(const Vector3& pos);
	static	float	GetHeight(const Vector3& pos, float dist);
	static	bool	CheckWall( const Vector3& pos, Vector3& p_move );
	static	bool	CheckWall( iexMesh* org, const Vector3& pos, Vector3& p_move );
	static	bool	CheckDown( Vector3& pos, Vector3& p_move, Vector3& Out = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	bool	CheckDown( iexMesh*	org, const Vector3& pos, float& outHeight );
	static	bool	CheckWallOut( iexMesh* org, Vector3& outPos, Vector3& outVec );
	
	//	ステージ判定
	static	float	GetHeight( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	float	GetFront( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	float	GetBack( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	float	GetRight( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	float	GetLeft( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	
	//	反射
	static	bool	DistCheck( const Vector3& p1, const Vector3& p2, float dist );
	static	bool	GetRefrectFix( iexMesh* org, Vector3& pos, Vector3& vec, float rate );
	static	bool	GetReflect( Vector3& pos, Vector3& vec, float rate );
	static	bool	GetReflect( iexMesh* org, Vector3& pos, Vector3& vec, float rate );
	static	bool	GetReflectFloor( const float& height, Vector3& pos, Vector3& vec, float rate );

	//	材質判定
	static	int		GetMaterial( const Vector3& pos );

	//	当たり判定
	static	float DistancePointSegmentSq( const Vector3& p1, const Vector3& l1p1, const Vector3& l1p2 );
	static	float DistanceSegmentSegmentSq( const Vector3& l1p1, const Vector3& l1p2, const Vector3& l2p1, const Vector3& l2p2 );
	static	bool	CapsuleVSCapsule( Vector3 p1, Vector3 p2, float r1, Vector3 p3, Vector3 p4, float r2 );
	static	bool	CapsuleVSSphere( const Vector3& p1, const Vector3& p2, float r1, const Vector3& c, float r2 );
	static	bool	CapsuleVSCyrinder( const Vector3& bottom1, const Vector3& top1, float r1, const Vector3& bottom2, const Vector3& top2, float r2 );
	static	bool	SphereVSSphere( Vector3 c1, float r1, Vector3 c2, float r2 );

	//	動作関数
	static	void	Vec3Transform( void );
};


//****************************************************************************************
#endif // !__COLLISION_H__
