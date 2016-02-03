
#include	"iextreme.h"
#include	"system\System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"

#include	"Item.h"
#include	"Effect.h"

//*******************************************************************************
//
//	Effectクラス
//
//*******************************************************************************

	//　実体宣言
	Effect*		m_Effect = nullptr;
	namespace{
		namespace{
			const float POS[3] =
			{
				3.0f,		//	最大
				-3.0f,		//	最少
				0.5f,		//	高さ
			};
			
			const float CON_POS[3] =
			{
				2.0f,		//	最大
				-2.0f,		//	最少
				0.5f,		//	高さ
			};
		}
	}

//-------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------

	//	コンストラクタ
	Effect::Effect( void ) : aura( nullptr ), isAura( false ), isStorm( false ), _adjustV( 0 )
	{
	
	}

	//	デストラクタ
	Effect::~Effect( void )
	{
		SafeDelete( aura );
		SafeDelete( pow_up.obj );
		SafeDelete( shieldInfo.obj );
		SafeDelete( confusion_pic );
		
		for ( int i = 0; i < 4; i++ )
		{
			SafeDelete( circle_pic[i] );
		}
	}

	//	初期化
	void	Effect::Initialize()
	{
		aura = new iexMesh("DATA/Effect/state/aura.IMO");
		storm = new iexMesh("DATA/Effect/cyclone/cyclone.IMO");
		storm_pos = Vector3(0.0f, 0.0f, 0.0f);
		storm_time = 0;

		circle_pic[0] = new iex2DObj("DATA/Effect/circle/PL1.png");
		circle_pic[1] = new iex2DObj("DATA/Effect/circle/PL2.png");
		circle_pic[2] = new iex2DObj("DATA/Effect/circle/PL3.png");
		circle_pic[3] = new iex2DObj("DATA/Effect/circle/PL4.png");
		confusion_pic = new iex2DObj("DATA/Effect/confusion.png");

		pow_up.obj = new iex2DObj( "DATA/Effect/ol-r.png" );
		shieldInfo.obj = new iex2DObj( "DATA/Effect/state/Guard-ef.png" );
		itemEffect.obj = new iex2DObj("DATA/UI/item-state.png");
		ImageInitialize(pow_up, 0, 0, 90, 90, 0, 0, 0, 0);
		ImageInitialize(itemEffect, 0, 0, 90, 90, 0, 0, 256, 256);

		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				//	頂点設定
				SetVertex(circle[i].poligon[j], 0, 0, 0, 0, 0, 0xFFFFFF);
				SetVertex(circle_out[i].poligon[j], 0, 0, 0, 0, 0, 0xFFFFFF);
				SetVertex(confusion[i].poligon[j], 0, 0, 0, 0, 0, 0xFFFFFF);
				SetVertex(confusion_out[i].poligon[j], 0, 0, 0, 0, 0, 0xFFFFFF);
			}
			circle[i].c_pos = Vector3( 0.0f, 10.0f, 0.0f );	
			circle_out[i].c_pos = Vector3( 0.0f, 10.0f, 0.0f );
			confusion[i].c_pos = Vector3( 0.0f, 10.0f, 0.0f );
			confusion_out[i].c_pos = Vector3(0.0f, 10.0f, 0.0f);
			isConfusion[i] = false;
		}

		circle_speed = 0.0f;
		confusion_speed = 0.0f;
		pow_pos = Vector3(0, 0, 0);
		pow_time = 0;

		item_pos = Vector3(0, 0, 0);
		
		//	シールド初期化
		ShieldInitialize();
	}

	//	シールド初期化
	void	Effect::ShieldInitialize( void )
	{
		for ( int n = 0; n < 4; n++ )
		{
			//	状態初期化
			shieldInfo.state[n] = false;

			for ( int i = 0; i < 4; i++ )
			{
				//	角度、座標初期化
				shieldInfo.shield[n].v[i].angle = ( D3DX_PI / 2 ) * i;
				shieldInfo.shield[n].pos[i] = Vector3( 0.0f, 0.0f, 0.0f );
			}
		}

		shieldInfo.scale = 4.0f;
		shieldInfo.r = 2.0f;
	}

