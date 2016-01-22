
#ifndef		__COLLISION_H__
#define		__COLLISION_H__

//****************************************************************************************
//
//	CollisionNX
//
//****************************************************************************************
class Collision
{
private:
	static	iexMesh*	obj;

public:
	enum
	{
		SPHEREVSCAPSULE = 1,	//	VSJvZ
		SPHEREVSSPHERE,			//	VS
		CAPSULEVSCAPSULE,			//	JvZVSJvZ
		SPHEREVSCYRINDER,			//	VS~i~Μρ]Θ΅j
		SPHEREVSPOINT,				//	VS_
	};
	
public:
	//	ϊ»Eπϊ
	Collision( void );
	~Collision( void );
	static	bool	Initiallize( iexMesh* org );
	static	void	Release( void );

	//	Xe[W»θ
	static	float	GetHeight(const Vector3& pos);
	static	float	GetHeight(const Vector3& pos, float dist);
	static	bool	CheckWall( const Vector3& pos, Vector3& p_move );
	static	bool	CheckWall( iexMesh* org, const Vector3& pos, Vector3& p_move );
	static	bool	CheckDown( Vector3& pos, Vector3& p_move, Vector3& Out = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	bool	CheckDown( iexMesh*	org, const Vector3& pos, float& outHeight );
	static	bool	CheckWallOut( iexMesh* org, Vector3& outPos, Vector3& outVec );
	
	//	Xe[W»θ
	static	float	GetHeight( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	float	GetFront( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	float	GetBack( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	float	GetRight( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	static	float	GetLeft( iexMesh* org, const Vector3& pos, Vector3& outHitPos = Vector3( 0.0f, 0.0f, 0.0f ) );
	
	//	½Λ
	static	bool	DistCheck( const Vector3& p1, const Vector3& p2, float dist );
	static	bool	GetRefrectFix( iexMesh* org, Vector3& pos, Vector3& vec, float rate );
	static	bool	GetReflect( Vector3& pos, Vector3& vec, float rate );
	static	bool	GetReflect( iexMesh* org, Vector3& pos, Vector3& vec, float rate );
	static	bool	GetReflectFloor( const float& height, Vector3& pos, Vector3& vec, float rate );

	//	ήΏ»θ
	static	int		GetMaterial( const Vector3& pos );

	//	½θ»θ
	static	float DistancePointSegmentSq( const Vector3& p1, const Vector3& l1p1, const Vector3& l1p2 );
	static	float DistanceSegmentSegmentSq( const Vector3& l1p1, const Vector3& l1p2, const Vector3& l2p1, const Vector3& l2p2 );
	static	bool	CapsuleVSCapsule( Vector3 p1, Vector3 p2, float r1, Vector3 p3, Vector3 p4, float r2 );
	static	bool	CapsuleVSSphere( const Vector3& p1, const Vector3& p2, float r1, const Vector3& c, float r2 );
	static	bool	SphereVSSphere( Vector3 c1, float r1, Vector3 c2, float r2 );
};


//****************************************************************************************
#endif // !__COLLISION_H__
