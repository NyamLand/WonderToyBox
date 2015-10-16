
#pragma once

//*******************************************************************************
//
//	sceneMenuクラス
//
//*******************************************************************************

//	include
#include	<memory>
using namespace std;

//	class
class sceneMenu	:	public	Scene
{
private:

private:
	unique_ptr<iex2DObj>		wipe;
	ImageObj							wipeImage;
	
public:
	//	初期化・解放
	sceneMenu( void );
	~sceneMenu( void );
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	SelectCharacter( void );
	void	SelectStage( void );
	void	SelectCheck( void );
	void	MoveMain( void );
};
