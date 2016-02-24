
#pragma once

//**********************************************************************
//
//	Stage�N���X
//
//**********************************************************************

//	include
#include	<list>
#include	"Object.h"

//	define

//	class
class Stage
{
public:
	enum
	{
		DESK,
		TOY,
	};

private:
	static	const	int	OBJ_MAX = 20;
	Object*					object[OBJ_MAX];	//	���X�g
	iexMesh*					org[OBJECT_TYPE::END];
	iexMesh*					collisionObj[OBJECT_TYPE::END];
	iexMesh*					forestRiver;
	Vector3					dirLightVec;
	float							adjustV;
	int							objectID;
	int							stageType;

public:
	//	�������E���
	Stage( void );
	~Stage( void );
	bool	Initialize( void );
	bool	LightInitialize( Vector3	dir );
	void	Release( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	ObjUpdate(void);
	void	Render( iexShader*	shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	void	Append( Vector3 pos, Vector3 angle, Vector3 scale, int moveType, int objType );
	bool	GetReflect( Vector3& outPos, Vector3& outVec, float rate );
	bool	CheckWall( const Vector3& p_pos, Vector3& outVec );
	bool	CheckDown( const Vector3& p_pos, float& outHeight );
	void	SubDurableValue( int id );

	//	���ݒ�
	void	SetUnrivaled( int id, bool state );

	//	���擾
	bool	GetUnrivaled( int id );
	Vector3	GetTempPos( int id );
	Vector3	GetMove( int id );
	float	GetHeightToObject( const Vector3& pos, Vector3& outTempPos, int& outId, int id = 0 );
	float	GetFrontToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id = 0 );
	float	GetBackToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id = 0 );
	float	GetRightToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id = 0 );
	float	GetLeftToObject( const Vector3& pos, Vector3& outHitPos, int& outId, int id = 0 );
	float	GetHeight( const Vector3& pos );
	float	GetFront( const Vector3& pos );
	float	GetBack( const Vector3& pos );
	float	GetRight( const Vector3& pos );
	float	GetLeft( const Vector3& pos );
	bool	GetLocal( Vector3 &localPos, Vector3 &localAngle);
	void	GetWorld( Vector3 &worldPos, Vector3 &worldAngle);
};

extern	Stage* stage;
