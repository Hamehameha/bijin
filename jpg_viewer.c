//
// Image loading and displaying 
//
//   http://www.directfb.org/docs/DirectFB_Tutorials/image.html
//
//-----------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <directfb.h>

// LCDに表示する文字
char text[2][50] = {"Shikoku Polytechnique College ","" };

// DirectFBの機能を実現するためのスーパサーフェイス（最上位のインターフェース）
 static IDirectFB *dfb = NULL;
 
// プライマリ・サーフェイス（フル画面のプライマリレイヤ）
 static IDirectFBSurface *primary = NULL;

// キー入力（タッチパネル）イベントバッファ
static IDirectFBEventBuffer *keybuffer;

// 画面の解像度
static int screen_width  = 0;
static int screen_height = 0;

// DirectFBから呼び出されるエラーチェック用マクロ
#define DFBCHECK(x...)											\
{																\
	DFBResult err = x;											\
																\
	if (err != DFB_OK)											\
	{															\
		fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );	\
		DirectFBErrorFatal( #x, err );							\
	}															\
}																\

// LCDのサイズに合わせる
DFBRectangle resize={0,0,357,272};


//画像表示用サーフェイス（ロゴ画像）
static IDirectFBSurface *logo[2] = {NULL , NULL};
unsigned char logo_index = 1;

struct tm *s_time;		// システムタイム

void get_url(char * url, char n, char *proxy, char *title)
{

	switch (n){
	
	case '0':		// 美人時計（キッズ）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/kids/ http://www.bijint.com/kids/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"キッズ時計");
		break;
		
	case '1':	// 美人時計（名古屋）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/nagoya/ http://www.bijint.com/nagoya/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"名古屋美人時計");
		break;
		
	case '2':
		// 美人時計（サーキット）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/cc/ http://www.bijint.com/cc/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"サーキット");
		break;
		
	case '3':	// 美人時計（福岡）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/fukuoka/ http://www.bijint.com/fukuoka/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"福岡美人時計");
		break;
		
	case '4':	// 美人時計（金沢）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/kanazawa/ http://www.bijint.com/kanazawa/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"金沢美人時計");
		break;
		
	case '5':	// 美人時計（岡山）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/okayama/ http://www.bijint.com/okayama/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"岡山美人");
		break;
		
	case '6':	// 美人時計（北海道）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/hokkaido/ http://www.bijint.com/hokkaido/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"北海道美人時計");
		break;
				
	case '7':	// 美人時計（京都）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/kyoto/ http://www.bijint.com/kyoto/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"京都美人時計");
		break;
		
	case '8':	// 美人時計（香川）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/kagawa/ http://www.bijint.com/kagawa/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"香川美人時計");
		break;
		
	case '9':	// 美人時計（仙台）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/sendai/ http://www.bijint.com/sendai/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"仙台美人時計");
		break;
		
	case 'A':	// 美人時計（香港）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com.hk/hk/ http://www.bijint.com.hk/assets/pict/hk/590x450/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"香港美人時計");
		break;

	case 'B':	// 美人時計（GAL）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://gal.bijint.com/gal/ http://gal.bijint.com/assets/pict/gal/590x450/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"GAL時計");
		break;
		
	case 'C':	// 美人時計（美魔女）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/bimajo/ http://www.bijint.com/bimajyo/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"美魔女時計");
		break;

	case 'D':	// 美人時計（花嫁）
		sprintf(url, "/usr/local/bin/wget -e %s --referer=http://www.bijint.com/hanayome/ http://www.bijint.com/hanayome/tokei_images/%02d%02d.jpg;", proxy, s_time->tm_hour, s_time->tm_min);
		sprintf(title,"花嫁美人時計");
		break;
		
		case 'L': // ローカルネット上のWebサーバ(http://www/shikoku-pc.ac.jp/bijin_img/img_cc/)から画像を取得する
		sprintf(url, "/usr/local/bin/wget http://www.shikoku-pc.ac.jp/bijin_img/img_cc/%02d%02d.jpg;", s_time->tm_hour, s_time->tm_min);
		sprintf(title,"応用課程");
		break;
		
		
		case 'S': // ローカルなSDカードから画像を取得する
		sprintf(url, "cp /mnt/sd/img/%02d%02d.jpg .", s_time->tm_hour, s_time->tm_min);
		sprintf(title,"応用課程");
		break;
		
		
		

		
	default: // ローカルなSDカードから画像を取得する
		sprintf(url, "cp /mnt/sd/img/%02d%02d.jpg .", s_time->tm_hour, s_time->tm_min);
		sprintf(title,"応用課程");
	}
}

