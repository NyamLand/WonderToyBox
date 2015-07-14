
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
		float4	Pos		:	POSITION;
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