
#ifndef __RANDOM_H__
#define	__RANDOM_H__

//*******************************************************************************
//
//	Randomクラス
//
//*******************************************************************************

//	include
#include	<random>
using namespace std;

//	class
class Random
{
private:
	static	std::mt19937* rnd;

public:
	//	初期化・解放
	static	bool	Initialize();
	static	void	Release();

	//	動作関数
	static	int		GetInt( int min, int max );
	static	float		GetFloat( float min, float max );
	static	bool		PercentageRandom( float probability );
};

//*******************************************************************************
#endif // !__RANDOM_H__