// 右にスライド表示
void r_shift_image(void)
{
	int i;
	
	//ロゴ画像を左から右にスライド
	for (i = 0; i < (screen_width + resize.w) / 2 + 4  ; i += 2)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));
		
		// 左（後）側の画像
		DFBCHECK (primary->Blit (primary, logo[ logo_index] , NULL, i - resize.w, 0));
			
		// 右（前）側の画像
		if (logo_index)
		{
			if (logo[0] != NULL)
				DFBCHECK (primary->Blit (primary, logo[0] , NULL, (screen_width - resize.w) / 2 + i , 0));
			}else{
				if (logo[1] != NULL)
					DFBCHECK (primary->Blit (primary, logo[1] , NULL, (screen_width - resize.w) / 2 + i, 0));
			}

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0xFF, 0xFF, 0x00, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 20, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}
}

// 左から右へ切り換え表示
void r_swap_image(void)
{

	DFBRectangle resize2={0,0,357,272};	// 右（前）側の画像用の窓

	int i;
	
	//ロゴ画像を左から右にスライド切り換え
	for (i = 1; i < resize.w  ; i += 2)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));
		
		// 左（後）側の画像
		DFBCHECK (primary->Blit (primary, logo[ logo_index] , NULL,  (screen_width - resize.w) / 2 , 0));
			
		// 右（前）側の画像
		resize2.x = i + 1;
		if (logo_index)
		{
			if (logo[0] != NULL)
				DFBCHECK (primary->Blit (primary, logo[0] , &resize2, (screen_width - resize.w) / 2 + i , 0));
			}else{
				if (logo[1] != NULL)
					DFBCHECK (primary->Blit (primary, logo[1] , &resize2, (screen_width - resize.w) / 2 + i , 0));
			}

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0xFF, 0xFF, 0x00, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 20, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}
}


// 左にスライド表示
void l_shift_image(void)
{
	int i;
	
	//ロゴ画像を右から左にスライド
	for (i = 0; i < (screen_width + resize.w) / 2 + 4 ; i += 2)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));
		
			
		// 左（前）側の画像
		if (logo_index)
		{
			if (logo[0] != NULL)
				DFBCHECK (primary->Blit (primary, logo[0] , NULL, (screen_width - resize.w) / 2 - i, 0));
			}else{
				if (logo[1] != NULL)
					DFBCHECK (primary->Blit (primary, logo[1] , NULL, (screen_width - resize.w) / 2 - i, 0));
			}
			
		// 右（後）側の画像
		DFBCHECK (primary->Blit (primary, logo[ logo_index] , NULL, screen_width - i, 0));

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 265, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}
}

// 左にスライド表示
void l_swap_image(void)
{
	int i;
	DFBRectangle resize2={0,0,357,272};	// 右（後）側の画像用の窓


	//ロゴ画像を右から左にスライド切り換え
	for (i = resize.w; i > 0  ; i -= 2)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));
		
			
		// 左（前）側の画像
		if (logo_index)
		{
			if (logo[0] != NULL)
				DFBCHECK (primary->Blit (primary, logo[0] , NULL, (screen_width - resize.w) / 2 , 0));
			}else{
				if (logo[1] != NULL)
					DFBCHECK (primary->Blit (primary, logo[1] , NULL, (screen_width - resize.w) / 2 , 0));
			}
			
		// 右（後）側の画像
		resize2.x = i;
		DFBCHECK (primary->Blit (primary, logo[ logo_index] , &resize2,  (screen_width - resize.w) / 2 + i, 0));

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 265, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}
}


