
#pragma once

//*******************************************************************************
//
//	sceneMenu�N���X
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
	//	�������E���
	sceneMenu( void );
	~sceneMenu( void );
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	SelectCharacter( void );
	void	SelectStage( void );
	void	SelectCheck( void );
	void	MoveMain( void );
};
