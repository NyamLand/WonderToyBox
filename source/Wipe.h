#ifndef	__WIPE_H__
#define	__WIPE_H__

//*******************************************************************************
//	
//	Wipe�N���X
//
//*******************************************************************************

class PlayerWipe
{
private:
		//	�萔
	static	const		int		WIPE_WIDTH = 250;
	static	const		int		WIPE_HEIGHT = 250;
	static	const		int		SPACE = 50;

						float	WIPE_RIGHT;
						float	WIPE_LEFT;
						float	WIPE_UP;
						float	WIPE_DOWN;
						float	LEN_MAX;

private:
	//	�l�J����
	unique_ptr<Camera>		playerView[PLAYER_MAX];
	
	//	���C�v
	unique_ptr<iex2DObj>	wipe[PLAYER_MAX];

	//	�摜
	ImageObj	arrow;

	//	�ϐ�
	Vector3		pos[PLAYER_MAX];
	Vector3		color[PLAYER_MAX];
	bool		check[PLAYER_MAX];
	Vector3		arrow_pos[PLAYER_MAX];
	float		arrow_angle[PLAYER_MAX];
	

public:
	//	�������E���
	PlayerWipe( void );
	~PlayerWipe( void );
	bool	Initialize( void );

	//	�X�V
	void	Update( void );
	void	Set( void );
	void	Check( int num );
	void	Arrow( int num, Vector3 target );
	void	Move( int num, Vector3 target );
	Vector3	LengthChecker( int num, Vector3 target );

	//	�`��
	void	Render( void );
	void	RenderTarget( int num );

	//	���擾
	static	PlayerWipe*		GetInstance();

};
#define	 playerWipe ( PlayerWipe::GetInstance() )


#endif	//	!__WIPE_H__