// 上にスライド表示
void u_shift_image(void)
{
	int i;
	
	//ロゴ画像を右から左にスライド
	for (i = 0; i < (screen_height + resize.h) / 2 + 2 ; i += 2)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));
		
			
		// 上（前）側の画像
		if (logo_index)
		{
			if (logo[0] != NULL)
				DFBCHECK (primary->Blit (primary, logo[0] , NULL, (screen_width - resize.w) / 2, -i));
			}else{
				if (logo[1] != NULL)
					DFBCHECK (primary->Blit (primary, logo[1] , NULL, (screen_width - resize.w) / 2, - i));
			}
			
		// 下（後）側の画像
		DFBCHECK (primary->Blit (primary, logo[ logo_index] , NULL, (screen_width - resize.w) / 2, screen_height - i));

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0x00, 0xFF, 0xFF, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 20, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}
}

// 下にスライド表示
void d_shift_image(void)
{
	int i;
	
	//ロゴ画像を右から左にスライド
	for (i = 0; i < (screen_height + resize.h) / 2 + 2 ; i += 2)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));


		// 上（後）側の画像
		DFBCHECK (primary->Blit (primary, logo[ logo_index] , NULL, (screen_width - resize.w) / 2, i - resize.h));
			
		// 下（前）側の画像
		if (logo_index)
		{
			if (logo[0] != NULL)
				DFBCHECK (primary->Blit (primary, logo[0] , NULL, (screen_width - resize.w) / 2, i));
			}else{
				if (logo[1] != NULL)
					DFBCHECK (primary->Blit (primary, logo[1] , NULL, (screen_width - resize.w) / 2, i));
			}
			

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0x00, 0xFF, 0xFF, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 20, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}
}

// ズームイン表示（拡大と縮小）
void zoomin_image(void)
{
	int i;
	// ズームイン、ズームアウトサイズに合わせる
	DFBRectangle resize2={0,0,357,272};

	//ズームアウト
	for (i = 100 ; i >= 1   ; i -= 1)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));


//		// ズームアウト（前）側の画像
		resize2.x =  (screen_width - resize2.w) / 2;
		resize2.y =  (272 - resize2.h)/2;
		resize2.w = (357 * i) /100;
		resize2.h = (272 * i) /100;
			
		if (logo_index)
		{
			if (logo[0] != NULL)
				DFBCHECK (primary->StretchBlit (primary, logo[0] , &resize , &resize2 ));
			}else{
				if (logo[1] != NULL)
					DFBCHECK (primary->StretchBlit (primary, logo[1] , &resize , &resize2 ));
			}
			

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0x80, 0xFF, 0x80, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 265, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}

	//ズームイン
	for (i = 1 ; i <= 100   ; i += 1)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));


		// ズームイン（後）側の画像
		resize2.x =  (screen_width - resize2.w) / 2;
		resize2.y =  (272 - resize2.h)/2;
		resize2.w = (357 * i) / 100;
		resize2.h = (272 * i) / 100;
		DFBCHECK (primary->StretchBlit (primary, logo[ logo_index] ,  &resize , &resize2  ));

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0x80, 0xFF, 0x80, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 265, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}
}