//-------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------

	//	更新
	void	Effect::Update(void)
	{
		if (gameManager->GetMode() == GAME_MODE::DONKETSU_DIRECTION)
		{
			return;
		}
		//	オーラ
		//AuraUpdate();

		//	竜巻タイマー
		StormTimer();
		
		_adjustV += 0.02f;

		//	情報更新
		for (int i = 0; i < 4; i++){
			PoligonSet( &circle[i], POS[0], POS[1], POS[2] );
			PoligonSet( &circle_out[i], POS[0], POS[1], POS[2] );
			PoligonSet( &confusion[i], CON_POS[0], CON_POS[1], CON_POS[2] );
			PoligonSet( &confusion_out[i], CON_POS[0], CON_POS[1], CON_POS[2] );
			CirclePosSet( &circle[i], i, Vector3(0.0f, 0.4f, 0.0f) );
			CirclePosSet( &circle_out[i], i, Vector3(0.0f, 0.4f, 0.0f) );
			CirclePosSet( &confusion[i], i, Vector3(0.0f, 6.0f, 0.0f) );
			CirclePosSet( &confusion_out[i], i, Vector3(0.0f, 6.0f, 0.0f) );
		}
		//	回転
		Spin();
		circle_speed += 0.02f;		//	プレイヤー番号サークルスピード
		confusion_speed += 0.08f;

		//	パワー更新
		pow_time++;
		if ( pow_up.waveState == false ){
			SetWave( pow_up, 2.0f );
		}
		WaveUpdate( pow_up, 30, 0.3f );
		//particle->Arrow_DOWN(circle_out[1].c_pos, 1.5f);
		//particle->Arrow_UP(circle_out[0].c_pos, 1.5f);

		//	シールド更新
		Shield();

		//	アイテムエフェクト更新
		ItemEffectUpdate();


	}

	//	オーラ
	void	Effect::AuraUpdate( void )
	{
		if (gameManager->GetMode() == GAME_MODE::CLIMAX)	isAura = true;
		if (isAura)
		{
			Vector3 p = characterManager->GetPos(gameManager->GetWorst()) + Vector3(0, 1.5f, 0);
			aura->SetPos(p);
			aura->SetScale(0.02f);
			aura->SetAngle(0.0f);

			aura->Update();
		}
	}

	//	アイテムエフェクト
	void	Effect::ItemEffectUpdate( void )
	{
		ScalingAlphaUpdate(itemEffect, 100);	
		Lerp(item_pos, item_start, item_finish, itemEffect.t);
		itemEffect.x = (int)item_pos.x;	itemEffect.y = (int)item_pos.y;
	}

	//	プレイヤー番号でその場所にエフェクト(アイテム用)
	void	Effect::ItemEffectSet( int num ,int state)
	{
		WorldToClient(characterManager->GetPos(num), item_pos, matView * matProjection);
		item_start = item_pos + Vector3(0.0f, 0.0f, 0.0f);
		item_finish = item_pos + Vector3(0.0f, -100.0f, 0.0f);
		SetScaling(itemEffect, 1.0f, true);

		switch (state)
		{
		case ITEM_TYPE::ATTACK_UP:
			itemEffect.sx = 0;
			itemEffect.sy = 0;
			break;

		case ITEM_TYPE::UNRIVALED:
			itemEffect.sx = 256;
			itemEffect.sy = 0;
			break;

		case ITEM_TYPE::SPEED_UP:
			itemEffect.sx = 0;
			itemEffect.sy = 256;
			break;

		case ITEM_TYPE::MAGNET:
			itemEffect.sx = 256;
			itemEffect.sy = 256;
			break;
				
		}

	}

	//	竜巻
	void	Effect::StormSet( Vector3 pos, int time)
	{
		if (isStorm) return;


		isStorm = true;
		storm_pos = pos;
		storm_time = time;

		storm->SetPos(storm_pos);
		storm->SetScale(0.06f,0.02f,0.06f);
		storm->SetAngle(0.0f);
		storm->Update();

	}

	void	Effect::StormTimer( void )
	{
		if (storm_time > 0)
		{
			storm_time--;
		}
		else
		{
			isStorm = false;
		}
	}

	//	回転
	void	Effect::Spin( void )
	{
		//	頂点回転
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
			{
				//	サークル
				circle_out[i].poligon[n].x = cosf(circle_speed) * (circle[i].poligon[n].x - circle[i].c_pos.x) - sinf(circle_speed) * (circle[i].poligon[n].z - circle[i].c_pos.z) + circle[i].c_pos.x;
				circle_out[i].poligon[n].z = sinf(circle_speed) * (circle[i].poligon[n].x - circle[i].c_pos.x) + cosf(circle_speed) * (circle[i].poligon[n].z - circle[i].c_pos.z) + circle[i].c_pos.z;

				//	混乱
				confusion_out[i].poligon[n].x = cosf(confusion_speed) * (confusion[i].poligon[n].x - confusion[i].c_pos.x) - sinf(confusion_speed) * (confusion[i].poligon[n].z - confusion[i].c_pos.z) + confusion[i].c_pos.x;
				confusion_out[i].poligon[n].z = sinf(confusion_speed) * (confusion[i].poligon[n].x - confusion[i].c_pos.x) + cosf(confusion_speed) * (confusion[i].poligon[n].z - confusion[i].c_pos.z) + confusion[i].c_pos.z;
			}
		}
	}

	//	描画
	void	Effect::Render( void )
	{

		shader3D->SetValue("adjustV", _adjustV);
		if(isAura)	aura->Render(shader3D,"effect_add");
		if (isStorm) storm->Render(shader3D, "effect_add");

		//	サークル描画
		for (int i = 0; i < 4; i++){
			iexPolygon::Render3D(circle_out[i].poligon, 2, circle_pic[i], shader3D, "alpha" );

			//	混乱描画
			if (isConfusion[i])
			{
				iexPolygon::Render3D(confusion_out[i].poligon, 2, confusion_pic, shader3D, "alpha");
			}
		}

		//	シールド描画
		RenderShield();

		for ( int i = 0; i < 4; i++ ){
			//	プレイヤーのポジションより2高い位置を取る
			WorldToClient(circle_out[i].c_pos + Vector3(0, 2.0f, 0), pow_pos, matView* matProjection);
			pow_up.x = (int)pow_pos.x;	pow_up.y = (int)pow_pos.y;
			
			//	ベース（NORMAL）のパラメータ用
			pow_up.renderflag = true;	pow_up.alpha = 0.3f;

		}

		RenderImage(itemEffect, itemEffect.sx, itemEffect.sy, itemEffect.sw, itemEffect.sh, IMAGE_MODE::SCALING);
	}

	//	シールド描画
	void	Effect::RenderShield( void )
	{
		//	シールド描画
		for ( int i = 0; i < 4; i++ )
		{
			if ( shieldInfo.state[i] )
			{
				for ( int n = 0; n < 4; n++ )
				{
					iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
					iexPolygon::Render3D( shieldInfo.shield[i].v[n].v, 2, shieldInfo.obj, RS_COPY );
					iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
				}
			}
		}
	}

