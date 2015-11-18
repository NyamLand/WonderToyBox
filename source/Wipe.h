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

private:
	//	�l�J����
	unique_ptr<Camera>		playerView[PLAYER_MAX];
	
	//	���C�v
	unique_ptr<iex2DObj>	wipe[PLAYER_MAX];

	//	�ϐ�
	Vector2		pos[PLAYER_MAX];
	Vector3		color[PLAYER_MAX];
	bool		check[PLAYER_MAX];
	float		ASPECT;

public:
	//	�������E���
	PlayerWipe( void );
	~PlayerWipe( void );
	bool	Initialize( void );

	//	�X�V
	void	Update( void );
	void	Set( void );
	void	Check( float angle, int num );

	//	�`��
	void	Render( void );
	void	RenderTarget( int num );

	//	���擾
	static	PlayerWipe*		GetInstance();

};
#define	 playerWipe ( PlayerWipe::GetInstance() )


#endif	//	!__WIPE_H__