
#pragma once

//****************************************************************************************
//
//	CharacterManagerクラス
//
//****************************************************************************************

//	include
#include	"BaseChara.h"
#include	"Tiger.h"
#include	"Knight.h"
#include	"Princess.h"
#include	"Squirrel.h"
#include	"Scavenger.h"
#include	"Thief.h"
#include	"Pirate.h"

#include	"Tiger_CPU.h"
#include	"Knight_CPU.h"
#include	"Princess_CPU.h"
#include	"Squirrel_CPU.h"
#include	"Scavenger_CPU.h"
#include	"Thief_CPU.h"
#include	"Pirate_CPU.h"

//	class
class CharacterManager
{
private:
	//	定数
	static const int OBJ_MAX = 4;
	static const int CHARACTER_MAX = 4;

private:
	BaseChara*	character[CHARACTER_MAX];
	bool	canHyper;		//	ハイパーアーツ同時発動制限用

private:
	CharacterManager( void );
	~CharacterManager( void );

public:
	//	初期化・解放
	bool	Initialize( int playerNum, int type, Vector3 pos, bool isPlayer = true );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );
	void	DrawDebug();

	//	動作関数
	void	DonketsuBoost();
	void	RaiseStatus( int worst, int type );

	//	当たり判定
	void	HitCheck( void );
	void	HitCheckSphereVSCapsule( BaseChara* bc1, BaseChara* bc2 );
	void	HitCheckCapsuleVSCapsule( BaseChara* bc1, BaseChara* bc2 );
	void	HitCheckObjectCapsule( BaseChara* bc );
	void	HitCheckObjectSphere( BaseChara* bc );

	//ライフ処理
	void	AddLife(int player )const;
	void	SubLife(int player )const;

	//	情報取得
	Matrix	GetMatrix( int player )const;
	Vector3	GetPos( int player )const;
	Vector3	GetDamageColor( int player )const;
	Vector3	GetFront( int player )const;
	Vector3	GetRight( int player )const;
	Vector3	GetUp( int player )const;
	float		GetAngle( int player )const;
	float		GetTotalSpeed( int player )const;
	bool		GetUnrivaled( int player )const;
	bool		GetCanHyper( int player )const;
	bool		GetParameterState( int player, int type )const;
	int		GetAttackParam( int player )const;
	int		GetPlayerNum( int playerNum )const;
	int		GetPower( int playerNum )const;
	int		GetTotalPower( int player )const;
	int		GetRank( int player )const;
	int		GetMode( int player )const;
	int		GetAIMode(int player)const;
	int		GetLife( int player )const;
	static	CharacterManager*	GetInstance();

	//	情報設定
	void		SetPos( int player, Vector3 pos );
	void		SetAngle( int player, float angle );
	void		SetScale( int player, float scale );
	void		SetKnockBackVec( int player, Vector3 vec );
	void		SetMode( int player, int mode );
	void		SetAIMode( int player, int mode );
	//void		SetBoosting( int player, bool boosting );
	void		SetLeanFrame( int player, int leanframe );
	void		SetPassColor( int player, Vector3 color );
	void		SetParameterInfo( int player, int parameterInfo );
	void		SetRank(int player, int rank);
	void		SetForce(int player, float force);
	void		SetKnockBackParam(BaseChara* bc1, BaseChara* bc2);
	void		SetLife( int player, int life );
};

#define	characterManager ( CharacterManager::GetInstance() )
