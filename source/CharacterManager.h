
#pragma once

//****************************************************************************************
//
//	CharacterManager�N���X
//
//****************************************************************************************

//	include
#include	"BaseChara.h"
#include	"Tiger.h"
#include	"Knight.h"
#include	"Princess.h"
#include	"Squirrel.h"
#include	"Tiger_CPU.h"
#include	"Knight_CPU.h"
#include	"Princess_CPU.h"
#include	"Squirrel_CPU.h"

//	class
class CharacterManager
{
private:
	//	�萔
	static const int OBJ_MAX = 4;
	static const int CHARACTER_MAX = 4;

private:
	BaseChara*	character[CHARACTER_MAX];
	bool	canHyper;		//	�n�C�p�[�A�[�c�������������p

private:
	CharacterManager( void );
	~CharacterManager( void );

public:
	//	�������E���
	bool	Initialize( int playerNum, int type, Vector3 pos, bool isPlayer = true );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	void	DonketsuBoost();
	void	RaiseStatus( int worst, int type );

	//	�����蔻��
	void	HitCheck( void );
	void	HitCheckSphereVSCapsule( BaseChara* bc1, BaseChara* bc2 );
	void	HitCheckCapsuleVSCapsule( BaseChara* bc1, BaseChara* bc2 );

	//	���擾
	Matrix	GetMatrix( int player )const;
	Vector3	GetPos( int player )const;
	Vector3	GetDamageColor( int player )const;
	float		GetAngle( int player )const;
	bool		GetUnrivaled( int player )const;
	bool		GetCanHyper( int player )const;
	bool		GetParameterState( int player, int type )const;
	int			GetAttackParam( int player )const;
	int			GetPlayerNum( int playerNum )const;
	int			GetPower( int playerNum )const;
	static	CharacterManager*	GetInstance();

	//	���ݒ�
	void		SetPos( int player, Vector3 pos );
	void		SetAngle( int player, float angle );
	void		SetScale( int player, float scale );
	void		SetKnockBackVec( int player, Vector3 vec );
	void		SetMode( int player, int mode );
	void		SetBoosting( int player, bool boosting );
	void		SetLeanFrame( int player, int leanframe );
	void		SetPassColor( int player, Vector3 color );
	void		SetParameterInfo( int player, int parameterInfo );
	void		SetKnockBackParam(BaseChara* bc1, BaseChara* bc2);
};

#define	characterManager ( CharacterManager::GetInstance() )