// 領域分け切り換え表示（拡大と縮小）
void area_swap_image(void)
{

	DFBRectangle resize2={0,0,357,272};	// 左上（後）側の画像用の窓
	DFBRectangle resize3={0,0,357,272};	// 右下（前）側の画像用の窓

	int i;
	
	//ロゴ画像を左から右にスライド切り換え
	for (i = 1; i < resize.h  ; i += 1)
	{
		//画面をクリア
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		//画面全体をスライド(logからprimaryにずらしてコピー）
		//      DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - dsc.height) / 2));
		
		// 左（後）側の画像
		resize2.w = (357 * i) / resize.h ;
		resize2.h = i;
		resize2.x =  (screen_width - resize.w) / 2;
		resize2.y =  0; 
		DFBCHECK (primary->StretchBlit (primary, logo[ logo_index] , &resize , &resize2));
			
		// 右（前）側の画像		
		resize3.w = resize.w - resize2.w;
		resize3.h = resize.h - resize2.h; 
		resize3.x = resize2.x + resize2.w;
		resize3.y = resize2.h;
		if (logo_index)
		{
			if (logo[0] != NULL)
				DFBCHECK (primary->StretchBlit (primary, logo[0] ,  &resize , &resize3 ));
			}else{
				if (logo[1] != NULL)
					DFBCHECK (primary->StretchBlit (primary, logo[1] ,  &resize , &resize3 ));
			}

		//文字色の指定
   	DFBCHECK (primary->SetColor (primary, 0xFF, 0x80, 0xFF, 0x80));
		//文字を表示
		DFBCHECK (primary->DrawString (primary, text[0], -1, 60, 20, DSTF_LEFT));
		
		DFBCHECK (primary->SetColor (primary, 0xFF, 0x00, 0x00, 0x80));
		DFBCHECK (primary->DrawString (primary, text[1], -1, 240, 270, DSTF_LEFT));

		
		//文字色の指定
  	   DFBCHECK (primary->SetColor (primary, 0x00, 0x00, 0x00, 0xFF));

			
		//描画を反映（垂直帰線を待って）
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

	}
}





