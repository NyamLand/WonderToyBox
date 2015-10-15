
#ifndef		__COLLISION_H__
#define		__COLLISION_H__

//****************************************************************************************
//
//	Collision�N���X
//
//****************************************************************************************
class Collision
{
private:
	static	iexMesh*	obj;

public:
	//	�������E���
	Collision( void );
	~Collision( void );
	static	bool	Initiallize( iexMesh* org );
	static	void	Release( void );

	//	�X�e�[�W����
	static	float	GetHeight(const Vector3 pos);
	static	float	GetHeight(const Vector3 pos, float dist);
	static	bool	CheckWall( const Vector3 pos, Vector3& p_move );

	//	����
	static	bool	DistCheck( const Vector3 p1, const Vector3 p2, float dist );
	static	bool	GetReflect( Vector3& pos, Vector3& vec, float rate );
	static	bool	GetReflect( iexMesh* org, Vector3& pos, Vector3& vec, float rate );

	//	�ގ�����
	static	int		GetMaterial(const Vector3& pos);

	//	�����蔻��
	static	float DistancePointSegmentSq( const Vector3& p1, const Vector3& l1p1, const Vector3& l1p2 );
	static	float DistanceSegmentSegmentSq( const Vector3& l1p1, const Vector3& l1p2, const Vector3& l2p1, const Vector3& l2p2 );
	static	bool	CapsuleVSCapsule( Vector3 p1, Vector3 p2, float r1, Vector3 p3, Vector3 p4, float r2 );
	static	bool	CapsuleVSSphere( Vector3 p1, Vector3 p2, float r1, Vector3 c, float r2 );
	static	bool	SphereVSSphere( Vector3 c1, float r1, Vector3 c2, float r2 );
	static	void	CoinCollide( void );
};


//****************************************************************************************
#endif // !__COLLISION_H__
