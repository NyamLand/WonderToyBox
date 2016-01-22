
//--------------------------------------------------------------------------------
//	テクスチャサンプラー
//--------------------------------------------------------------------------------

	//	デカールサンプラー
	texture	Texture;
	sampler	DecaleSamp = sampler_state
	{
		Texture = <Texture>;
		MinFilter = POINT;
		MagFilter = POINT;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	被写界深度
	texture	DepthTex;
	sampler	DepthSamp = sampler_state
	{
		Texture = <DepthTex>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

//--------------------------------------------------------------------------------
//	頂点フォーマット
//--------------------------------------------------------------------------------
	
	//	２D用頂点フォーマット
	struct VS_2D
	{
		float4	Pos		:	POSITION0;
		float4	Color		:	COLOR0;
		float2	Tex		:	TEXCOORD0;
	};

//-----------------------------------------------------------------------------------
//	被写界深度
//-----------------------------------------------------------------------------------

	//	フォーカス
	float	FocusDist = 10.0f;		//	焦点距離
	float	FocusRange = 5.0f;	//	範囲


//************************************************************************
//
//	2D描画と合成
//
//************************************************************************

	//--------------------------------------------------------------------------------
	//	頂点シェーダー
	//--------------------------------------------------------------------------------

		//	なにもしない
		VS_2D	VS_Basic( VS_2D In )
		{
			return	In;
		}

	//--------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//--------------------------------------------------------------------------------

		//	頂点シェーダーから受け取ったピクセル情報をもとにピクセル色を決定
		float4	PS_Basic( VS_2D In )		:	COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			return	OUT;
		}

//************************************************************************
//
//	スクリーンフィルタ
//
//************************************************************************
	
	//--------------------------------------------------------------------------------
	//	フィルタ情報の定義
	//--------------------------------------------------------------------------------

		float3	screenColor = { 1.2f, 1.1f, 0.8f };
		float		contrast = 1.8f;
		float		saturate = 0.6f;
		float		brightness = 0.0f;
	
	//--------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//--------------------------------------------------------------------------------

		float4	PS_Filter( VS_2D In )		:	COLOR
		{
			float4	OUT;

			float3	col = In.Color.rgb * tex2D( DecaleSamp, In.Tex );

			//	輝度調整
			col += brightness;

			//	コントラスト調整
			col = ( ( col - 0.5f ) * contrast ) + 0.5f;
			
			//	彩度調整
			float	avr = ( col.r + col.g + col.b ) / 3;

			//	カラーバランス調整
			col *= screenColor;

			//	ピクセル色決定
			OUT.rgb = col;
			OUT.w = In.Color.a;

			return	OUT;
		}

//************************************************************************
//
//	被写界深度
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Focus( VS_2D In )	:	COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			//	深度・焦点・範囲をもとに透明度計算
			float	a = tex2D( DepthSamp, In.Tex ).r;
			a = abs( a - FocusDist ) / FocusRange;
			OUT.a *= a > 1 ? 1 : a;

			return	OUT;
		}

//************************************************************************
//
//	ワイプ用エフェクト
//
//************************************************************************

	//--------------------------------------------------------------------------------
	//	パラメータ
	//--------------------------------------------------------------------------------
		float		wipe_size = 100.0f;
		float		frame_size = 10.0f;
		int			center_posX = 0;
		int			center_posY = 0;
		int			picture_width = 0;
		int			picture_height = 0;
		float3	frame_color = float3( 0.0f, 0.0f, 1.0f );

	//--------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//--------------------------------------------------------------------------------

		//	ピクセルシェーダー
		float4	PS_Wipe( VS_2D In, float2 pPos : VPOS ) : COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			//	ピクセルの中心からの位置を計算
			float p1 = pPos.x - ( center_posX + ( picture_width * 0.5f ) );
			float p2 = pPos.y - ( center_posY + ( picture_height * 0.5f ) );

			//	フレームのサイズを含むサイズを設定
			float	renderSize = ( wipe_size + frame_size ) * ( wipe_size + frame_size );
			float	wipeSize = p1 * p1 + p2 * p2;

			//	設定したサイズ以内は表示、外は透明にする
			if ( wipeSize >= renderSize )		OUT.a = 0.0f;
			else	OUT.a = 1.0f;

			//	フレーム分を省いたサイズを設定
			renderSize = wipe_size * wipe_size;

			//	フレーム部分を設定した色にかえる
			if ( wipeSize >= renderSize )		OUT.rgb = frame_color;

			return	OUT;
		}