int main (int argc, char **argv)
{
	int width;
	char name[10]={'\0'};
	char stime[10];		// 現在の時間（時分）
	char command[200];
	static IDirectFBFont *font = NULL;
	char genre = '0';			// 表示画像のジャンル
	
	DFBInputEvent evt;	//入力イベント
	int xcur, ycur;	//タッチした座標位置
	
	unsigned char func = 0;

	//フォント属性用
	DFBFontDescription font_dsc;

	time_t the_time;	// 起点（GMT 1970年1月1日0時)からの時間（エポックタイム）

	// LCDのサイズに合わせる
	DFBRectangle resize={0,0,357,272};
 
	//サーフェイス
	DFBSurfaceDescription dsc; 

	//画像読み込み用イメージプロバイダ.
	IDirectFBImageProvider *provider;

	// DirectFB の初期化
	DFBCHECK (DirectFBInit (&argc, &argv));

	//スーパインターフェイスの生成
	DFBCHECK (DirectFBCreate (&dfb));
	
	//入力デバイス（タッチパネル）用イベントバッファ
	DFBCHECK(dfb->CreateInputEventBuffer(dfb, DICAPS_ALL,
							 DFB_FALSE, &keybuffer));

	//スーパインターフェイスのフルスクリーン指定
	DFBCHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
 
	//サーフェイスの設定（プライマリサーフェイス）
	dsc.flags = DSDESC_CAPS;
	dsc.caps  = DSCAPS_PRIMARY | DSCAPS_FLIPPING;

	//プライマリサーフェイス生成
	DFBCHECK (dfb->CreateSurface( dfb, &dsc, &primary ));
 
	//解像度の取得
	DFBCHECK (primary->GetSize (primary, &screen_width, &screen_height));
  
  //描画フォントの指定
  font_dsc.flags = DFDESC_HEIGHT;
  font_dsc.height = 24;
//  DFBCHECK (dfb->CreateFont (dfb, "/usr/local/font/decker.ttf", &font_dsc, &font));

// 日本語フォント（UTF-8Nで保存する）
  DFBCHECK (dfb->CreateFont (dfb, "/usr/local/font/cinecaption227.ttf", &font_dsc, &font));

  //フォントをサーフェイスに設定
  DFBCHECK (primary->SetFont (primary, font));

  //文字列を描画した場合の長さを調べます。
  DFBCHECK (font->GetStringWidth (font, text[0], -1, &width));

  
	time(&the_time);	// エポックタイム取得
	s_time=localtime(&the_time);	//ローカルタイムに変換
  
	sprintf(stime,"%02d%02d", s_time->tm_hour, s_time->tm_min);
	sprintf(text[1],"%4d/%02d/%02d %02d:%02d",  s_time->tm_year + 1900, s_time->tm_mon + 1, s_time->tm_mday, s_time->tm_hour, s_time->tm_min );
	
	genre =  '0' + (( s_time->tm_mon +  s_time->tm_mday +  s_time->tm_hour + s_time->tm_min)  % 10);	// 表示画像のジャンル
 		
	//　前回表示した画像を削除
	if (name[0] != '\0'){
		sprintf(command, "rm -f *.jpg");
		system(command);
	}
  
	// 液晶に表示する画像をダウンロード　./wget -e HTTP_PROXY=192.168.10.249:8080 --referer=http://www.bijint.com/kyoto/ http://www.bijint.com/kyoto/tokei_images/1520.jpg	
//	get_url(command , argv[1][0]);
	if (argc > 1){	
		if (argc == 2){
			if ((argv[1][0] == 'S') ||(argv[1][0] == 'L') )
				get_url(command , argv[1][0], "", text[0]);		// デフォルト画像を選択（ローカルのSDカードから）
			else
				get_url(command ,  genre , argv[1], text[0] );	// 画像ジャンルを選択とPROXY等の指定
		}else{
			if (argv[2][0] == '1')	// 時報ON
				get_url(command ,  genre , argv[1], text[0] );	// 画像ジャンルを選択とPROXY等の指定
			else
				get_url(command , argv[1][0], argv[2], text[0]);	// 画像ジャンルを選択とPROXY等の指定
		}
	}else
		get_url(command , 'S', "", text[0]);		// デフォルト画像を選択（ローカルのSDカードから）

	system(command);

	
	while (1){
		//　時間が変わったら,表示する画像を切り替え
		if (name[3] != stime[3]){
			if (logo_index) logo_index = 0; else logo_index = 1;	//切り替え
			
			sprintf(name, "%02d%02d.jpg", s_time->tm_hour, s_time->tm_min);
	
			//画像プロバイダを生成
			//  DFBCHECK (dfb->CreateImageProvider (dfb, name, &provider));
			DFBCHECK (dfb->CreateImageProvider (dfb, name, &provider));

			//画像プロバイダの設定値取得（画像ファイル情報、解像度、ピクセル当たりのビット数等）
			DFBCHECK (provider->GetSurfaceDescription (provider, &dsc));

			// 画像プロバイダの設定値でサーフェイスを生成
			if ( logo[logo_index] != NULL)
				logo[logo_index]->Release (logo[logo_index]);	//	これがないと４時間後にメモリリークで停止？
			DFBCHECK (dfb->CreateSurface( dfb, &dsc, &(logo[logo_index]) ));

			//ロゴ画像を表示（画像をlogoにコピー）リサイズ指定
			//  DFBCHECK (provider->RenderTo (provider, logo, NULL));
			resize.h =  screen_height;
			resize.w = (int) ((dsc.width * screen_height) / dsc.height);
			DFBCHECK (provider->RenderTo (provider, logo[logo_index], &resize));

			provider->Release (provider);
		}
		
		// スライド表示
		switch (func) {
			case 0:
				r_shift_image();  // 右にスライド
				break;
			
			case 5:
				l_shift_image();  // 左にスライド
				break;
		
			case 1:
				u_shift_image();  // 上にスライド
				break;
				
			case 6:
				d_shift_image();  // 下にスライド
				break;
			
			case 2:
				zoomin_image();
				break;
				
			case 3:
				r_swap_image();  // 右にスライド切り替え
				break;
			
			case 7:
				l_swap_image();  // 左にスライド切り替え
				break;
				
			case 4:
				area_swap_image();	// 斜め画像切り替え
				break;
		}
	
		if ( (++func) > 7) func = 0;


		

		if(1){	// 時間が変わるまで画像を静止させる場合は１にする

			while(stime[3] == name[3]){	//分が変化したか？
  
				time(&the_time);	// エポックタイム取得
				s_time=localtime(&the_time);	//ローカルタイムに変換
  
				sprintf(stime,"%02d%02d", s_time->tm_hour, s_time->tm_min);
				sprintf(text[1],"%4d/%02d/%02d %02d:%02d",  s_time->tm_year + 1900, s_time->tm_mon + 1, s_time->tm_mday, s_time->tm_hour, s_time->tm_min );
				
				genre =  '0' + (( genre + s_time->tm_mon +  s_time->tm_mday +  s_time->tm_hour + s_time->tm_min)  % 10);	// 表示画像のジャンル
				
				//sleep(1);
				// イベント待ち(タイムアウト付き 100ms）
				keybuffer->WaitForEventWithTimeout(keybuffer, 0, 100);

				//イベント取得
				while (keybuffer->GetEvent(keybuffer, DFB_EVENT(&evt)) == DFB_OK) {
					//イベントの種類
					switch (evt.type) {
			
						case DIET_BUTTONPRESS:	/* a (mouse) button is been pressed */
							printf("PRESS !! \n");
							
							// シャットダウンする。
							system("/usr/bin/amixer cset numid=13 100");
							system("/etc/init.d/S91music stop");
							system("/usr/local/bin/mpg123 -a hw:0,1 /usr/local/mp3/system/shutdown.mp3");
							system("poweroff");
							system("/usr/local/bin/mpg123 -a hw:0,1 /usr/local/mp3/system/shutdown.mp3");

							break;
				 
						case DIET_BUTTONRELEASE:	/* a (mouse) button is been released */
							printf("RELEASE !! (x,y)=(%d,%d)\n",xcur, ycur);
							break;
				
						case DIET_AXISMOTION:	/* mouse/joystick movement */
			
							switch (evt.axis) {
								case DIAI_X:	// X軸の値
				
									xcur = evt.axisabs;
									//printf("x=%d\n", evt.axisabs);
									break;
					
								case DIAI_Y:	// Y軸の値
									ycur = evt.axisabs;
									//printf("y=%d\n", evt.axisabs);
									break;
					
								default:
									printf("? evt.axis=%d\n", evt.axis);
							}
							break;
							
						default:
							printf("? evt.type=%d\n", evt.type);
					}
				}
			}

		}else{
			time(&the_time);	// エポックタイム取得
  			s_time=localtime(&the_time);	//ローカルタイムに変換
			sprintf(stime,"%02d%02d", s_time->tm_hour, s_time->tm_min);

		}
	  
		//　時間が変わったら,表示する画像を切り替え、前回表示した画像を削除
		if (name[3] != stime[3]){
		
			if (((stime[3] == '0') || (stime[3] == '5')) && (argc == 3) && (argv[2][0] == '1')) {
				// 時報再生
				system("/usr/bin/amixer cset numid=13 100");
				system("/etc/init.d/S91music stop");
				
				if ((s_time->tm_min == 0) || (s_time->tm_min == 15) || (s_time->tm_min == 30) || (s_time->tm_min == 45) )
					sprintf(command, "/usr/local/bin/mpg123 -a hw:0,1 /usr/local/mp3/jiho2_mp3/%s.mp3" , stime);
				else
					sprintf(command, "/usr/local/bin/mpg123 -a hw:0,1 /usr/local/mp3/jiho_mp3/%s.mp3" , stime);
								
				system(command);
				system("/etc/init.d/S91music start");
			}
			
			//　表示している画像ファイルを削除
			sprintf(command, "rm -f *.jpg");
			system(command);
	
 			// 液晶に表示する画像をダウンロード
			if (argc > 1){	
				if (argc == 2){
					if ((argv[1][0] == 'S') ||(argv[1][0] == 'L') )
						get_url(command , argv[1][0], "", text[0]);		// デフォルト画像を選択（ローカルのSDカードから）
					else
						get_url(command ,  genre , argv[1], text[0] );	// 画像ジャンルを選択とPROXY等の指定
				}else{
					if (argv[2][0] == '1')	// 時報ON
						get_url(command ,  genre , argv[1], text[0] );	// 画像ジャンルを選択とPROXY等の指定
					else
						get_url(command , argv[1][0], argv[2], text[0]);	// 画像ジャンルを選択とPROXY等の指定
				}
			}else
				get_url(command , 'S', "", text[0]);		// デフォルト画像を選択（ローカルのSDカードから）

			system(command);
	
		
		}else
			sleep(3);
		
	}

	//リソースを解放
	logo[0]->Release (logo[0]);
	logo[1]->Release (logo[1]);

	primary->Release (primary);
	dfb->Release (dfb);

	return 23;
}
    