//-------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------
	void	Effect::PoligonSet( Circle* c, float max, float min, float height )
	{
			//	保存用

			c->poligon[0].x = min	+ c->c_pos.x;
			c->poligon[0].y = height	+ c->c_pos.y;
			c->poligon[0].z = max	+ c->c_pos.z;
			c->poligon[0].tu = 0.0f;
			c->poligon[0].tv = 0.0f;
			c->poligon[0].color = 0xFFFFFFFF;

			c->poligon[1].x = max	+ c->c_pos.x;
			c->poligon[1].y = height	+ c->c_pos.y;
			c->poligon[1].z = max	+ c->c_pos.z;
			c->poligon[1].tu = 1.0f;
			c->poligon[1].tv = 0.0f;
			c->poligon[1].color = 0xFFFFFFFF;


			c->poligon[2].x = min + c->c_pos.x;
			c->poligon[2].y = height + c->c_pos.y;
			c->poligon[2].z = min + c->c_pos.z;
			c->poligon[2].tu = 0.0f;
			c->poligon[2].tv = 1.0f;
			c->poligon[2].color = 0xFFFFFFFF;


			c->poligon[3].x = max + c->c_pos.x;
			c->poligon[3].y = height + c->c_pos.y;
			c->poligon[3].z = min + c->c_pos.z;
			c->poligon[3].tu = 1.0f;
			c->poligon[3].tv = 1.0f;
			c->poligon[3].color = 0xFFFFFFFF;

	}

	void	Effect::CirclePosSet( Circle* c, int i, Vector3 add )
	{
			c->c_pos = characterManager->GetPos(i) + add;
	}


	void	LVChange( LVERTEX &l,Vector3 &p )
	{
		p.x = l.x;
		p.y = l.y;
		p.z = l.z;
	}


//-------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------

	//	シールド
	void	Effect::Shield( void )
	{
		//	プレイヤー４人分
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	ガード中じゃなかったらとばす
			if ( !shieldInfo.state[i] )	continue;

			//	座標設定
			shieldInfo.pos[i] = characterManager->GetPos( i );

			//	ポリゴン4枚分更新
			for ( int p = 0; p < 4; p++ )
			{
				//	各ポリゴン座標設定
				shieldInfo.shield[i].pos[p] = Vector3( shieldInfo.pos[i].x + shieldInfo.r * sinf( shieldInfo.shield[i].v[p].angle ), shieldInfo.pos[i].y + 2.0f, shieldInfo.pos[i].z + shieldInfo.r * cosf( shieldInfo.shield[i].v[p].angle ) );

				//	回転
				shieldInfo.shield[i].v[p].angle += 0.01f;

				//	4頂点更新
				for ( int n = 0; n < 4; n++ )
				{
					//	テクスチャUV設定
					float tu = static_cast<float>( n % 2 );
					float tv = static_cast<float>( n / 2 );

					//	頂点座標セット
					float x = shieldInfo.shield[i].pos[p].x - ( -1.0f * tu ) * ( shieldInfo.scale / 2.0f ) * cosf( shieldInfo.shield[i].v[p].angle );
					float	y = shieldInfo.shield[i].pos[p].y + cosf( D3DX_PI * tv ) * shieldInfo.scale / 2;
					float	z = shieldInfo.shield[i].pos[p].z - ( -1.0f * tu ) * ( shieldInfo.scale / 2.0f ) * -sinf( shieldInfo.shield[i].v[p].angle );

					//	頂点をセット
					SetVertex( shieldInfo.shield[i].v[p].v[n], x, y, z, tu, tv, 0xFFFFFFFF );
				}
			}
		}
	}

	//	シールド情報設定
	void	Effect::SetShield( int player, bool state )
	{
		shieldInfo.state[player] = state;
	}