//************************************************************************
//
//	スポットライトエフェクト
//
//************************************************************************

	//--------------------------------------------------------------------------------
	//	パラメータ
	//--------------------------------------------------------------------------------
		float		light_Size[4] = { 200.0f, 150.0f, 200.0f, 150.0f };
		float		lightPower = 1.0f;
		float3	spotLightColor = float3( 0.4f, 0.4f, 0.4f );
		float3	lightPos[4] =
		{
			float3( 0.0f, 0.0f, 0.0f ),
			float3( 0.0f, 0.0f, 0.0f ),
			float3( 0.0f, 0.0f, 0.0f ),
			float3( 0.0f, 0.0f, 0.0f ),
		};

	//--------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//--------------------------------------------------------------------------------

		//	ピクセルシェーダー
		float4	PS_SpotLight( VS_2D In, float2 pPos : VPOS ) : COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			//	元々の色に明るさを指定
			float3	orgColor = OUT.rgb * lightPower;
			OUT.rgb = orgColor;

			//	ピクセルの中心からの位置を計算
			float p1, p2, renderSize, lightSize;
			for ( int i = 0; i < 4; i++ )
			{
				p1 = pPos.x - ( lightPos[i].x );
				p2 = pPos.y - ( lightPos[i].y );
												 
				//　ライトのサイズを設定
				renderSize = light_Size[i] * light_Size[i];
				lightSize = p1 * p1 + p2 * p2;

				//	ライトサイズ内の明るさを上げる
				if ( lightSize <= renderSize )		OUT.rgb = orgColor + spotLightColor;
			}

			return	OUT;
		}

//************************************************************************
//
//	テクニック
//
//************************************************************************
	
	//--------------------------------------------------------------------------------
	//	２Ｄ描画と合成
	//--------------------------------------------------------------------------------

		//	合成なし
		technique	copy
		{
			pass	P0
			{
				AlphaBlendEnable		=		true;
				BlendOp					=		Add;
				SrcBlend					=		SrcAlpha;
				DestBlend					=		InvSrcAlpha;
				CullMode					=		None;
				ZEnable						=		false;

				VertexShader		=	compile		vs_2_0	VS_Basic();
				PixelShader		=	compile		ps_2_0	PS_Basic();
			}
		}

		//	加算合成
		technique	add
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		One;
				CullMode				=		None;
				ZEnable					=		false;

				VertexShader		=		compile		vs_2_0	VS_Basic();
				PixelShader		=		compile		ps_2_0	PS_Basic();

			}
		}

		//	減算合成
		technique	sub
		{
			pass	P0
			{
				AlphaBlendEnable		=		true;
				BlendOp					=		RevSubtract;
				SrcBlend					=		SrcAlpha;
				DestBlend					=		One;
				CullMode					=		None;
				ZEnable						=		false;

				VertexShader		=		compile		vs_2_0	VS_Basic();
				PixelShader		=		compile		ps_2_0	PS_Basic();
			}
		}

	//--------------------------------------------------------------------------------
	//	スクリーンフィルタ
	//--------------------------------------------------------------------------------

		//	フィルター
		technique	filter
		{
			pass	P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				CullMode = None;
				ZEnable = false;

				VertexShader		=	compile		vs_2_0 VS_Basic();
				PixelShader		=	compile		ps_2_0	PS_Filter();
			}
		}

	//--------------------------------------------------------------------------------
	//	被写界深度
	//--------------------------------------------------------------------------------

		//	被写界深度
		technique	focus
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		None;
				ZEnable					=		false;

				PixelShader = compile ps_2_0 PS_Focus();
			}
		}

	//--------------------------------------------------------------------------------
	//	ワイプ処理
	//--------------------------------------------------------------------------------

		//	ワイプエフェクト
		technique WipeEffect
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				VertexShader = compile		vs_3_0 VS_Basic();
				PixelShader = compile		ps_3_0 PS_Wipe();
			}
		}

	//--------------------------------------------------------------------------------
	//	スポットライト処理
	//--------------------------------------------------------------------------------

		//	スポットライトエフェクト
		technique SpotLight
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				VertexShader = compile	vs_3_0 VS_Basic();
				PixelShader = compile		ps_3_0 PS_SpotLight();
			}
		}