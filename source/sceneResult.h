
#ifndef __SCENERESULT_H__
#define	__SCENERESULT_H__

//*******************************************************************************
//
//	sceneResultクラス
//
//*******************************************************************************

//	include
#include	<memory>
#include	"Camera.h"
using namespace std;

//	class
class sceneResult : public	Scene
{
private:
	//	数字情報
	struct NUMBER_INFO
	{
		int		hundred;		//コイン三桁目
		int		ten;			//コイン二桁目
		int		one;			//コイン一桁目
		bool	H_flg;			//百の位レンダー用フラグ
	};

	//	並べかえる情報
	struct SORT_INFO
	{
		int		num;
		int		sortRank;
		int		rank;
	};

	//	元のデータ
	struct ORIGINAL_INFO
	{
		int		num;			//	ゲーム終了時のコイン枚数
		int		rank;			//	ゲーム終了時の順位
		int		bonus;			//	ボーナス
	};

	//	ラストボーナスデータ
	struct LASTBONUS_INFO
	{
		int		bonus[4];
		int		step;
		float	t;
		ImageObj	textImage;
		TLVERTEX	v[4];
	};

	//	コイン枚数構造体
	struct NUMBERIMAGE_INFO
	{
		int			scale;
		POINT	pos;
		ImageObj		hundred;		//	コイン三桁目
		ImageObj		ten;				//	コイン二桁目
		ImageObj		one;				//	コイン一桁目
		bool	hundredRenderFlag;	//	百の位レンダー用フラグ
	};

	//	ルーレット構造体
	struct ROULETTE_INFO
	{
		int		step;
		int		timer;
	};

	//	メニュー構造体
	struct MENU_INFO
	{
		int		select;
		int		screenH;
		float	alpha;
		float	t;
	};

	//	ランク表示用構造体
	struct VIEW_RANK_INORDER
	{
		int		timer;
		int		step;
	};

	//	カメラ情報
	struct VIEW_INFO
	{
		Vector3	pos;
		Vector3	target;
		Vector3	texPos;		//	描画位置
		LVERTEX	v[4];		//	描画用頂点
	};

	//	ライフ発表情報
	struct LIFE_INFO
	{
		int		step;
		bool		renderflag;
		float		t;
		int		waitTimer;
		bool		isEnd;
		int		culLife;
		int		maxW;
		int		maxH;
	};

private:
	SORT_INFO						sortInfo[4];
	ORIGINAL_INFO				originInfo[4];
	NUMBER_INFO					number[4];
	NUMBER_INFO					bonusNumber[4];
	NUMBERIMAGE_INFO		numberImageInfo[4];
	NUMBERIMAGE_INFO		bonusNumberImageInfo[4];
	ROULETTE_INFO				rouletteInfo;
	MENU_INFO						menuInfo;
	VIEW_RANK_INORDER	viewRankInOrder;
	LASTBONUS_INFO			lastBonusInfo;
	VIEW_INFO						viewInfo;

	//	ラストボーナス計算用
	SORT_INFO						maxCoinNum[4];
	SORT_INFO						fallStageNum[4];
	SORT_INFO						coin77[4];
	SORT_INFO						minCoinNum[4];
	SORT_INFO						hitAttackNum[4];
	
	//	モデル・画像
	Surface*							backBuffer;
	unique_ptr<Camera>		view2D;
	iex2DObj*						infoScreen;
	unique_ptr<iex2DObj>	back;
	unique_ptr<iex3DObj>	org[4];
	iexMesh*							bgStage;
	iex3DObj*						obj[4];
	iex2DObj*						originNumber;
	iex2DObj*						menuText;
	iex2DObj*						lastBonusText;
	iex2DObj*						life;
	ImageObj							menuHead;
	ImageObj							originCoinImage[4];
	ImageObj							rankImage[4];
	ImageObj							menuImage[3];
	ImageObj							playerNumImage;
	ImageObj							faceImage;
	ImageObj							waveCircleImage;
	ImageObj							notApplicable;		//	該当なし画像
	ImageObj							lifeImage[4];		//	ライフ画像
	LIFE_INFO						lifeInfo;

	//	変数
	int		mode;
	int		step;
	int		lastBonus;
	int		bonusPlayer;
	bool		changeScene;

public:
	//	初期化・解放
	sceneResult( void );
	~sceneResult( void );
	bool	Initialize( void );
	void	Release( void );

	//	各種情報初期化
	void	CameraInitialize( void );
	void	ModelInitialize( void );
	void	ResultInfoInitialize( void );
	void	NumberImageInfoInitialize( void );
	void	RankImageInitialize( void );
	void	LifeImageInitialize( void );

	//	全体更新・描画
	void	Update( void );
	void	Render( void );

	//	各モード更新
	void	ResultUpdate( void );
	void	SelectUpdate( void );
	void	IntermediateResultsUpdate( void );	//	中間結果

	//	各画像描画
	void	SelectRender( void );
	void	NumberImageRender( void );
	void	RankRender( void );
	void	LifeRender( void );
	
	//	動作関数
	void	SetLastBonus( void );
	void	AddLastBonus( void );
	void	SetBonusPlayer( void );
	void	Sort( SORT_INFO( &sort_info )[4] );
	void	ReverseSort( SORT_INFO( &sort_info )[4] );
	bool	Roulette( void );
	bool	ViewRankInOrder( void );
	bool	RankWave( void );
	void	MoveScene( void );
	bool	NextLifeAnnouncing( void );
	void	ProductionSkip( void );

	//	選択モード関数
	bool	DownPolygon( void );
	void	ControlCursor( void );

	//	リザルトモード
	void	ModeRoulette( void );
	void	ModeLastBonus( void );
	void	ModeRank( void );
	void	ModeRankSkip( void );

	//	ラストボーナス用関数
	bool	LastBonusUpdate( void );
	bool	InBoard( void );
	bool	BonusAnnouncing( void );
	bool	PlayerAnnouncing( void );
	bool	OutBoard( void );
	bool	AddBonus( void );

	//	情報設定
	void	SetNextLife( void );
	void	SetRank( void );
	void	SetNumberImageInfo( const int& player, const int& coin );
	void	SetNumberImageInfo( NUMBERIMAGE_INFO& numImageInfo, NUMBER_INFO& numInfo, const int& num );
	void	SetNumberInfo( NUMBER_INFO& number, int coin );	//	コイン枚数引き渡し
};

//*******************************************************************************
#endif // !__SCENERESULT_H__
