
#pragma once

//**********************************************************************
//
//	Stage�N���X
//
//**********************************************************************

//	include
#include	<list>

//	class
class Stage
{
private:
	struct meshInfo
	{
		iexMesh*	obj;
		int			type;
	};

private:
	std::list<meshInfo*>		objList;
	iexMesh*						org[4];

private:
	//	�R���X�g���N�^�E�f�X�g���N�^
	Stage( void );
	~Stage( void );

public:
	//	�������E���
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	Append( Vector3 pos, Vector3 angle, Vector3 scale, int type );
	bool	GetReflect( Vector3& outPos, Vector3& outVec, float rate );
	bool	CheckWall( const Vector3& p_pos, Vector3& outVec );
	bool	CheckDown( const Vector3& p_pos, float& outHeight );

	//	���ݒ�

	//	���擾
	float	GetHeightToObject( const Vector3& pos );
	float	GetFrontToObject( const Vector3& pos );
	float	GetBackToObject( const Vector3& pos );
	float GetRightToObject( const Vector3& pos );
	float GetLeftToObject( const Vector3& pos );
	float	GetHeight( const Vector3& pos );
	float	GetFront( const Vector3& pos );
	float	GetBack( const Vector3& pos );
	float	GetRight( const Vector3& pos );
	float	GetLeft( const Vector3& pos );
	static	Stage*				GetInstance( void );
};


#define	stage ( Stage::GetInstance() )
