
#pragma once

//***********************************************************************
//
//	ShieldItemクラス
//
//***********************************************************************

//	include
#include	"Item.h"

//	class
class ShieldItem : public	Item
{
private:
public:
	//	初期化・解放
	ShieldItem( void );
	~ShieldItem( void );

	//	動作関数
	void	Hitduringtheoperation( const Vector3& pos, const int& playerNum )override;
};
