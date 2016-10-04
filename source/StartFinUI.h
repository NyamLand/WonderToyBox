//*********************************************************************************
//
//	StartFinUIクラス
//
//*********************************************************************************
class StartFinUI
{
private:
	int		count;
	Vector3 s_pos;
	Vector3 f_pos;
	float	s_t;
	int		s_step;
	int		waitTimer;
	bool	changeflag;

	ImageObj	s_obj;
	ImageObj	f_obj;

public:
	//	初期化・解放
	StartFinUI( void );
	~StartFinUI( void );

	//	更新
	void StartUpdate( void );
	bool StartMove( void );
	void FinishUpdate( void );

	//	描画
	void StartRender( void );
	void FinishRender( void );

	//	情報設定・取得
	void SetChangeFlag( const bool& flag );
	bool GetChangeFlag( void );

};